/*
Regression Test for the Polar Encoder and Decoder

Copyright 2020 Ahmet Inan <inan@aicodix.de>
*/

#include <limits>
#include <random>
#include <chrono>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <functional>
#include "polar_helper.hh"
#include "polar_decoder.hh"
#include "polar_encoder.hh"
#include "polar_freezer.hh"
#include "polar_sequence.hh"

bool get_bit(const uint32_t *bits, int idx)
{
	return (bits[idx/32] >> (idx%32)) & 1;
}

int main()
{
	const int M = 20;
	const int N = 1 << M;
	const bool systematic = true;
#if 1
	typedef int8_t code_type;
#else
	typedef float code_type;
#endif

	std::random_device rd;
	typedef std::default_random_engine generator;
	typedef std::uniform_int_distribution<int> distribution;
	auto data = std::bind(distribution(0, 1), generator(rd()));
	auto frozen = new uint32_t[N/32];
	auto codeword = new code_type[N];
	auto temp = new code_type[N];

	double erasure_probability = 1. / 3.;
	int K = (1 - erasure_probability) * N;
	double design_SNR = 10 * std::log10(-std::log(erasure_probability));
	std::cerr << "design SNR: " << design_SNR << std::endl;
	double better_SNR = design_SNR + 0.5;//1.59175;
	std::cerr << "better SNR: " << better_SNR << std::endl;
	double probability = std::exp(-pow(10.0, better_SNR / 10));
	if (1) {
		auto freeze = new CODE::BhattacharyyaFreezer<M>;
		std::cerr << "sizeof(BhattacharyyaFreezer<M>) = " << sizeof(CODE::BhattacharyyaFreezer<M>) << std::endl;
		(*freeze)(frozen, M, K, probability);
		delete freeze;
	} else {
		auto sequence = new int[N];
		auto construct = new CODE::BhattacharyyaSequence<M>;
		std::cerr << "sizeof(BhattacharyyaSequence<M>) = " << sizeof(CODE::BhattacharyyaSequence<M>) << std::endl;
		(*construct)(sequence, M, probability);
		delete construct;
		for (int i = 0; i < N / 32; ++i)
			frozen[i] = 0;
		for (int i = 0; i < N - K; ++i)
			frozen[sequence[i]/32] |= 1 << (sequence[i]%32);
		delete[] sequence;
	}
	std::cerr << "Polar(" << N << ", " << K << ")" << std::endl;
	auto message = new code_type[K];
	auto decoded = new code_type[K];
	std::cerr << "sizeof(PolarDecoder<code_type, M>) = " << sizeof(CODE::PolarDecoder<code_type, M>) << std::endl;
	auto decode = new CODE::PolarDecoder<code_type, M>;

	auto orig = new code_type[N];
	auto noisy = new code_type[N];
	auto symb = new double[N];
	double low_SNR = std::floor(design_SNR-3);
	double high_SNR = std::ceil(design_SNR+5);
	double min_SNR = high_SNR, max_mbs = 0;
	int count = 0;
	std::cerr << "SNR BER Mbit/s Eb/N0" << std::endl;
	for (double SNR = low_SNR; count <= 3 && SNR <= high_SNR; SNR += 0.1, ++count) {
		//double mean_signal = 0;
		double sigma_signal = 1;
		double mean_noise = 0;
		double sigma_noise = std::sqrt(sigma_signal * sigma_signal / (2 * std::pow(10, SNR / 10)));

		typedef std::normal_distribution<double> normal;
		auto awgn = std::bind(normal(mean_noise, sigma_noise), generator(rd()));

		int64_t awgn_errors = 0;
		int64_t quantization_erasures = 0;
		int64_t uncorrected_errors = 0;
		int64_t ambiguity_erasures = 0;
		double avg_mbs = 0;
		int64_t loops = 0;
		while (uncorrected_errors < 1000 && ++loops < 100) {
			for (int i = 0; i < K; ++i)
				message[i] = 1 - 2 * data();

			if (systematic) {
				CODE::PolarSysEnc<code_type> sysenc;
				sysenc(codeword, message, frozen, M);
				for (int i = 0, j = 0; i < N; ++i)
					if (!get_bit(frozen, i))
						assert(codeword[i] == message[j++]);
			} else {
				CODE::PolarEncoder<code_type> encode;
				encode(codeword, message, frozen, M);
			}

			for (int i = 0; i < N; ++i)
				orig[i] = codeword[i];

			for (int i = 0; i < N; ++i)
				symb[i] = codeword[i];

			for (int i = 0; i < N; ++i)
				symb[i] += awgn();

			// $LLR=log(\frac{p(x=+1|y)}{p(x=-1|y)})$
			// $p(x|\mu,\sigma)=\frac{1}{\sqrt{2\pi}\sigma}}e^{-\frac{(x-\mu)^2}{2\sigma^2}}$
			double DIST = 2; // BPSK
			double fact = DIST / (sigma_noise * sigma_noise);
			for (int i = 0; i < N; ++i)
				codeword[i] = CODE::PolarHelper<code_type>::quant(fact * symb[i]);

			for (int i = 0; i < N; ++i)
				noisy[i] = codeword[i];

			auto start = std::chrono::system_clock::now();
			(*decode)(decoded, codeword, frozen, M);
			auto end = std::chrono::system_clock::now();
			auto usec = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
			double mbs = (double)K / usec.count();
			avg_mbs += mbs;

			if (systematic) {
				CODE::PolarEncoder<code_type> encode;
				encode(temp, decoded, frozen, M);
				for (int i = 0, j = 0; i < N; ++i)
					if (!get_bit(frozen, i))
						decoded[j++] = temp[i];
			}

			for (int i = 0; i < N; ++i)
				awgn_errors += noisy[i] * (orig[i] < 0);
			for (int i = 0; i < N; ++i)
				quantization_erasures += !noisy[i];
			for (int i = 0; i < K; ++i)
				uncorrected_errors += decoded[i] * message[i] <= 0;
			for (int i = 0; i < K; ++i)
				ambiguity_erasures += !decoded[i];
		}

		avg_mbs /= loops;

		max_mbs = std::max(max_mbs, avg_mbs);
		double bit_error_rate = (double)uncorrected_errors / (double)(K * loops);
		if (!uncorrected_errors)
			min_SNR = std::min(min_SNR, SNR);
		else
			count = 0;

		int MOD_BITS = 1; // BPSK
		double code_rate = (double)K / (double)N;
		double spectral_efficiency = code_rate * MOD_BITS;
		double EbN0 = 10 * std::log10(sigma_signal * sigma_signal / (spectral_efficiency * 2 * sigma_noise * sigma_noise));

		if (0) {
			std::cerr << SNR << " Es/N0 => AWGN with standard deviation of " << sigma_noise << " and mean " << mean_noise << std::endl;
			std::cerr << EbN0 << " Eb/N0, using spectral efficiency of " << spectral_efficiency << " from " << code_rate << " code rate and " << MOD_BITS << " bits per symbol." << std::endl;
			std::cerr << awgn_errors << " errors caused by AWGN." << std::endl;
			std::cerr << quantization_erasures << " erasures caused by quantization." << std::endl;
			std::cerr << uncorrected_errors << " errors uncorrected." << std::endl;
			std::cerr << ambiguity_erasures << " ambiguity erasures." << std::endl;
			std::cerr << bit_error_rate << " bit error rate." << std::endl;
			std::cerr << avg_mbs << " megabit per second." << std::endl;
		} else {
			std::cout << SNR << " " << bit_error_rate << " " << avg_mbs << " " << EbN0 << std::endl;
		}
	}
	std::cerr << "QEF at: " << min_SNR << " SNR, speed: " << max_mbs << " Mb/s." << std::endl;
	double QEF_SNR = design_SNR + 0.2;
	assert(min_SNR < QEF_SNR);
	std::cerr << "Polar regression test passed!" << std::endl;
	return 0;
}

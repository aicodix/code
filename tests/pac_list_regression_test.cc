/*
Regression Test for the Polarization-Adjusted Convolutional Encoder and List Decoder

Copyright 2025 Ahmet Inan <inan@aicodix.de>
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
#include "pac_list_decoder.hh"
#include "pac_encoder.hh"
#include "polar_sequence.hh"
#include "crc.hh"

int main()
{
	double R = 0.5;
	const int MAX_M = 8;
	const int MAX_N = 1 << MAX_M;
	const int M = 7;
	const int N = 1 << M;
	const bool crc_aided = true;
	CODE::CRC<uint16_t> crc(0xA8F4);
	const int C = 16;
	int K = R * N + crc_aided * C;
#if 1
	const int L = 64;
	typedef int8_t code_type;
#else
	const int L = 16;
	typedef float code_type;
#endif

	typedef SIMD<code_type, L> simd_type;

	std::random_device rd;
	typedef std::default_random_engine generator;
	typedef std::uniform_int_distribution<int> distribution;
	auto data = std::bind(distribution(0, 1), generator(rd()));
	auto codeword = new code_type[N];

	double erasure_probability = 1 - R;
	double design_SNR = 10 * std::log10(-std::log(erasure_probability));
	std::cerr << "design SNR: " << design_SNR << std::endl;
	auto construct = new CODE::ReedMullerSequence<MAX_M>;
	std::cerr << "sizeof(ReedMullerSequence<MAX_M>) = " << sizeof(CODE::ReedMullerSequence<MAX_M>) << std::endl;
	auto rel_seq = new int[MAX_N];
	(*construct)(rel_seq, MAX_M);
	delete construct;
	auto rank_map = new uint8_t[N];
	for (int i = 0, j = 0; i < MAX_N && j < N; ++i) {
		int index = rel_seq[i];
		if (index < N)
			rank_map[index] = j++;
	}
	std::cerr << "PAC(" << N << ", " << K << ")" << std::endl;
	auto message = new code_type[K];
	auto decoded = new simd_type[K];
	std::cerr << "sizeof(PACListDecoder<simd_type, MAX_M>) = " << sizeof(CODE::PACListDecoder<simd_type, MAX_M>) << std::endl;
	auto decode = new CODE::PACListDecoder<simd_type, MAX_M>;

	auto orig = new code_type[N];
	auto noisy = new code_type[N];
	auto symb = new double[N];
	double low_SNR = std::floor(design_SNR-3);
	double high_SNR = std::ceil(design_SNR+5);
	double min_SNR = high_SNR, max_mbs = 0;
	int count = 0;
	std::cerr << "SNR FER BER Mbit/s Eb/N0" << std::endl;
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
		int64_t frame_errors = 0;
		double avg_mbs = 0;
		int64_t loops = 0;
		while (uncorrected_errors < 10000 && ++loops < 1000) {
			if (crc_aided) {
				crc.reset();
				for (int i = 0; i < K-C; ++i) {
					bool bit = data();
					crc(bit);
					message[i] = 1 - 2 * bit;
				}
				for (int i = 0; i < C; ++i) {
					bool bit = (crc() >> i) & 1;
					message[K-C+i] = 1 - 2 * bit;
				}
			} else {
				for (int i = 0; i < K; ++i)
					message[i] = 1 - 2 * data();
			}

			CODE::PACEncoder<code_type> encode;
			encode(codeword, message, rank_map, K, M);

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

			int rank[L];
			auto start = std::chrono::system_clock::now();
			(*decode)(rank, decoded, codeword, rank_map, K, M);
			auto end = std::chrono::system_clock::now();
			auto usec = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
			double mbs = (double)K / usec.count();
			avg_mbs += mbs;

			int best = 0;
			if (crc_aided) {
				bool error = true;
				for (int k = 0; k < L; ++k) {
					crc.reset();
					for (int i = 0; i < K; ++i)
						crc(decoded[i].v[k] < 0);
					if (crc() == 0) {
						best = k;
						error = false;
						break;
					}
				}
				frame_errors += error;
			} else {
				bool error = rank[0] == rank[1];
				for (int i = 0; i < K; ++i)
					error |= decoded[i].v[0] * message[i] <= 0;
				frame_errors += error;
			}

			for (int i = 0; i < N; ++i)
				awgn_errors += noisy[i] * (orig[i] < 0);
			for (int i = 0; i < N; ++i)
				quantization_erasures += !noisy[i];
			for (int i = 0; i < K; ++i)
				uncorrected_errors += decoded[i].v[best] * message[i] <= 0;
			for (int i = 0; i < K; ++i)
				ambiguity_erasures += !decoded[i].v[best];
		}

		avg_mbs /= loops;

		max_mbs = std::max(max_mbs, avg_mbs);
		double frame_error_rate = (double)frame_errors / (double)loops;
		double bit_error_rate = (double)uncorrected_errors / (double)(K * loops);
		if (!uncorrected_errors)
			min_SNR = std::min(min_SNR, SNR);
		else
			count = 0;

		int MOD_BITS = 1; // BPSK
		double spectral_efficiency = R * MOD_BITS;
		double EbN0 = 10 * std::log10(sigma_signal * sigma_signal / (spectral_efficiency * 2 * sigma_noise * sigma_noise));

		if (0) {
			std::cerr << SNR << " Es/N0 => AWGN with standard deviation of " << sigma_noise << " and mean " << mean_noise << std::endl;
			std::cerr << EbN0 << " Eb/N0, using spectral efficiency of " << spectral_efficiency << " from " << R << " code rate and " << MOD_BITS << " bits per symbol." << std::endl;
			std::cerr << awgn_errors << " errors caused by AWGN." << std::endl;
			std::cerr << quantization_erasures << " erasures caused by quantization." << std::endl;
			std::cerr << uncorrected_errors << " errors uncorrected." << std::endl;
			std::cerr << ambiguity_erasures << " ambiguity erasures." << std::endl;
			std::cerr << frame_error_rate << " frame error rate." << std::endl;
			std::cerr << bit_error_rate << " bit error rate." << std::endl;
			std::cerr << avg_mbs << " megabit per second." << std::endl;
		} else {
			std::cout << SNR << " " << frame_error_rate << " " << bit_error_rate << " " << avg_mbs << " " << EbN0 << std::endl;
		}
	}
	std::cerr << "QEF at: " << min_SNR << " SNR, speed: " << max_mbs << " Mb/s." << std::endl;
	double QEF_SNR = design_SNR + 2;
	assert(min_SNR < QEF_SNR);
	std::cerr << "Polarization-Adjusted Convolutional list regression test passed!" << std::endl;
	return 0;
}

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
#include "sequence.h"

bool get_bit(const uint32_t *bits, int idx)
{
	return (bits[idx/32] >> (idx%32)) & 1;
}

int main()
{
	const int M = 7;
	assert(M <= 10);
	const int N = 1 << M;
	const bool crc_aided = false;
	CODE::CRC<uint32_t> crc(0xD419CC15);
	const int C = 32;
#if 1
	const int L = 64;
	typedef int8_t code_type;
#else
	const int L = 8;
	typedef float code_type;
#endif

	typedef SIMD<code_type, L> simd_type;

	std::random_device rd;
	typedef std::default_random_engine generator;
	typedef std::uniform_int_distribution<int> distribution;
	auto data = std::bind(distribution(0, 1), generator(rd()));
	auto frozen = new uint32_t[N/32];
	auto codeword = new code_type[N];

	double erasure_probability = 0.5;
	int K = (1 - erasure_probability) * N;
	double design_SNR = 10 * std::log10(-std::log(erasure_probability));
	std::cerr << "design SNR: " << design_SNR << std::endl;
	for (int i = 0; i < N / 32; ++i)
		frozen[i] = 0;
	const int *reliability_sequence;
	if (1) {
		auto construct = new CODE::ReedMullerSequence<10>;
		std::cerr << "sizeof(ReedMullerSequence<M>) = " << sizeof(CODE::ReedMullerSequence<M>) << std::endl;
		auto rel_seq = new int[1024];
		(*construct)(rel_seq, 10);
		delete construct;
		reliability_sequence = rel_seq;
	} else {
		reliability_sequence = sequence;
	}
	for (int i = 0, j = 0; i < 1024 && j < N - K; ++i) {
		int index = reliability_sequence[i];
		if (index < N) {
			frozen[index/32] |= 1 << (index%32);
			++j;
		}
	}
	std::cerr << "Polar(" << N << ", " << K << ")" << std::endl;
	auto message = new code_type[K];
	auto decoded = new simd_type[K];
	std::cerr << "sizeof(PACListDecoder<simd_type, M>) = " << sizeof(CODE::PACListDecoder<simd_type, M>) << std::endl;
	auto decode = new CODE::PACListDecoder<simd_type, M>;

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
			encode(codeword, message, frozen, M);

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
			(*decode)(rank, decoded, codeword, frozen, M);
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
		double code_rate = (double)(K - crc_aided * C) / (double)N;
		double spectral_efficiency = code_rate * MOD_BITS;
		double EbN0 = 10 * std::log10(sigma_signal * sigma_signal / (spectral_efficiency * 2 * sigma_noise * sigma_noise));

		if (0) {
			std::cerr << SNR << " Es/N0 => AWGN with standard deviation of " << sigma_noise << " and mean " << mean_noise << std::endl;
			std::cerr << EbN0 << " Eb/N0, using spectral efficiency of " << spectral_efficiency << " from " << code_rate << " code rate and " << MOD_BITS << " bits per symbol." << std::endl;
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

/*
Regression Test for the Polar Encoder and Decoders

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
#include "polar_sequence.hh"

bool get_bit(const uint32_t *bits, int idx)
{
	return (bits[idx/32] >> (idx%32)) & 1;
}

int main()
{
	const int M = 20;
	const int N = 1 << M;
	const bool systematic = false;
#if 0
	typedef int8_t code_type;
	double SCALE = 2;
#else
	typedef float code_type;
	double SCALE = 1;
#endif

	std::random_device rd;
	typedef std::default_random_engine generator;
	typedef std::uniform_int_distribution<int> distribution;
	auto data = std::bind(distribution(0, 1), generator(rd()));
	auto frozen = new uint32_t[N/32];
	auto codeword = new code_type[N];
	auto temp = new code_type[N];

	double erasure_probability = 1. / 2.;
	int K = (1 - erasure_probability) * N;
	double design_SNR = 10 * std::log10(-std::log(erasure_probability));
	std::cerr << "design SNR: " << design_SNR << std::endl;
	double better_SNR = design_SNR + 0.5;//1.59175;
	std::cerr << "better SNR: " << better_SNR << std::endl;
	double probability = std::exp(-pow(10.0, better_SNR / 10));
	auto sequence = new int[N];
	auto construct = new CODE::PolarSeqConst0<M>;
	std::cerr << "sizeof(PolarSeqConst0<M>) = " << sizeof(CODE::PolarSeqConst0<M>) << std::endl;
	(*construct)(sequence, M, probability);
	delete construct;
	for (int i = 0; i < N / 32; ++i)
		frozen[i] = 0;
	for (int i = 0; i < N - K; ++i)
		frozen[sequence[i]/32] |= 1 << (sequence[i]%32);
	const int segments = 4;
	auto segment_mapping = new char[N];
	for (int i = 0; i < N; ++i)
		segment_mapping[i] = -1;
	if (0) {
		for (int i = 0; i < K; ++i)
			segment_mapping[sequence[N-1-i]] = std::min(i / (K / segments), segments - 1);
		for (int i = 0, j = 0; i < N; ++i)
			if (segment_mapping[i] >= 0)
				segment_mapping[j++] = segment_mapping[i];
	} else {
		for (int i = 0; i < K; ++i)
			segment_mapping[i] = std::min(i / (K / segments), segments - 1);
	}
	std::cerr << "Polar(" << N << ", " << K << ")" << std::endl;
	auto message = new code_type[K];
	auto decoded = new code_type[K];
	std::cerr << "sizeof(PolarDecoder<code_type, M>) = " << sizeof(CODE::PolarDecoder<code_type, M>) << std::endl;
	auto decode = new CODE::PolarDecoder<code_type, M>;

	auto symb = new double[N];
	double low_SNR = std::floor(design_SNR-3);
	double high_SNR = std::ceil(design_SNR+.5);
	for (double SNR = low_SNR; SNR <= high_SNR; SNR += 0.1) {
		//double mean_signal = 0;
		double sigma_signal = 1;
		double mean_noise = 0;
		double sigma_noise = std::sqrt(sigma_signal * sigma_signal / (2 * std::pow(10, SNR / 10)));

		typedef std::normal_distribution<double> normal;
		auto awgn = std::bind(normal(mean_noise, sigma_noise), generator(rd()));

		int64_t segment_errors[segments] = { 0 };
		int64_t loops = 0;
		while (segment_errors[0] < 100000 && ++loops < 10000) {
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
				symb[i] = codeword[i];

			for (int i = 0; i < N; ++i)
				symb[i] += awgn();

			// $LLR=log(\frac{p(x=+1|y)}{p(x=-1|y)})$
			// $p(x|\mu,\sigma)=\frac{1}{\sqrt{2\pi}\sigma}}e^{-\frac{(x-\mu)^2}{2\sigma^2}}$
			double DIST = 2; // BPSK
			double fact = SCALE * DIST / (sigma_noise * sigma_noise);
			for (int i = 0; i < N; ++i)
				codeword[i] = CODE::PolarHelper<code_type>::quant(fact * symb[i]);

			(*decode)(decoded, codeword, frozen, M);

			if (systematic) {
				CODE::PolarEncoder<code_type> encode;
				encode(temp, decoded, frozen, M);
				for (int i = 0, j = 0; i < N; ++i)
					if (!get_bit(frozen, i))
						decoded[j++] = temp[i];
			}

			for (int i = 0; i < K; ++i)
				segment_errors[(int)segment_mapping[i]] += decoded[i] * message[i] <= 0;
		}
		for (int i = 0; i < segments - 1; ++i)
			segment_errors[i + 1] += segment_errors[i];

		std::cout << SNR;
		for (int i = 0; i < segments; ++i)
			std::cout << " " << double(segments * segment_errors[i]) / double(K * loops * (i + 1));
		std::cout << std::endl;
	}
	return 0;
}

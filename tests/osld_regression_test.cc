/*
Regression Test for ordered statistics list decoding

Copyright 2024 Ahmet Inan <inan@aicodix.de>
*/

#include <random>
#include <cassert>
#include <iostream>
#include <functional>
#include "osd.hh"
#include "galois_field.hh"
#include "bose_chaudhuri_hocquenghem_encoder.hh"
#include "bose_chaudhuri_hocquenghem_decoder.hh"

int main()
{
	const int L = 16;
	const bool oracle = true;
#if 1
	// BCH(127, 64) T=10
	const int O = 4;
	const int N = 127;
	const int K = 64;
	const int NR = 20;
	const int loops = 10;
	const double low_SNR = -5;
	const double high_SNR = 5;
	const double QEF_SNR = -2;
	typedef CODE::GaloisField<7, 0b10001001, uint8_t> GF;
	std::initializer_list<int> minpols {
		0b10001001, 0b10001111, 0b10011101,
		0b11110111, 0b10111111, 0b11010101,
		0b10000011, 0b11101111, 0b11001011,
	};
#endif
#if 0
	// NASA INTRO BCH(15, 5) T=3
	const int O = 1;
	const int N = 15;
	const int K = 5;
	const int NR = 6;
	const int loops = 100000;
	const double low_SNR = -7;
	const double high_SNR = 7;
	const double QEF_SNR = 3.5;
	typedef CODE::GaloisField<4, 0b10011, uint8_t> GF;
	std::initializer_list<int> minpols {
		0b10011, 0b11111, 0b00111
	};
#endif
	GF instance;
	const int NW = (N+7)/8;
	const int KW = (K+7)/8;
	const int PW = (N-K+7)/8;
	int8_t genmat[N*K];
	CODE::BoseChaudhuriHocquenghemGenerator<N, K>::matrix(genmat, true, minpols);
	CODE::BoseChaudhuriHocquenghemEncoder<N, K> bchenc(minpols);
	CODE::LinearEncoder<N, K> linenc;
	CODE::OrderedStatisticsListDecoder<N, K, O, L> osddec;
	CODE::BoseChaudhuriHocquenghemDecoder<NR, 1, K, GF> bchdec;
	GF::value_type erasures[NR];
	uint8_t message[KW], parity[PW], decoded[NW*L], codeword[NW];
	int8_t orig[N], noisy[N];

	std::random_device rd;
	typedef std::default_random_engine generator;
	typedef std::uniform_int_distribution<int> distribution;
	auto data = std::bind(distribution(0, 255), generator(rd()));

	int rank[L];
	double symb[N];
	double min_SNR = high_SNR;
	for (double SNR = low_SNR; SNR <= high_SNR; SNR += 0.1) {
		//double mean_signal = 0;
		double sigma_signal = 1;
		double mean_noise = 0;
		double sigma_noise = std::sqrt(sigma_signal * sigma_signal / (2 * std::pow(10, SNR / 10)));

		typedef std::normal_distribution<double> normal;
		auto awgn = std::bind(normal(mean_noise, sigma_noise), generator(rd()));

		int awgn_errors = 0;
		int quantization_erasures = 0;
		int uncorrected_errors = 0;
		int ambiguity_erasures = 0;
		int frame_errors = 0;
		int bchdec_errors = 0;
		for (int l = 0; l < loops; ++l) {
			for (int i = 0; i < KW; ++i)
				message[i] = data();

			linenc(codeword, message, genmat);

			for (int i = 0; i < K; ++i)
				assert(CODE::get_be_bit(codeword, i) == CODE::get_be_bit(message, i));

			bchenc(message, parity);

			for (int i = K; i < N; ++i)
				assert(CODE::get_be_bit(codeword, i) == CODE::get_be_bit(parity, i-K));

			for (int i = 0; i < N; ++i)
				orig[i] = 1 - 2 * CODE::get_be_bit(codeword, i);

			for (int i = 0; i < N; ++i)
				symb[i] = orig[i];

			for (int i = 0; i < N; ++i)
				symb[i] += awgn();

			// $LLR=log(\frac{p(x=+1|y)}{p(x=-1|y)})$
			// $p(x|\mu,\sigma)=\frac{1}{\sqrt{2\pi}\sigma}}e^{-\frac{(x-\mu)^2}{2\sigma^2}}$
			double DIST = 2; // BPSK
			double fact = DIST / (sigma_noise * sigma_noise);
			for (int i = 0; i < N; ++i)
				noisy[i] = std::min<double>(std::max<double>(std::nearbyint(fact * symb[i]), -127), 127);

			osddec(rank, decoded, noisy, genmat);
			bool unique = rank[0] != rank[1];

			if (oracle) {
				for (int j = 0; j < L; ++j) {
					bool found = true;
					for (int i = 0; i < N; ++i)
						found &= CODE::get_be_bit(decoded+NW*j, i) == CODE::get_be_bit(codeword, i);
					if (found) {
						for (int i = 0; j && i < NW; ++i)
							decoded[i] = decoded[NW*j+i];
						unique = true;
						break;
					}
				}
			}

			for (int i = 0; i < N; ++i)
				awgn_errors += noisy[i] * orig[i] < 0;
			for (int i = 0; i < N; ++i)
				quantization_erasures += !noisy[i];
			for (int i = 0; i < N; ++i)
				uncorrected_errors += CODE::get_be_bit(decoded, i) != CODE::get_be_bit(codeword, i);
			if (unique) {
				bool error = false;
				for (int i = 0; i < N; ++i)
					error |= CODE::get_be_bit(decoded, i) != CODE::get_be_bit(codeword, i);
				frame_errors += error;
			} else {
				ambiguity_erasures += N;
				++frame_errors;
			}

			for (int i = 0; i < K; ++i)
				CODE::set_be_bit(message, i, noisy[i] < 0);
			for (int i = K; i < N; ++i)
				CODE::set_be_bit(parity, i-K, noisy[i] < 0);
			int erasures_count = 0;
			for (int i = 0; erasures_count < NR && i < N; ++i)
				if (!noisy[i])
					erasures[erasures_count++] = i;

			bchdec(message, parity, erasures, erasures_count);

			for (int i = 0; i < K; ++i)
				bchdec_errors += CODE::get_be_bit(message, i) != CODE::get_be_bit(codeword, i);
			for (int i = K; i < N; ++i)
				bchdec_errors += CODE::get_be_bit(parity, i-K) != CODE::get_be_bit(codeword, i);
		}

		double frame_error_rate = (double)frame_errors / (double)loops;
		double bit_error_rate = (double)uncorrected_errors / (double)(N * loops);
		if (!uncorrected_errors && !ambiguity_erasures)
			min_SNR = std::min(min_SNR, SNR);

		int MOD_BITS = 1; // BPSK
		double code_rate = (double)K / (double)N;
		double spectral_efficiency = code_rate * MOD_BITS;
		double EbN0 = 10 * std::log10(sigma_signal * sigma_signal / (spectral_efficiency * 2 * sigma_noise * sigma_noise));

		double bchdec_ber = (double)bchdec_errors / (double)(N * loops);

		if (0) {
			std::cerr << SNR << " Es/N0 => AWGN with standard deviation of " << sigma_noise << " and mean " << mean_noise << std::endl;
			std::cerr << EbN0 << " Eb/N0, using spectral efficiency of " << spectral_efficiency << " from " << code_rate << " code rate and " << MOD_BITS << " bits per symbol." << std::endl;
			std::cerr << awgn_errors << " errors caused by AWGN." << std::endl;
			std::cerr << quantization_erasures << " erasures caused by quantization." << std::endl;
			std::cerr << uncorrected_errors << " errors uncorrected." << std::endl;
			std::cerr << ambiguity_erasures << " ambiguity erasures." << std::endl;
			std::cerr << frame_error_rate << " frame error rate." << std::endl;
			std::cerr << bit_error_rate << " bit error rate." << std::endl;
			std::cerr << bchdec_ber << " BCH decoder bit error rate." << std::endl;
		} else {
			std::cout << SNR << " " << frame_error_rate << " " << bit_error_rate << " " << bchdec_ber << " " << EbN0 << std::endl;
		}
	}
	std::cerr << "QEF at: " << min_SNR << " SNR" << std::endl;
	assert(min_SNR < QEF_SNR);
	std::cerr << "Ordered statistics list decoding regression test passed!" << std::endl;
	return 0;
}

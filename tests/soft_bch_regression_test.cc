/*
Regression Test for the short BCH code encoder and soft decoder

Copyright 2020 Ahmet Inan <inan@aicodix.de>
*/

#include <iostream>
#include <random>
#include <cmath>
#include <cassert>
#include <algorithm>
#include <functional>
#include "short_bch_code_decoder.hh"
#include "short_bch_code_encoder.hh"

template<typename TYPE>
int popcnt(TYPE x)
{
	int cnt = 0;
	while (x) {
		++cnt;
		x &= x-1;
	}
	return cnt;
}

#if 1
	// Perfect binary Golay code using x^11+x^9+x^7+x^6+x^5+x+1
	const int LOOPS = 100000;
	const float QEF_SNR = 3.0;
	const int CODE_LEN = 23;
	const int DATA_LEN = 12;
	const int RADIUS_T = 3;
	const int GEN_POLY = 0b101011100011;
#endif

int main()
{
	CODE::ShortBCHCodeEncoder<CODE_LEN, DATA_LEN> encode(GEN_POLY);
	CODE::ShortBCHCodeDecoder<CODE_LEN, DATA_LEN> decode(GEN_POLY, RADIUS_T);

	std::random_device rd;
	std::default_random_engine generator(rd());
	typedef std::uniform_int_distribution<int> uniform;
	typedef std::normal_distribution<float> normal;

	float min_SNR = 20;

	for (float SNR = -10; SNR <= 10; SNR += 0.1) {
		//float mean_signal = 0;
		float sigma_signal = 1;
		float mean_noise = 0;
		float sigma_noise = std::sqrt(sigma_signal * sigma_signal / (2 * std::pow(10, SNR / 10)));

		auto data = std::bind(uniform(0, (1 << DATA_LEN) - 1), generator);
		auto awgn = std::bind(normal(mean_noise, sigma_noise), generator);

		int awgn_errors = 0;
		int quantization_erasures = 0;
		int uncorrected_errors = 0;
		int decoder_errors = 0;
		for (int loop = 0; loop < LOOPS; ++loop) {
			int8_t code[CODE_LEN], orig[CODE_LEN], noisy[CODE_LEN];
			float symb[CODE_LEN];

			int dat = data();
			encode(code, dat);

			for (int i = 0; i < CODE_LEN; ++i)
				orig[i] = code[i];

			for (int i = 0; i < CODE_LEN; ++i)
				symb[i] = code[i];

			for (int i = 0; i < CODE_LEN; ++i)
				symb[i] += awgn();

			// $LLR=log(\frac{p(x=+1|y)}{p(x=-1|y)})$
			// $p(x|\mu,\sigma)=\frac{1}{\sqrt{2\pi}\sigma}}e^{-\frac{(x-\mu)^2}{2\sigma^2}}$
			float DIST = 2; // BPSK
			float fact = DIST / (sigma_noise * sigma_noise);
			for (int i = 0; i < CODE_LEN; ++i)
				code[i] = std::min<float>(std::max<float>(std::nearbyint(fact * symb[i]), -128), 127);

			for (int i = 0; i < CODE_LEN; ++i)
				noisy[i] = code[i];

			int dec = decode(code) >> (CODE_LEN - DATA_LEN);

			for (int i = 0; i < CODE_LEN; ++i)
				awgn_errors += noisy[i] * orig[i] < 0;
			for (int i = 0; i < CODE_LEN; ++i)
				quantization_erasures += !noisy[i];
			uncorrected_errors += popcnt(dat^dec);
			for (int i = 0; i < DATA_LEN; ++i)
				decoder_errors += ((dec^dat)&(1<<i)) && orig[i] * noisy[i] > 0;
		}
		float bit_error_rate = (float)uncorrected_errors / (float)(DATA_LEN * LOOPS);
		if (bit_error_rate < 0.0001)
			min_SNR = std::min(min_SNR, SNR);

		if (0) {
			std::cerr << SNR << " Es/N0 => AWGN with standard deviation of " << sigma_noise << " and mean " << mean_noise << std::endl;
			std::cerr << awgn_errors << " errors caused by AWGN." << std::endl;
			std::cerr << quantization_erasures << " erasures caused by quantization." << std::endl;
			std::cerr << decoder_errors << " errors caused by decoder." << std::endl;
			std::cerr << uncorrected_errors << " errors uncorrected." << std::endl;
			std::cerr << bit_error_rate << " bit error rate." << std::endl;
		} else {
			std::cout << SNR << " " << bit_error_rate << std::endl;
		}
	}

	std::cerr << "QEF at: " << min_SNR << " SNR" << std::endl;
	assert(min_SNR < QEF_SNR);
	std::cerr << "Soft BCH regression test passed!" << std::endl;
	return 0;
}


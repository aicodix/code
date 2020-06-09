/*
Regression Test for the Simplex code Encoder and soft Decoder

Copyright 2020 Ahmet Inan <inan@aicodix.de>
*/

#include <iostream>
#include <random>
#include <cmath>
#include <cassert>
#include <algorithm>
#include <functional>
#include "simplex_encoder.hh"
#include "simplex_decoder.hh"

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

#if 0
	const int LOOPS = 320000;
	const float QEF_SNR = 7.0;
	const int DATA_LEN = 2;
#endif
#if 0
	const int LOOPS = 160000;
	const float QEF_SNR = 4.5;
	const int DATA_LEN = 3;
#endif
#if 1
	const int LOOPS = 80000;
	const float QEF_SNR = 2.0;
	const int DATA_LEN = 4;
#endif
#if 0
	const int LOOPS = 40000;
	const float QEF_SNR = -1.0;
	const int DATA_LEN = 5;
#endif
#if 0
	const int LOOPS = 20000;
	const float QEF_SNR = -3.5;
	const int DATA_LEN = 6;
#endif

int main()
{
	const int CODE_LEN = (1 << DATA_LEN) - 1;

	CODE::SimplexEncoder<DATA_LEN> encode;
	CODE::SimplexDecoder<DATA_LEN> decode;

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

			int dec = decode(code);

			for (int i = 0; i < CODE_LEN; ++i)
				awgn_errors += noisy[i] * orig[i] < 0;
			for (int i = 0; i < CODE_LEN; ++i)
				quantization_erasures += !noisy[i];
			uncorrected_errors += dec < 0 ? DATA_LEN : popcnt(dat^dec);
			for (int i = 0; dec >= 0 && i < DATA_LEN; ++i)
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
	std::cerr << "Simplex code regression test passed!" << std::endl;
	return 0;
}


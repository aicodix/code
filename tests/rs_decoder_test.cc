/*
Test for the Reed Solomon Decoder

Copyright 2018 Ahmet Inan <inan@aicodix.de>
*/

#include <cassert>
#include <random>
#include <iostream>
#include "galois_field.hh"
#include "reed_solomon_decoder.hh"

int main()
{
	std::random_device rd;
	std::default_random_engine generator(rd());
	if (1) {
		// BBC WHP031 RS(15, 11) T=2
		typedef CODE::GaloisField<4, 0b10011, uint8_t> GF;
		typedef CODE::ReedSolomonDecoder<4, 0, GF> RS;
		GF instance;
		RS decode;
		RS::value_type target[RS::N] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 3, 3, 12, 12 };
		RS::value_type code[RS::N];
		for (int i = 0; i < RS::N; ++i)
			code[i] = target[i];
		typedef std::uniform_int_distribution<RS::value_type> distribution;
		auto pos = std::bind(distribution(0, RS::N-1), generator);
		auto val = std::bind(distribution(0, RS::N), generator);
		for (int i = 0; i < RS::NR/2; ++i)
			code[pos()] = val();
		decode(code, code + RS::K);
		for (int i = 0; i < RS::N; ++i)
			assert(code[i] == target[i]);
	}
	if (1) {
		// DVB-T RS(255, 239) T=8
		typedef CODE::GaloisField<8, 0b100011101, uint8_t> GF;
		typedef CODE::ReedSolomonDecoder<16, 0, GF> RS;
		GF instance;
		RS decode;
		RS::value_type target[RS::N];
		for (int i = 0; i < RS::K; ++i)
			target[i] = i + 1;
		RS::value_type parity[RS::NP] = { 1, 126, 147, 48, 155, 224, 3, 157, 29, 226, 40, 114, 61, 30, 244, 75 };
		for (int i = 0; i < RS::NP; ++i)
			target[RS::K+i] = parity[i];
		RS::value_type code[RS::N];
		for (int i = 0; i < RS::N; ++i)
			code[i] = target[i];
		typedef std::uniform_int_distribution<RS::value_type> distribution;
		auto pos = std::bind(distribution(0, RS::N-1), generator);
		auto val = std::bind(distribution(0, RS::N), generator);
		for (int i = 0; i < RS::NR/2; ++i)
			code[pos()] = val();
		decode(code, code + RS::K);
		for (int i = 0; i < RS::N; ++i)
			assert(code[i] == target[i]);
	}
	if (1) {
		// FUN RS(65535, 65471) T=32
		typedef CODE::GaloisField<16, 0b10001000000001011, uint16_t> GF;
		typedef CODE::ReedSolomonDecoder<64, 1, GF> RS;
		GF *instance = new GF();
		RS *decode = new RS();
		RS::value_type *target = new RS::value_type[RS::N];
		for (int i = 0; i < RS::K; ++i)
			target[i] = i + 1;
		RS::value_type parity[RS::NP] = { 25271, 26303, 22052, 31318, 31233, 6076, 40148, 29468, 47507, 32655, 12404, 13265, 23901, 38403, 50967, 50433, 40818, 226, 62296, 23636, 56393, 12952, 11476, 44416, 518, 50014, 10037, 57582, 33421, 42654, 54025, 7157, 4826, 52148, 17167, 23294, 6427, 40953, 11168, 35305, 18209, 1868, 39971, 54928, 27566, 1424, 4846, 25347, 34710, 42190, 56452, 21859, 49805, 28028, 41657, 25756, 22014, 24479, 28758, 17438, 12976, 61743, 46735, 1557 };
		for (int i = 0; i < RS::NP; ++i)
			target[RS::K+i] = parity[i];
		RS::value_type *code = new RS::value_type[RS::N];
		for (int i = 0; i < RS::N; ++i)
			code[i] = target[i];
		typedef std::uniform_int_distribution<RS::value_type> distribution;
		auto pos = std::bind(distribution(0, RS::N-1), generator);
		auto val = std::bind(distribution(0, RS::N), generator);
		for (int i = 0; i < RS::NR/2; ++i)
			code[pos()] = val();
		(*decode)(code, code + RS::K);
		for (int i = 0; i < RS::N; ++i)
			assert(code[i] == target[i]);
		delete[] target;
		delete[] code;
		delete decode;
		delete instance;
	}
	std::cerr << "Reed Solomon Decoder test passed!" << std::endl;
	return 0;
}


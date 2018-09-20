/*
Test for the Reed Solomon Encoder

Copyright 2018 Ahmet Inan <inan@aicodix.de>
*/

#include <cassert>
#include <iostream>
#include "galois_field.hh"
#include "reed_solomon_encoder.hh"

int main()
{
	if (1) {
		// BBC WHP031 RS(15, 11) T=2
		typedef CODE::GaloisField<4, 0b10011, uint8_t> GF;
		typedef CODE::ReedSolomonEncoder<4, 0, GF> RS;
		GF instance;
		RS encode;
		uint8_t code[15] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
		uint8_t target[15] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 3, 3, 12, 12 };
		encode(code);
		for (int i = 0; i < 15; ++i)
			assert(code[i] == target[i]);
	}
	if (1) {
		// DVB-T RS(255, 239) T=8
		typedef CODE::GaloisField<8, 0b100011101, uint8_t> GF;
		typedef CODE::ReedSolomonEncoder<16, 0, GF> RS;
		GF instance;
		RS encode;
		uint8_t code[255], target[255];
		for (int i = 0; i < 239; ++i)
			target[i] = code[i] = i + 1;
		uint8_t parity[16] = { 1, 126, 147, 48, 155, 224, 3, 157, 29, 226, 40, 114, 61, 30, 244, 75 };
		for (int i = 0; i < 16; ++i)
			target[239+i] = parity[i];
		encode(code);
		for (int i = 0; i < 255; ++i)
			assert(code[i] == target[i]);
	}
	if (1) {
		// FUN RS(65535, 65471) T=32
		typedef CODE::GaloisField<16, 0b10001000000001011, uint16_t> GF;
		typedef CODE::ReedSolomonEncoder<64, 1, GF> RS;
		GF *instance = new GF();
		RS *encode = new RS();
		uint16_t *code = new uint16_t[65535];
		uint16_t *target = new uint16_t[65535];
		for (int i = 0; i < 65471; ++i)
			target[i] = code[i] = i + 1;
		uint16_t parity[64] = { 25271, 26303, 22052, 31318, 31233, 6076, 40148, 29468, 47507, 32655, 12404, 13265, 23901, 38403, 50967, 50433, 40818, 226, 62296, 23636, 56393, 12952, 11476, 44416, 518, 50014, 10037, 57582, 33421, 42654, 54025, 7157, 4826, 52148, 17167, 23294, 6427, 40953, 11168, 35305, 18209, 1868, 39971, 54928, 27566, 1424, 4846, 25347, 34710, 42190, 56452, 21859, 49805, 28028, 41657, 25756, 22014, 24479, 28758, 17438, 12976, 61743, 46735, 1557 };
		for (int i = 0; i < 64; ++i)
			target[65471+i] = parity[i];
		(*encode)(code);
		for (int i = 0; i < 65535; ++i)
			assert(code[i] == target[i]);
		delete[] target;
		delete[] code;
		delete encode;
		delete instance;
	}
	std::cerr << "Reed Solomon Encoder test passed!" << std::endl;
	return 0;
}


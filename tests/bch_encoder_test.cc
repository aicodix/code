/*
Test for the Bose Chaudhuri Hocquenghem Encoder

Copyright 2018 Ahmet Inan <inan@aicodix.de>
*/

#include <cassert>
#include <iostream>
#include "bitman.hh"
#include "galois_field.hh"
#include "bose_chaudhuri_hocquenghem_encoder.hh"

int main()
{
	if (1) {
		// NASA INTRO BCH(15, 5) T=3
		typedef CODE::BoseChaudhuriHocquenghemEncoder<15, 5> BCH;
		const int D = (BCH::K + 7) / 8;
		const int P = (BCH::NP + 7) / 8;
		const int L = D + P;
		BCH encode({0b10011, 0b11111, 0b00111});
		uint8_t code[L] = { 0b11001000 };
		uint8_t target[L] = { 0b11001000, 0b00011110, 0b10000000 };
		encode(code, code + D);
		for (int i = 0; i < L; ++i)
			assert(code[i] == target[i]);
	}
	if (1) {
		// DVB-S2 FULL BCH(65535, 65343) T=12
		typedef CODE::BoseChaudhuriHocquenghemEncoder<65535, 65343> BCH;
		const int D = (BCH::K + 7) / 8;
		const int P = (BCH::NP + 7) / 8;
		const int L = D + P;
		BCH *encode = new BCH({
			0b10000000000101101, 0b10000000101110011, 0b10000111110111101,
			0b10101101001010101, 0b10001111100101111, 0b11111011110110101,
			0b11010111101100101, 0b10111001101100111, 0b10000111010100001,
			0b10111010110100111, 0b10011101000101101, 0b10001101011100011});
		uint8_t *target = new uint8_t[L];
		for (int i = 0; i < L; ++i)
			target[i] = 0;
		for (int i = 0, s = 0; i < BCH::K; ++i, s=(s*(s*s*51767+71287)+35149)&0xffffff)
			CODE::set_be_bit(target, i, (s^=s>>7)&1);
		uint8_t *code = new uint8_t[L];
		for (int i = 0; i < L; ++i)
			code[i] = target[i];
		bool parity[BCH::NP] = { 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0 };
		for (int i = 0; i < BCH::NP; ++i)
			CODE::set_be_bit(target + D, i, parity[i]);
		(*encode)(code, code + D);
		for (int i = 0; i < L; ++i)
			assert(code[i] == target[i]);
		delete[] target;
		delete[] code;
		delete encode;
	}
	if (1) {
		// NASA INTRO BCH(15, 5) T=3
		typedef CODE::GaloisField<4, 0b10011, uint8_t> GF;
		typedef CODE::BoseChaudhuriHocquenghemEncoderReference<6, 1, 5, GF> BCH;
		GF instance;
		BCH encode({0b10011, 0b11111, 0b00111});
		BCH::value_type code[BCH::N] = { 1, 1, 0, 0, 1 };
		BCH::value_type target[BCH::N] = { 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0 };
		encode(reinterpret_cast<GF::ValueType *>(code), reinterpret_cast<GF::ValueType *>(code) + BCH::K);
		for (int i = 0; i < BCH::N; ++i)
			assert(code[i] == target[i]);
	}
	if (1) {
		// DVB-S2 FULL BCH(65535, 65343) T=12
		typedef CODE::GaloisField<16, 0b10000000000101101, uint16_t> GF;
		typedef CODE::BoseChaudhuriHocquenghemEncoderReference<24, 1, 65343, GF> BCH;
		GF *instance = new GF();
		BCH *encode = new BCH({
			0b10000000000101101, 0b10000000101110011, 0b10000111110111101,
			0b10101101001010101, 0b10001111100101111, 0b11111011110110101,
			0b11010111101100101, 0b10111001101100111, 0b10000111010100001,
			0b10111010110100111, 0b10011101000101101, 0b10001101011100011});
		BCH::value_type *code = new BCH::value_type[BCH::N];
		BCH::value_type *target = new BCH::value_type[BCH::N];
		for (int i = 0, s = 0; i < BCH::K; ++i, s=(s*(s*s*51767+71287)+35149)&0xffffff)
			target[i] = code[i] = (s^=s>>7)&1;
		BCH::value_type parity[BCH::NP] = { 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0 };
		for (int i = 0; i < BCH::NP; ++i)
			target[BCH::K+i] = parity[i];
		(*encode)(reinterpret_cast<GF::ValueType *>(code), reinterpret_cast<GF::ValueType *>(code) + BCH::K);
		for (int i = 0; i < BCH::N; ++i)
			assert(code[i] == target[i]);
		delete[] target;
		delete[] code;
		delete encode;
		delete instance;
	}
	std::cerr << "Bose Chaudhuri Hocquenghem Encoder test passed!" << std::endl;
	return 0;
}


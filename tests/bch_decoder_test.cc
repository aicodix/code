/*
Test for the Bose Chaudhuri Hocquenghem Decoder

Copyright 2018 Ahmet Inan <inan@aicodix.de>
*/

#include <cassert>
#include <random>
#include <iostream>
#include "galois_field.hh"
#include "bose_chaudhuri_hocquenghem_decoder.hh"

int main()
{
	std::random_device rd;
	std::default_random_engine generator(rd());
	if (1) {
		// NASA INTRO BCH(15, 5) T=3
		typedef CODE::GaloisField<4, 0b10011, uint8_t> GF;
		typedef CODE::BoseChaudhuriHocquenghemDecoder<6, 1, 5, GF> BCH;
		GF instance;
		BCH decode;
		uint8_t target[15] = { 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0 };
		uint8_t code[15];
		for (int i = 0; i < 15; ++i)
			code[i] = target[i];
		std::uniform_int_distribution<uint8_t> distribution(0, 15);
		auto noise = std::bind(distribution, generator);
		for (int i = 0; i < 3; ++i)
			code[noise()] ^= 1;
		decode(reinterpret_cast<GF::ValueType *>(code));
		for (int i = 0; i < 15; ++i)
			assert(code[i] == target[i]);
	}
	if (1) {
		// DVB-S2 FULL BCH(65535, 65343) T=12
		typedef CODE::GaloisField<16, 0b10000000000101101, uint16_t> GF;
		typedef CODE::BoseChaudhuriHocquenghemDecoder<24, 1, 65343, GF> BCH;
		GF *instance = new GF();
		BCH *decode = new BCH();
		uint16_t *target = new uint16_t[65535];
		for (int i = 0, s = 0; i < 65343; ++i, s=(s*(s*s*51767+71287)+35149)&0xffffff)
			target[i] = (s^=s>>7)&1;
		uint16_t parity[192] = { 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0 };
		for (int i = 0; i < 192; ++i)
			target[65343+i] = parity[i];
		uint16_t *code = new uint16_t[65535];
		for (int i = 0; i < 65535; ++i)
			code[i] = target[i];
		std::uniform_int_distribution<uint16_t> distribution(0, 65535);
		auto noise = std::bind(distribution, generator);
		for (int i = 0; i < 12; ++i)
			code[noise()] ^= 1;
		(*decode)(reinterpret_cast<GF::ValueType *>(code));
		for (int i = 0; i < 65535; ++i)
			assert(code[i] == target[i]);
		delete[] target;
		delete[] code;
		delete decode;
		delete instance;
	}
	std::cerr << "Bose Chaudhuri Hocquenghem Decoder test passed!" << std::endl;
	return 0;
}


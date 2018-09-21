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
		const int L = (BCH::N + 7) / 8;
		GF instance;
		BCH decode;
		uint8_t target[L] = { 0b11001000, 0b11110100 };
		uint8_t code[L];
		for (int i = 0; i < L; ++i)
			code[i] = target[i];
		std::uniform_int_distribution<int> distribution(0, BCH::N-1);
		auto noise = std::bind(distribution, generator);
		for (int i = 0; i < 3; ++i) {
			int n = noise();
			code[n / 8] ^= 128 >> (n % 8);
		}
		decode(code);
		for (int i = 0; i < L; ++i)
			assert(code[i] == target[i]);
	}
	if (1) {
		// DVB-S2 FULL BCH(65535, 65343) T=12
		typedef CODE::GaloisField<16, 0b10000000000101101, uint16_t> GF;
		typedef CODE::BoseChaudhuriHocquenghemDecoder<24, 1, 65343, GF> BCH;
		const int L = (BCH::N + 7) / 8;
		GF *instance = new GF();
		BCH *decode = new BCH();
		uint8_t *target = new uint8_t[L];
		for (int i = 0; i < L; ++i)
			target[i] = 0;
		for (int i = 0, s = 0; i < BCH::K; ++i, s=(s*(s*s*51767+71287)+35149)&0xffffff)
			target[i/8] |= ((s^=s>>7)&1) << (7-i%8);
		bool parity[BCH::NP] = { 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0 };
		for (int i = 0; i < BCH::NP; ++i)
			target[(BCH::K+i)/8] |= parity[i] << (7-(BCH::K+i)%8);
		uint8_t *code = new uint8_t[L];
		for (int i = 0; i < L; ++i)
			code[i] = target[i];
		std::uniform_int_distribution<int> distribution(0, BCH::N-1);
		auto noise = std::bind(distribution, generator);
		for (int i = 0; i < 12; ++i) {
			int n = noise();
			code[n / 8] ^= 128 >> (n % 8);
		}
		(*decode)(code);
		for (int i = 0; i < L; ++i)
			assert(code[i] == target[i]);
		delete[] target;
		delete[] code;
		delete decode;
		delete instance;
	}
	std::cerr << "Bose Chaudhuri Hocquenghem Decoder test passed!" << std::endl;
	return 0;
}


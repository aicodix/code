/*
Test for the Bose Chaudhuri Hocquenghem Decoder

Copyright 2018 Ahmet Inan <inan@aicodix.de>
*/

#include <cassert>
#include <random>
#include <iostream>
#include <functional>
#include "bitman.hh"
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
		const int D = (BCH::K + 7) / 8;
		const int P = (BCH::NP + 7) / 8;
		const int L = D + P;
		GF instance;
		BCH decode;
		uint8_t target[L] = { 0b11001000, 0b00011110, 0b10000000 };
		uint8_t code[L];
		for (int i = 0; i < L; ++i)
			code[i] = target[i];
		typedef std::uniform_int_distribution<BCH::value_type> distribution;
		auto noise = std::bind(distribution(0, BCH::N-1), generator);
		for (int i = 0; i < BCH::NR/2; ++i) {
			int n = noise();
			if (n < BCH::K)
				CODE::xor_be_bit(code, n, 1);
			else
				CODE::xor_be_bit(code+D, n-BCH::K, 1);
		}
		decode(code, code + D);
		for (int i = 0; i < L; ++i)
			assert(code[i] == target[i]);
	}
	if (1) {
		// DVB-S2 FULL BCH(65535, 65343) T=12
		typedef CODE::GaloisField<16, 0b10000000000101101, uint16_t> GF;
		typedef CODE::BoseChaudhuriHocquenghemDecoder<24, 1, 65343, GF> BCH;
		const int D = (BCH::K + 7) / 8;
		const int P = (BCH::NP + 7) / 8;
		const int L = D + P;
		GF *instance = new GF();
		BCH *decode = new BCH();
		uint8_t *target = new uint8_t[L];
		for (int i = 0; i < L; ++i)
			target[i] = 0;
		for (int i = 0, s = 0; i < BCH::K; ++i, s=(s*(s*s*51767+71287)+35149)&0xffffff)
			CODE::set_be_bit(target, i, (s^=s>>7)&1);
		bool parity[BCH::NP] = { 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0 };
		for (int i = 0; i < BCH::NP; ++i)
			CODE::set_be_bit(target + D, i, parity[i]);
		uint8_t *code = new uint8_t[L];
		for (int i = 0; i < L; ++i)
			code[i] = target[i];
		typedef std::uniform_int_distribution<BCH::value_type> distribution;
		auto noise = std::bind(distribution(0, BCH::N-1), generator);
		for (int i = 0; i < BCH::NR/2; ++i) {
			int n = noise();
			if (n < BCH::K)
				CODE::xor_be_bit(code, n, 1);
			else
				CODE::xor_be_bit(code+D, n-BCH::K, 1);
		}
		(*decode)(code, code + D);
		for (int i = 0; i < L; ++i)
			assert(code[i] == target[i]);
		delete[] target;
		delete[] code;
		delete decode;
		delete instance;
	}
	if (1) {
		// NASA INTRO BCH(15, 5) T=3
		typedef CODE::GaloisField<4, 0b10011, uint8_t> GF;
		typedef CODE::BoseChaudhuriHocquenghemDecoderReference<6, 1, 5, GF> BCH;
		GF instance;
		BCH decode;
		BCH::value_type target[BCH::N] = { 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0 };
		BCH::value_type code[BCH::N];
		for (int i = 0; i < BCH::N; ++i)
			code[i] = target[i];
		typedef std::uniform_int_distribution<BCH::value_type> distribution;
		auto noise = std::bind(distribution(0, BCH::N-1), generator);
		for (int i = 0; i < BCH::NR/2; ++i)
			code[noise()] ^= 1;
		decode(reinterpret_cast<GF::ValueType *>(code), reinterpret_cast<GF::ValueType *>(code) + BCH::K);
		for (int i = 0; i < BCH::N; ++i)
			assert(code[i] == target[i]);
	}
	if (1) {
		// DVB-S2 FULL BCH(65535, 65343) T=12
		typedef CODE::GaloisField<16, 0b10000000000101101, uint16_t> GF;
		typedef CODE::BoseChaudhuriHocquenghemDecoderReference<24, 1, 65343, GF> BCH;
		GF *instance = new GF();
		BCH *decode = new BCH();
		BCH::value_type *target = new BCH::value_type[BCH::N];
		for (int i = 0, s = 0; i < BCH::K; ++i, s=(s*(s*s*51767+71287)+35149)&0xffffff)
			target[i] = (s^=s>>7)&1;
		BCH::value_type parity[BCH::NP] = { 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0 };
		for (int i = 0; i < BCH::NP; ++i)
			target[BCH::K+i] = parity[i];
		BCH::value_type *code = new BCH::value_type[BCH::N];
		for (int i = 0; i < BCH::N; ++i)
			code[i] = target[i];
		typedef std::uniform_int_distribution<BCH::value_type> distribution;
		auto noise = std::bind(distribution(0, BCH::N-1), generator);
		for (int i = 0; i < BCH::NR/2; ++i)
			code[noise()] ^= 1;
		(*decode)(reinterpret_cast<GF::ValueType *>(code), reinterpret_cast<GF::ValueType *>(code) + BCH::K);
		for (int i = 0; i < BCH::N; ++i)
			assert(code[i] == target[i]);
		delete[] target;
		delete[] code;
		delete decode;
		delete instance;
	}
	std::cerr << "Bose Chaudhuri Hocquenghem Decoder test passed!" << std::endl;
	return 0;
}


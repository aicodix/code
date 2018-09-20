/*
Test for the Bose Chaudhuri Hocquenghem Encoder

Copyright 2018 Ahmet Inan <inan@aicodix.de>
*/

#include <cassert>
#include <iostream>
#include "galois_field.hh"
#include "bose_chaudhuri_hocquenghem_encoder.hh"

int main()
{
	if (1) {
		// NASA INTRO BCH(15, 5) T=3
		typedef CODE::GaloisField<4, 0b10011, uint8_t> GF;
		typedef CODE::BoseChaudhuriHocquenghemEncoder<6, 1, 5, GF> BCH;
		GF instance;
		BCH encode({0b10011, 0b11111, 0b00111});
		uint8_t code[15] = { 1, 1, 0, 0, 1 };
		uint8_t target[15] = { 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0 };
		encode(code);
		for (int i = 0; i < 15; ++i)
			assert(code[i] == target[i]);
	}
	if (1) {
		// DVB-S2 FULL BCH(65535, 65343) T=12
		typedef CODE::GaloisField<16, 0b10000000000101101, uint16_t> GF;
		typedef CODE::BoseChaudhuriHocquenghemEncoder<24, 1, 65343, GF> BCH;
		GF *instance = new GF();
		BCH *encode = new BCH({0b10000000000101101, 0b10000000101110011, 0b10000111110111101, 0b10101101001010101, 0b10001111100101111, 0b11111011110110101, 0b11010111101100101, 0b10111001101100111, 0b10000111010100001, 0b10111010110100111, 0b10011101000101101, 0b10001101011100011});
		uint16_t *code = new uint16_t[65535];
		uint16_t *target = new uint16_t[65535];
		for (int i = 0, s = 0; i < 65343; ++i, s=(s*(s*s*51767+71287)+35149)&0xffffff)
			target[i] = code[i] = (s^=s>>7)&1;
		uint16_t parity[192] = { 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0 };
		for (int i = 0; i < 192; ++i)
			target[65343+i] = parity[i];
		(*encode)(code);
		for (int i = 0; i < 65535; ++i)
			assert(code[i] == target[i]);
		delete[] target;
		delete[] code;
		delete encode;
		delete instance;
	}
	std::cerr << "Bose Chaudhuri Hocquenghem Encoder test passed!" << std::endl;
	return 0;
}


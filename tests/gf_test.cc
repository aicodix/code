/*
Test for the Galois field arithmetic

Copyright 2018 Ahmet Inan <inan@aicodix.de>
*/

#include <cstdint>
#include <cassert>
#include <iostream>
#include "galois_field.hh"

template <typename GF, typename GFR>
void exhaustive_test()
{
	for (int j = 0; j < GF::Q; ++j) {
		typename GF::ValueType a(j);
		for (int i = 0; i < GF::Q; ++i) {
			typename GF::ValueType b(i);
			assert((a * b).v == (GFR(j) * GFR(i)).v);
		}
	}
	for (int i = 1; i < GF::Q; ++i) {
		typename GF::ValueType a(i);
		assert(rcp(a).v == rcp(GFR(i)).v);
	}
}

int main()
{
	if (1) {
		// BBC WHP031 RS(15, 11) T=2
		typedef CODE::GaloisField<4, 0b10011, uint8_t> GF;
		typedef CODE::GaloisFieldReference<4, 0b10011, uint8_t> GFR;
		GF instance;
		GF::ValueType a(3), b(7), c(15), d(6);
		assert(a * b + c == d);
		exhaustive_test<GF, GFR>();
	}
	if (1) {
		// DVB-T RS(255, 239) T=8
		typedef CODE::GaloisField<8, 0b100011101, uint8_t> GF;
		typedef CODE::GaloisFieldReference<8, 0b100011101, uint8_t> GFR;
		GF instance;
		GF::ValueType a(154), b(83), c(144), d(63);
		assert(fma(a, b, c) == d);
		exhaustive_test<GF, GFR>();
	}
	if (1) {
		// FUN RS(65535, 65471) T=32
		typedef CODE::GaloisField<16, 0b10001000000001011, uint16_t> GF;
		typedef CODE::GaloisFieldReference<16, 0b10001000000001011, uint16_t> GFR;
		GF *instance = new GF();
		GF::ValueType a(42145), b(13346), c(40958), d(35941);
		assert(a / b + c == d);
		exhaustive_test<GF, GFR>();
		delete instance;
	}
	if (1) {
		// DVB-S2 FULL BCH(65535, 65343) T=12
		typedef CODE::GaloisField<16, 0b10000000000101101, uint16_t> GF;
		typedef CODE::GaloisFieldReference<16, 0b10000000000101101, uint16_t> GFR;
		GF *instance = new GF();
		GF::ValueType a(38532), b(7932), c(34283), d(22281);
		assert(a / (rcp(b) + c) == d);
		exhaustive_test<GF, GFR>();
		delete instance;
	}
	std::cerr << "Galois field arithmetic test passed!" << std::endl;
	return 0;
}


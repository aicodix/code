/*
Regression Test for the Mersenne 2^31-1 Prime Field Packer

Copyright 2026 Ahmet Inan <inan@aicodix.de>
*/

#include <cstdlib>
#include <cassert>
#include <random>
#include <iostream>
#include <functional>
#include "prime_field.hh"
#include "mersenne_packing.hh"

void cme_test(int trials)
{
	typedef CODE::PrimeField<uint32_t, 0x7FFFFFFF> M31;
	typedef CODE::MersennePacking MP;
	std::random_device rd;
	std::default_random_engine generator(rd());
	typedef std::uniform_int_distribution<int> distribution;
	const int bytes_max = 1024;
	const int count_max = (bytes_max * 8 + 30) / 31;
	auto rnd_cnt = std::bind(distribution(0, count_max), generator);
	auto rnd_len = std::bind(distribution(0, bytes_max), generator);
	auto rnd_m31 = std::bind(distribution(0, M31::P-1), generator);
	auto rnd_dat = std::bind(distribution(0, 255), generator);
	auto remap = new CODE::MersenneRemapping<bytes_max>();
	M31 *tmp0 = new M31[count_max+1];
	M31 *tmp1 = new M31[count_max+1];
	uint8_t *tmp2 = new uint8_t[bytes_max];
	uint8_t *tmp3 = new uint8_t[bytes_max];
	for (int j = 0; j < trials; ++j) {
		int count = rnd_cnt();
		int bytes = (count * 31 + 7) / 8;
		// M31 -> byte -> M31
		for (int i = 0; i < count; ++i)
			tmp0[i] = M31(rnd_m31());
		MP::unpack(tmp2, tmp0, count, bytes);
		MP::pack(tmp1, tmp2, count, bytes);
		for (int i = 0; i < count; ++i)
			assert(tmp0[i] == tmp1[i]);
	}
	for (int j = 0; j < trials; ++j) {
		int bytes = rnd_len();
		int count = (bytes * 8 + 30) / 31;
		// byte -> M31 -> byte
		for (int i = 0; i < bytes; ++i)
			tmp2[i] = rnd_dat();
		MP::pack(tmp0, tmp2, count, bytes);
		MP::unpack(tmp3, tmp0, count, bytes);
		for (int i = 0; i < bytes; ++i)
			assert(tmp2[i] == tmp3[i]);
	}
	for (int j = 0; j < trials; ++j) {
		int bytes = rnd_len();
		int count = (bytes * 8 + 30) / 31 + 1;
		// byte -> M31 -> byte
		for (int i = 0; i < bytes; ++i)
			tmp2[i] = i < bytes / 2 ? rnd_dat() : 255;
		remap->encode(tmp0, tmp2, bytes);
		for (int i = 0; i < count; ++i)
			assert(tmp0[i].v != M31::P);
		MP::decode(tmp3, tmp0, bytes);
		for (int i = 0; i < bytes; ++i)
			assert(tmp2[i] == tmp3[i]);
	}
	delete remap;
	delete[] tmp0;
	delete[] tmp1;
	delete[] tmp2;
	delete[] tmp3;
}

int main()
{
	cme_test(1000000);
	std::cerr << "Mersenne 2^31-1 prime field packing regression test passed!" << std::endl;
	return 0;
}


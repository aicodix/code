/*
Regression Test for the Mersenne 2^31-1 Prime Field Packer

Copyright 2026 Ahmet Inan <inan@aicodix.de>
*/

#include <cstdlib>
#include <cassert>
#include <random>
#include <iostream>
#include <functional>
#include "mersenne_31.hh"
#include "mersenne_packing.hh"

void cme_test(int trials)
{
	typedef CODE::Mersenne31 M31;
	CODE::MersennePacking mp;
	std::random_device rd;
	std::default_random_engine generator(rd());
	typedef std::uniform_int_distribution<int> distribution;
	const int bytes_max = 1024;
	const int count_max = (bytes_max * 8 + 30) / 31;
	auto rnd_cnt = std::bind(distribution(0, count_max), generator);
	auto rnd_len = std::bind(distribution(0, bytes_max), generator);
	auto rnd_m31 = std::bind(distribution(0, M31::P-1), generator);
	auto rnd_dat = std::bind(distribution(0, 255), generator);
	uint32_t *tmp0 = new uint32_t[count_max];
	uint32_t *tmp1 = new uint32_t[count_max];
	uint8_t *tmp2 = new uint8_t[bytes_max];
	uint8_t *tmp3 = new uint8_t[bytes_max];
	for (int j = 0; j < trials; ++j) {
		int count = rnd_cnt();
		int bytes = (count * 31 + 7) / 8;
		// M31 -> byte -> M31
		for (int i = 0; i < count; ++i)
			tmp0[i] = rnd_m31();
		mp.unpack(tmp2, tmp0, count, bytes);
		mp.pack(tmp1, tmp2, count, bytes);
		for (int i = 0; i < count; ++i)
			assert(tmp0[i] == tmp1[i]);
	}
	for (int j = 0; j < trials; ++j) {
		int bytes = rnd_len();
		int count = (bytes * 8 + 30) / 31;
		// byte -> M31 -> byte
		for (int i = 0; i < bytes; ++i)
			tmp2[i] = rnd_dat();
		mp.pack(tmp0, tmp2, count, bytes);
		mp.unpack(tmp3, tmp0, count, bytes);
		for (int i = 0; i < bytes; ++i)
			assert(tmp2[i] == tmp3[i]);
	}
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


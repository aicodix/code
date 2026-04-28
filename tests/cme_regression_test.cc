/*
Regression Test for the Cauchy Mersenne 2^31-1 Prime Field Encoder and Decoder

Copyright 2026 Ahmet Inan <inan@aicodix.de>
*/

#include <cstdlib>
#include <cassert>
#include <chrono>
#include <random>
#include <iostream>
#include <functional>
#include "mersenne_31.hh"
#include "cauchy_mersenne_erasure_coding.hh"

void cme_test(int trials)
{
	typedef CODE::Mersenne31 M31;
	int value_bytes = sizeof(M31);
	//int value_bits = value_bytes * 8;
	const int MAX_LEN = 1024;
	CODE::CauchyMersenneErasureCoding<MAX_LEN> cme;
	std::random_device rd;
	std::default_random_engine generator(rd());
	typedef std::uniform_int_distribution<int> distribution;
	auto rnd_cnt = std::bind(distribution(1, 256), generator);
	auto rnd_len = std::bind(distribution(1, MAX_LEN), generator);
	auto rnd_dat = std::bind(distribution(0, M31::P-1), generator);
	//auto rnd_dat = std::bind(distribution(0, (1 << value_bits) - 1), generator);
	while (--trials) {
		int block_count = rnd_cnt();
		int idents_total = 1000000; // M31::P / 2 - block_count;
		int block_values = rnd_len();
		int block_bytes = block_values * value_bytes;
		int data_values = block_count * block_values;
		int data_bytes = data_values * value_bytes;
		M31 *orig = new M31[data_values];
		M31 *data = new M31[data_values];
		M31 *blocks = new M31[data_values];
		int *idents = new int[idents_total];
		for (int i = 0; i < data_values; ++i)
			orig[i] = M31(rnd_dat());
		for (int i = 0; i < idents_total; ++i)
			idents[i] = block_count + i;
		for (int i = 0; i < block_count; i++) {
			std::uniform_int_distribution<int> hat(i, idents_total - 1);
			std::swap(idents[i], idents[hat(generator)]);
		}
		auto enc_start = std::chrono::system_clock::now();
		for (int i = 0; i < block_count; ++i)
			cme.encode(orig, blocks + block_values * i, idents[i], block_values, block_count);
		auto enc_end = std::chrono::system_clock::now();
		auto enc_usec = std::chrono::duration_cast<std::chrono::microseconds>(enc_end - enc_start);
		double enc_mbs = double(data_bytes) / enc_usec.count();
		auto dec_start = std::chrono::system_clock::now();
		for (int i = 0; i < block_count; ++i)
			cme.decode(data + block_values * i, blocks, idents, i, block_values, block_count);
		auto dec_end = std::chrono::system_clock::now();
		auto dec_usec = std::chrono::duration_cast<std::chrono::microseconds>(dec_end - dec_start);
		double dec_mbs = double(data_bytes) / dec_usec.count();
		std::cout << "block count = " << block_count << ", block size = " << block_bytes << " bytes, encoding speed = " << enc_mbs << " megabyte per second, decoding speed = " << dec_mbs << " megabyte per second" << std::endl;
		for (int i = 0; i < data_values; ++i)
			assert(data[i] == orig[i]);
		delete[] idents;
		delete[] blocks;
		delete[] orig;
		delete[] data;
	}
}

int main()
{
	cme_test(100);
	std::cerr << "Cauchy Mersenne 2^31-1 prime field regression test passed!" << std::endl;
	return 0;
}


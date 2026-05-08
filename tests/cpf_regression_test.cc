/*
Regression Test for the Cauchy Prime Field Encoder and Decoder

Copyright 2026 Ahmet Inan <inan@aicodix.de>
*/

#include <cstdlib>
#include <cassert>
#include <chrono>
#include <random>
#include <iostream>
#include <functional>
#include "prime_field.hh"
#include "cauchy_prime_field_erasure_coding.hh"

template <typename PF, int BITS>
void cpf_test(int trials)
{
	const int MAX_LEN = std::min<int>(PF::P - 2, 1024);
	CODE::CauchyPrimeFieldErasureCoding<PF> cpf;
	std::random_device rd;
	std::default_random_engine generator(rd());
	typedef std::uniform_int_distribution<int> distribution;
	auto rnd_cnt = std::bind(distribution(1, std::min<int>(PF::P / 4, 256)), generator);
	auto rnd_len = std::bind(distribution(1, MAX_LEN), generator);
	auto rnd_dat = std::bind(distribution(0, PF::P-1), generator);
	while (--trials) {
		int block_count = rnd_cnt();
		int idents_total = std::min<int>(1000000, PF::P / 2 - block_count);
		int block_values = rnd_len();
		int block_bits = block_values * BITS;
		int block_bytes = block_bits / 8;
		int data_values = block_count * block_values;
		int data_bits = data_values * BITS;
		int data_bytes = data_bits / 8;
		PF *orig = new PF[data_values];
		PF *data = new PF[data_values];
		PF *blocks = new PF[data_values];
		int *idents = new int[idents_total];
		for (int i = 0; i < data_values; ++i)
			orig[i] = PF(rnd_dat());
		for (int i = 0; i < idents_total; ++i)
			idents[i] = block_count + i;
		for (int i = 0; i < block_count; i++) {
			std::uniform_int_distribution<int> hat(i, idents_total - 1);
			std::swap(idents[i], idents[hat(generator)]);
		}
		auto enc_start = std::chrono::system_clock::now();
		for (int i = 0; i < block_count; ++i)
			cpf.encode(orig, blocks + block_values * i, idents[i], block_values, block_count);
		auto enc_end = std::chrono::system_clock::now();
		auto enc_usec = std::chrono::duration_cast<std::chrono::microseconds>(enc_end - enc_start);
		double enc_mbs = double(data_bytes) / enc_usec.count();
		auto dec_start = std::chrono::system_clock::now();
		for (int i = 0; i < block_count; ++i)
			cpf.decode(data + block_values * i, blocks, idents, i, block_values, block_count);
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
	cpf_test<CODE::PrimeField<uint16_t, 257>, 8>(100);
	cpf_test<CODE::PrimeField<uint32_t, 65537>, 16>(100);
	cpf_test<CODE::PrimeField<uint32_t, 0x7FFFFFFF>, 31>(100);
	std::cerr << "Cauchy prime field erasure coding regression test passed!" << std::endl;
	return 0;
}


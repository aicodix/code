/*
Regression Test for the Cauchy Prime Field Encoder and Decoder

Copyright 2024 Ahmet Inan <inan@aicodix.de>
*/

#include <cstdlib>
#include <cassert>
#include <chrono>
#include <random>
#include <iostream>
#include <functional>
#include "prime_field.hh"
#include "cauchy_prime_field_erasure_coding.hh"

template <typename PF, typename IO>
void cpf_test(int trials)
{
	int value_bytes = sizeof(IO);
	int value_bits = value_bytes * 8;
	const int MAX_LEN = std::min<int>(PF::P - 2, 1024);
	CODE::CauchyPrimeFieldErasureCoding<PF, IO, MAX_LEN> crs;
	std::random_device rd;
	std::default_random_engine generator(rd());
	typedef std::uniform_int_distribution<int> distribution;
	auto rnd_cnt = std::bind(distribution(1, std::min<int>(PF::P / 4, 256)), generator);
	auto rnd_len = std::bind(distribution(1, MAX_LEN), generator);
	auto rnd_dat = std::bind(distribution(0, (1 << value_bits) - 1), generator);
	while (--trials) {
		int block_count = rnd_cnt();
		int identifiers_total = PF::P / 2 - block_count;
		int block_values = rnd_len();
		int block_bytes = block_values * value_bytes;
		int data_values = block_count * block_values;
		int data_bytes = data_values * value_bytes;
		IO *orig = new IO[data_values];
		IO *data = new IO[data_values];
		IO *blocks = new IO[data_values+block_count];
		for (int i = 0; i < data_values; ++i)
			orig[i] = rnd_dat();
		auto identifiers = new int[identifiers_total];
		for (int i = 0; i < identifiers_total; ++i)
			identifiers[i] = block_count + i;
		for (int i = 0; i < block_count; i++) {
			std::uniform_int_distribution<int> hat(i, identifiers_total - 1);
			std::swap(identifiers[i], identifiers[hat(generator)]);
		}
		auto enc_start = std::chrono::system_clock::now();
		for (int i = 0; i < block_count; ++i)
			crs.encode(orig, blocks + (block_values+1) * i, identifiers[i], block_values, block_count);
		auto enc_end = std::chrono::system_clock::now();
		auto enc_usec = std::chrono::duration_cast<std::chrono::microseconds>(enc_end - enc_start);
		double enc_mbs = double(data_bytes) / enc_usec.count();
		auto dec_start = std::chrono::system_clock::now();
		for (int i = 0; i < block_count; ++i)
			crs.decode(data + block_values * i, blocks, identifiers, i, block_values, block_count);
		auto dec_end = std::chrono::system_clock::now();
		auto dec_usec = std::chrono::duration_cast<std::chrono::microseconds>(dec_end - dec_start);
		double dec_mbs = double(data_bytes) / dec_usec.count();
		std::cout << "block count = " << block_count << ", block size = " << block_bytes << " bytes, encoding speed = " << enc_mbs << " megabyte per second, decoding speed = " << dec_mbs << " megabyte per second" << std::endl;
		for (int i = 0; i < data_values; ++i)
			assert(data[i] == orig[i]);
		delete[] identifiers;
		delete[] blocks;
		delete[] orig;
		delete[] data;
	}
}

int main()
{
	if (1) {
		cpf_test<CODE::PrimeField<uint32_t, 257>, uint8_t>(200);
	}
	if (1) {
		cpf_test<CODE::PrimeField<uint64_t, 65537>, uint16_t>(100);
	}
	std::cerr << "Cauchy prime field regression test passed!" << std::endl;
	return 0;
}


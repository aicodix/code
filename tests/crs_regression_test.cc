/*
Regression Test for the Cauchy Reed Solomon Encoder and Decoder

Copyright 2023 Ahmet Inan <inan@aicodix.de>
*/

#include <cstdlib>
#include <cassert>
#include <chrono>
#include <random>
#include <iostream>
#include <functional>
#include "galois_field.hh"
#include "cauchy_reed_solomon_erasure_coding.hh"

template <typename GF>
void crs_test(int trials)
{
#ifdef __AVX2__
	int SIMD = 32;
#else
	int SIMD = 16;
#endif
	CODE::CauchyReedSolomonErasureCoding<GF> crs;
	std::random_device rd;
	std::default_random_engine generator(rd());
	typedef std::uniform_int_distribution<int> distribution;
	auto rnd_cnt = std::bind(distribution(1, std::min(GF::Q / 2, 256)), generator);
	auto rnd_len = std::bind(distribution(1, 1 << 10), generator);
	auto rnd_dat = std::bind(distribution(0, 255), generator);
	while (--trials) {
		int block_count = rnd_cnt();
		int identifiers_total = GF::Q - block_count;
		int block_bytes = rnd_len() * sizeof(typename GF::value_type) * SIMD;
		int data_bytes = block_count * block_bytes;
		uint8_t *orig = reinterpret_cast<uint8_t *>(std::aligned_alloc(SIMD, data_bytes));
		uint8_t *data = reinterpret_cast<uint8_t *>(std::aligned_alloc(SIMD, data_bytes));
		uint8_t *blocks = reinterpret_cast<uint8_t *>(std::aligned_alloc(SIMD, data_bytes));
		for (int i = 0; i < data_bytes; ++i)
			orig[i] = rnd_dat();
		auto identifiers = new typename GF::value_type[identifiers_total];
		for (int i = 0; i < identifiers_total; ++i)
			identifiers[i] = block_count + i;
		for (int i = 0; i < block_count; i++) {
			std::uniform_int_distribution<int> hat(i, identifiers_total - 1);
			std::swap(identifiers[i], identifiers[hat(generator)]);
		}
		auto enc_start = std::chrono::system_clock::now();
		for (int i = 0; i < block_count; ++i)
			crs.encode(orig, blocks + block_bytes * i, identifiers[i], block_bytes, block_count);
		auto enc_end = std::chrono::system_clock::now();
		auto enc_usec = std::chrono::duration_cast<std::chrono::microseconds>(enc_end - enc_start);
		double enc_mbs = double(data_bytes) / enc_usec.count();
		auto dec_start = std::chrono::system_clock::now();
		for (int i = 0; i < block_count; ++i)
			crs.decode(data + block_bytes * i, blocks, identifiers, i, block_bytes, block_count);
		auto dec_end = std::chrono::system_clock::now();
		auto dec_usec = std::chrono::duration_cast<std::chrono::microseconds>(dec_end - dec_start);
		double dec_mbs = double(data_bytes) / dec_usec.count();
		std::cout << "block count = " << block_count << ", block size = " << block_bytes << " bytes, encoding speed = " << enc_mbs << " megabyte per second, decoding speed = " << dec_mbs << " megabyte per second" << std::endl;
		for (int i = 0; i < data_bytes; ++i)
			assert(data[i] == orig[i]);
		delete[] identifiers;
		std::free(blocks);
		std::free(orig);
		std::free(data);
	}
}

int main()
{
	if (1) {
		typedef CODE::GaloisField<8, 0b100011101, uint8_t> GF;
		GF instance;
		crs_test<GF>(200);
	}
	if (1) {
		typedef CODE::GaloisField<16, 0b10001000000001011, uint16_t> GF;
		GF *instance = new GF();
		crs_test<GF>(100);
		delete instance;
	}
	std::cerr << "Cauchy Reed Solomon regression test passed!" << std::endl;
	return 0;
}


/*
Regression Test for the Reed Solomon Encoder and Decoder

Copyright 2018 Ahmet Inan <inan@aicodix.de>
*/

#include <cassert>
#include <random>
#include <iostream>
#include "galois_field.hh"
#include "reed_solomon_decoder.hh"
#include "reed_solomon_encoder.hh"

template <typename GF, typename ENC, typename DEC>
void rs_test(int trials)
{
	std::random_device rd;
	std::default_random_engine generator(rd());
	GF instance;
	DEC decode;
	ENC encode;
	auto rnd_cnt = std::bind(std::uniform_int_distribution<typename ENC::value_type>(0, ENC::NP), generator);
	auto rnd_len = std::bind(std::uniform_int_distribution<typename ENC::value_type>(1, ENC::K), generator);
	auto rnd_val = std::bind(std::uniform_int_distribution<typename ENC::value_type>(0, ENC::N), generator);
	while (--trials) {
		int data_len = rnd_len();
		auto rnd_pos = std::bind(std::uniform_int_distribution<typename ENC::value_type>(0, data_len + ENC::NP - 1), generator);
		typename ENC::value_type data[data_len], orig_data[data_len];
		for (int i = 0; i < data_len; ++i)
			data[i] = orig_data[i] = rnd_val();
		typename ENC::value_type parity[ENC::NP];
		encode(data, parity, data_len);
		for (int i = 0; i < data_len; ++i)
			assert(data[i] == orig_data[i]);
		typename ENC::value_type orig_parity[ENC::NP];
		for (int i = 0; i < ENC::NP; ++i)
			orig_parity[i] = parity[i];
		int error_count = rnd_cnt() / 2;
		typename ENC::value_type errors[error_count];
		for (int i = 0; i < error_count; ++i) {
			int pos = rnd_pos();
			for (int j = 0; j < i; ++j) {
				if (errors[j] == pos) {
					pos = rnd_pos();
					j = -1;
				}
			}
			errors[i] = pos;
			if (pos < data_len)
				data[pos] = rnd_val();
			else
				parity[pos-data_len] = rnd_val();
		}
		int erasures_count = ENC::NP - 2 * error_count;
		typename ENC::value_type erasures[erasures_count];
		for (int i = 0; i < erasures_count; ++i) {
			int pos = rnd_pos();
			for (int j = 0; j < i; ++j) {
				if (erasures[j] == pos) {
					pos = rnd_pos();
					j = -1;
				}
			}
			erasures[i] = pos;
			if (pos < data_len)
				data[pos] = rnd_val();
			else
				parity[pos-data_len] = rnd_val();
		}
		int ret = decode(data, parity, erasures, erasures_count, data_len);
		assert(ret >= 0);
		for (int i = 0; i < data_len; ++i)
			assert(data[i] == orig_data[i]);
		for (int i = 0; i < ENC::NP; ++i)
			assert(parity[i] == orig_parity[i]);
	}
}

int main()
{
	if (1) {
		// BBC WHP031 RS(15, 11) T=2
		typedef CODE::GaloisField<4, 0b10011, uint8_t> GF;
		typedef CODE::ReedSolomonDecoder<4, 0, GF> Decoder;
		typedef CODE::ReedSolomonEncoder<4, 0, GF> Encoder;
		rs_test<GF, Encoder, Decoder>(1000000);
	}
	if (1) {
		// DVB-T RS(255, 239) T=8
		typedef CODE::GaloisField<8, 0b100011101, uint8_t> GF;
		typedef CODE::ReedSolomonDecoder<16, 0, GF> Decoder;
		typedef CODE::ReedSolomonEncoder<16, 0, GF> Encoder;
		rs_test<GF, Encoder, Decoder>(100000);
	}
	if (1) {
		// FUN RS(65535, 65471) T=32
		typedef CODE::GaloisField<16, 0b10001000000001011, uint16_t> GF;
		typedef CODE::ReedSolomonDecoder<64, 1, GF> Decoder;
		typedef CODE::ReedSolomonEncoder<64, 1, GF> Encoder;
		rs_test<GF, Encoder, Decoder>(100);
	}
	std::cerr << "Reed Solomon regression test passed!" << std::endl;
	return 0;
}


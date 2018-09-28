/*
Regression Test for the Bose Chaudhuri Hocquenghem Encoder and Decoder

Copyright 2018 Ahmet Inan <inan@aicodix.de>
*/

#include <cassert>
#include <random>
#include <iostream>
#include "galois_field.hh"
#include "bose_chaudhuri_hocquenghem_encoder.hh"
#include "bose_chaudhuri_hocquenghem_decoder.hh"

template <typename ENC, typename DEC>
void bch_reference_test(ENC *encode, DEC *decode, int trials)
{
	std::random_device rd;
	std::default_random_engine generator(rd());
	typedef std::uniform_int_distribution<typename ENC::value_type> distribution;
	auto rnd_cnt = std::bind(distribution(0, ENC::NR), generator);
	auto rnd_len = std::bind(distribution(1, ENC::K), generator);
	auto rnd_val = std::bind(distribution(0, 1), generator);
	while (--trials) {
		int data_len = rnd_len();
		auto rnd_pos = std::bind(distribution(0, data_len + ENC::NP - 1), generator);
		typename ENC::value_type data[data_len], orig_data[data_len];
		for (int i = 0; i < data_len; ++i)
			data[i] = orig_data[i] = rnd_val();
		typename ENC::value_type parity[ENC::NP];
		(*encode)(reinterpret_cast<typename ENC::ValueType *>(data), reinterpret_cast<typename ENC::ValueType *>(parity), data_len);
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
				data[pos] ^= 1;
			else
				parity[pos-data_len] ^= 1;
		}
		int erasures_count = ENC::NR - 2 * error_count;
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
				data[pos] ^= 1;
			else
				parity[pos-data_len] ^= 1;
		}
		int ret = (*decode)(reinterpret_cast<typename ENC::ValueType *>(data), reinterpret_cast<typename ENC::ValueType *>(parity), reinterpret_cast<typename ENC::IndexType *>(erasures), erasures_count, data_len);
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
		// NASA INTRO BCH(15, 5) T=3
		typedef CODE::GaloisField<4, 0b10011, uint8_t> GF;
		GF instance;
		CODE::BoseChaudhuriHocquenghemEncoderReference<6, 1, 5, GF> encoder({0b10011, 0b11111, 0b00111});
		CODE::BoseChaudhuriHocquenghemDecoderReference<6, 1, 5, GF> decoder;
		bch_reference_test(&encoder, &decoder, 1000000);
	}
	if (1) {
		// DVB-S2 FULL BCH(65535, 65343) T=12
		typedef CODE::GaloisField<16, 0b10000000000101101, uint16_t> GF;
		GF instance;
		CODE::BoseChaudhuriHocquenghemEncoderReference<24, 1, 65343, GF> encoder({0b10000000000101101, 0b10000000101110011, 0b10000111110111101, 0b10101101001010101, 0b10001111100101111, 0b11111011110110101, 0b11010111101100101, 0b10111001101100111, 0b10000111010100001, 0b10111010110100111, 0b10011101000101101, 0b10001101011100011});
		CODE::BoseChaudhuriHocquenghemDecoderReference<24, 1, 65343, GF> decoder;
		bch_reference_test(&encoder, &decoder, 100);
	}
	std::cerr << "Bose Chaudhuri Hocquenghem regression test passed!" << std::endl;
	return 0;
}


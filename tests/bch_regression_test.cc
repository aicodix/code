/*
Regression Test for the Bose Chaudhuri Hocquenghem Encoder and Decoder

Copyright 2018 Ahmet Inan <inan@aicodix.de>
*/

#include <cassert>
#include <random>
#include <iostream>
#include <functional>
#include "galois_field.hh"
#include "bose_chaudhuri_hocquenghem_encoder.hh"
#include "bose_chaudhuri_hocquenghem_decoder.hh"

template <typename ENC, typename DEC>
void bch_test(ENC *encode, DEC *decode, int trials)
{
	std::random_device rd;
	std::default_random_engine generator(rd());
	typedef std::uniform_int_distribution<typename DEC::value_type> distribution;
	auto rnd_cnt = std::bind(distribution(0, DEC::NR), generator);
	auto rnd_len = std::bind(distribution(1, ENC::K), generator);
	auto rnd_val = std::bind(distribution(0, 255), generator);
	while (--trials) {
		int data_len = rnd_len();
		auto rnd_pos = std::bind(distribution(0, data_len + ENC::NP - 1), generator);
		int D = (data_len + 7) / 8;
		uint8_t data[D], orig_data[D];
		for (int i = 0; i < D; ++i)
			data[i] = orig_data[i] = rnd_val();
		int P = (ENC::NP + 7) / 8;
		uint8_t parity[P];
		(*encode)(data, parity, data_len);
		for (int i = 0; i < D; ++i)
			assert(data[i] == orig_data[i]);
		uint8_t orig_parity[P];
		for (int i = 0; i < P; ++i)
			orig_parity[i] = parity[i];
		int error_count = rnd_cnt() / 2;
		typename DEC::value_type errors[DEC::NR];
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
				CODE::xor_be_bit(data, pos, 1);
			else
				CODE::xor_be_bit(parity, pos-data_len, 1);
		}
		int erasures_count = DEC::NR - 2 * error_count;
		typename DEC::value_type erasures[erasures_count];
		for (int i = 0; i < erasures_count; ++i) {
			int pos = rnd_pos();
			for (int j = 0; j < error_count + i; ++j) {
				if (errors[j] == pos) {
					pos = rnd_pos();
					j = -1;
				}
			}
			errors[error_count + i] = pos;
			erasures[i] = pos;
			if (pos < data_len)
				CODE::xor_be_bit(data, pos, 1);
			else
				CODE::xor_be_bit(parity, pos-data_len, 1);
		}
		int ret = (*decode)(data, parity, erasures, erasures_count, data_len);
		assert(ret >= 0);
		for (int i = 0; i < D; ++i)
			assert(data[i] == orig_data[i]);
		for (int i = 0; i < P; ++i)
			assert(parity[i] == orig_parity[i]);
	}
}

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
		typename ENC::value_type errors[ENC::NR];
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
			for (int j = 0; j < error_count + i; ++j) {
				if (errors[j] == pos) {
					pos = rnd_pos();
					j = -1;
				}
			}
			errors[error_count + i] = pos;
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
		CODE::BoseChaudhuriHocquenghemEncoder<15, 5> encoder({0b10011, 0b11111, 0b00111});
		CODE::BoseChaudhuriHocquenghemDecoder<6, 1, 5, GF> decoder;
		bch_test(&encoder, &decoder, 1000000);
	}
	if (1) {
		// DVB-S2 FULL BCH(16383, 16215) T=12
		typedef CODE::GaloisField<14, 0b100000000101011, uint16_t> GF;
		GF instance;
		CODE::BoseChaudhuriHocquenghemEncoder<16383, 16215> encoder({
			0b100000000101011, 0b100100101000001, 0b100011001000111,
			0b101010110010001, 0b110101101010101, 0b110001110001001,
			0b110110011100101, 0b100111100100001, 0b100011000001111,
			0b101101001001001, 0b101100000010001, 0b110010111101111});
		CODE::BoseChaudhuriHocquenghemDecoder<24, 1, 16215, GF> decoder;
		bch_test(&encoder, &decoder, 100);
	}
	if (1) {
		// DVB-S2 FULL BCH(65535, 65343) T=12
		typedef CODE::GaloisField<16, 0b10000000000101101, uint16_t> GF;
		GF instance;
		CODE::BoseChaudhuriHocquenghemEncoder<65535, 65343> encoder({
			0b10000000000101101, 0b10000000101110011, 0b10000111110111101,
			0b10101101001010101, 0b10001111100101111, 0b11111011110110101,
			0b11010111101100101, 0b10111001101100111, 0b10000111010100001,
			0b10111010110100111, 0b10011101000101101, 0b10001101011100011});
		CODE::BoseChaudhuriHocquenghemDecoder<24, 1, 65343, GF> decoder;
		bch_test(&encoder, &decoder, 100);
	}
	if (1) {
		// NASA INTRO BCH(15, 5) T=3
		typedef CODE::GaloisField<4, 0b10011, uint8_t> GF;
		GF instance;
		CODE::BoseChaudhuriHocquenghemEncoderReference<6, 1, 5, GF> encoder({0b10011, 0b11111, 0b00111});
		CODE::BoseChaudhuriHocquenghemDecoderReference<6, 1, 5, GF> decoder;
		bch_reference_test(&encoder, &decoder, 1000000);
	}
	if (1) {
		// DVB-S2 FULL BCH(16383, 16215) T=12
		typedef CODE::GaloisField<14, 0b100000000101011, uint16_t> GF;
		GF instance;
		CODE::BoseChaudhuriHocquenghemEncoderReference<24, 1, 16215, GF> encoder({
			0b100000000101011, 0b100100101000001, 0b100011001000111,
			0b101010110010001, 0b110101101010101, 0b110001110001001,
			0b110110011100101, 0b100111100100001, 0b100011000001111,
			0b101101001001001, 0b101100000010001, 0b110010111101111});
		CODE::BoseChaudhuriHocquenghemDecoderReference<24, 1, 16215, GF> decoder;
		bch_reference_test(&encoder, &decoder, 100);
	}
	if (1) {
		// DVB-S2 FULL BCH(65535, 65343) T=12
		typedef CODE::GaloisField<16, 0b10000000000101101, uint16_t> GF;
		GF instance;
		CODE::BoseChaudhuriHocquenghemEncoderReference<24, 1, 65343, GF> encoder({
			0b10000000000101101, 0b10000000101110011, 0b10000111110111101,
			0b10101101001010101, 0b10001111100101111, 0b11111011110110101,
			0b11010111101100101, 0b10111001101100111, 0b10000111010100001,
			0b10111010110100111, 0b10011101000101101, 0b10001101011100011});
		CODE::BoseChaudhuriHocquenghemDecoderReference<24, 1, 65343, GF> decoder;
		bch_reference_test(&encoder, &decoder, 100);
	}
	std::cerr << "Bose Chaudhuri Hocquenghem regression test passed!" << std::endl;
	return 0;
}


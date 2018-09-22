/*
Bose Chaudhuri Hocquenghem Decoder

Copyright 2018 Ahmet Inan <inan@aicodix.de>
*/

#ifndef BOSE_CHAUDHURI_HOCQUENGHEM_DECODER_HH
#define BOSE_CHAUDHURI_HOCQUENGHEM_DECODER_HH

#include "reed_solomon_error_correction.hh"
#include "bitman.hh"

namespace CODE {

template <int NR, int FCR, int MSG, typename GF>
class BoseChaudhuriHocquenghemDecoder
{
public:
	typedef typename GF::value_type value_type;
	typedef typename GF::ValueType ValueType;
	typedef typename GF::IndexType IndexType;
	static const int N = GF::N, K = MSG, NP = N - K;
	ReedSolomonErrorCorrection<NR, FCR, GF> algorithm;
	int compute_syndromes(uint8_t *code, ValueType *syndromes)
	{
		// $syndromes_i = code(pe^{FCR+i})$
		for (int i = 0; i < NR; ++i)
			syndromes[i] = ValueType(get_be_bit(code, 0));
		for (int j = 1; j < N; ++j) {
			IndexType root(FCR), pe(1);
			for (int i = 0; i < NR; ++i) {
				syndromes[i] = fma(root, syndromes[i], ValueType(get_be_bit(code, j)));
				root *= pe;
			}
		}
		int nonzero = 0;
		for (int i = 0; i < NR; ++i)
			nonzero += !!syndromes[i];
		return nonzero;
	}
	int compute_syndromes(uint8_t *code, value_type *syndromes)
	{
		return compute_syndromes(code, reinterpret_cast<ValueType *>(syndromes));
	}
	int operator()(uint8_t *code, value_type *erasures = 0, int erasures_count = 0)
	{
		assert(0 <= erasures_count && erasures_count <= NR);
		if (0) {
			for (int i = 0; i < erasures_count; ++i)
				set_be_bit(code, erasures[i], 0);
		}
		ValueType syndromes[NR];
		if (!compute_syndromes(code, syndromes))
			return 0;
		IndexType locations[NR];
		ValueType magnitudes[NR];
		int count = algorithm(syndromes, locations, magnitudes, reinterpret_cast<IndexType *>(erasures), erasures_count);
		if (count <= 0)
			return count;
		for (int i = 0; i < count; ++i)
			if (1 < (int)magnitudes[i])
				return -1;
		for (int i = 0; i < count; ++i)
			xor_be_bit(code, (int)locations[i], (bool)magnitudes[i]);
		int corrections_count = 0;
		for (int i = 0; i < count; ++i)
			corrections_count += !!magnitudes[i];
		return corrections_count;
	}
};

template <int NR, int FCR, int MSG, typename GF>
class BoseChaudhuriHocquenghemDecoderReference
{
public:
	typedef typename GF::value_type value_type;
	typedef typename GF::ValueType ValueType;
	typedef typename GF::IndexType IndexType;
	static const int N = GF::N, K = MSG, NP = N - K;
	ReedSolomonErrorCorrection<NR, FCR, GF> algorithm;
	int compute_syndromes(ValueType *code, ValueType *syndromes)
	{
		// $syndromes_i = code(pe^{FCR+i})$
		for (int i = 0; i < NR; ++i)
			syndromes[i] = code[0];
		for (int j = 1; j < N; ++j) {
			IndexType root(FCR), pe(1);
			for (int i = 0; i < NR; ++i) {
				syndromes[i] = fma(root, syndromes[i], code[j]);
				root *= pe;
			}
		}
		int nonzero = 0;
		for (int i = 0; i < NR; ++i)
			nonzero += !!syndromes[i];
		return nonzero;
	}
	int operator()(ValueType *code, IndexType *erasures = 0, int erasures_count = 0)
	{
		assert(0 <= erasures_count && erasures_count <= NR);
		if (0) {
			for (int i = 0; i < erasures_count; ++i)
				code[(int)erasures[i]] = ValueType(0);
		}
		ValueType syndromes[NR];
		if (!compute_syndromes(code, syndromes))
			return 0;
		IndexType locations[NR];
		ValueType magnitudes[NR];
		int count = algorithm(syndromes, locations, magnitudes, erasures, erasures_count);
		if (count <= 0)
			return count;
		for (int i = 0; i < count; ++i)
			if (1 < (int)magnitudes[i])
				return -1;
		for (int i = 0; i < count; ++i)
			code[(int)locations[i]] += magnitudes[i];
		int corrections_count = 0;
		for (int i = 0; i < count; ++i)
			corrections_count += !!magnitudes[i];
		return corrections_count;
	}
};

}

#endif

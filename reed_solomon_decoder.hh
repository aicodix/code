/*
Reed Solomon Decoder

Copyright 2018 Ahmet Inan <inan@aicodix.de>
*/

#ifndef REED_SOLOMON_DECODER_HH
#define REED_SOLOMON_DECODER_HH

#include "reed_solomon_error_correction.hh"

namespace CODE {

template <int NR, int FCR, typename GF>
class ReedSolomonDecoder
{
public:
	typedef typename GF::value_type value_type;
	typedef typename GF::ValueType ValueType;
	typedef typename GF::IndexType IndexType;
	static const int N = GF::N, K = N - NR, NP = NR;
private:
	ReedSolomonErrorCorrection<NR, FCR, GF> algorithm;
	void update_syndromes(ValueType *poly, ValueType *syndromes, int begin, int end)
	{
		for (int j = begin; j < end; ++j) {
			ValueType coeff(poly[j]);
			IndexType root(FCR), pe(1);
			for (int i = 0; i < NR; ++i) {
				syndromes[i] = fma(root, syndromes[i], coeff);
				root *= pe;
			}
		}
	}
public:
	int compute_syndromes(ValueType *code, ValueType *syndromes)
	{
		// $syndromes_i = code(pe^{FCR+i})$
		ValueType coeff(code[0]);
		for (int i = 0; i < NR; ++i)
			syndromes[i] = coeff;
		update_syndromes(code, syndromes, 1, N);
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
			code[(int)locations[i]] += magnitudes[i];
		int corrections_count = 0;
		for (int i = 0; i < count; ++i)
			corrections_count += !!magnitudes[i];
		return corrections_count;
	}
	int operator()(value_type *code, value_type *erasures = 0, int erasures_count = 0)
	{
		return (*this)(reinterpret_cast<ValueType *>(code), reinterpret_cast<IndexType *>(erasures), erasures_count);
	}
	int compute_syndromes(value_type *code, value_type *syndromes)
	{
		return compute_syndromes(reinterpret_cast<ValueType *>(code), reinterpret_cast<ValueType *>(syndromes));
	}
};

}

#endif

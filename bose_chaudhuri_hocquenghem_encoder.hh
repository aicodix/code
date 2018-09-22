/*
Bose Chaudhuri Hocquenghem Encoder

Copyright 2018 Ahmet Inan <inan@aicodix.de>
*/

#ifndef BOSE_CHAUDHURI_HOCQUENGHEM_ENCODER_HH
#define BOSE_CHAUDHURI_HOCQUENGHEM_ENCODER_HH

#include <initializer_list>

namespace CODE {

template <int LEN, int MSG>
class BoseChaudhuriHocquenghemEncoder
{
public:
	static const int N = LEN, K = MSG, NP = N - K;
	static const int G = ((NP+1)+7)/8;
	uint8_t generator[G];
	BoseChaudhuriHocquenghemEncoder(std::initializer_list<int> minimal_polynomials)
	{
		// $generator(x) = \prod_i(minpoly_i(x))$
		int generator_degree = 1;
		generator[0] = 1;
		for (int i = 1; i < G; ++i)
			generator[i] = 0;
		for (auto m: minimal_polynomials) {
			int m_degree = 0;
			while (m>>m_degree)
				++m_degree;
			--m_degree;
			assert(generator_degree + m_degree <= NP + 1);
			for (int i = generator_degree; i >= 0; --i) {
				if (!((generator[i/8]>>(i%8))&1))
					continue;
				generator[i/8] &= ~(1<<(i%8));
				generator[i/8] |= (m&1)<<(i%8);
				for (int j = 1; j <= m_degree; ++j)
					generator[(i+j)/8] ^= ((m>>j)&1)<<((i+j)%8);
			}
			generator_degree += m_degree;
		}
		assert(generator_degree == NP + 1);
		if (0) {
			std::cerr << "generator =";
			for (int i = 0; i <= NP; ++i)
				std::cerr << " " << int((generator[i/8] >> (i%8))&1);
			std::cerr << std::endl;
		}
	}
	void operator()(uint8_t *code)
	{
		// $code = data * x^{NP} + (data * x^{NP}) \mod{generator}$
		for (int i = 0; i < NP; ++i)
			code[(K+i)/8] &= ~(128>>((K+i)%8));
		for (int i = 0; i < K; ++i) {
			if (((code[i/8]>>(7-i%8))&1) != ((code[K/8]>>(7-K%8))&1)) {
				for (int j = 1; j < NP; ++j) {
					code[(K+j-1)/8] &= ~(128>>((K+j-1)%8));
					code[(K+j-1)/8] |= (((generator[(NP-j)/8]>>((NP-j)%8))^(code[(K+j)/8]>>(7-(K+j)%8)))&1)<<(7-(K+j-1)%8);
				}
				code[(N-1)/8] &= ~(128>>((N-1)%8));
				code[(N-1)/8] |= (generator[0]&1)<<(7-(N-1)%8);
			} else {
				for (int j = 1; j < NP; ++j) {
					code[(K+j-1)/8] &= ~(128>>((K+j-1)%8));
					code[(K+j-1)/8] |= ((code[(K+j)/8]>>(7-(K+j)%8))&1)<<(7-(K+j-1)%8);
				}
				code[(N-1)/8] &= ~(128>>((N-1)%8));
			}
		}
	}
};

template <int NR, int FCR, int MSG, typename GF>
class BoseChaudhuriHocquenghemEncoderReference
{
public:
	typedef typename GF::value_type value_type;
	typedef typename GF::ValueType ValueType;
	typedef typename GF::IndexType IndexType;
	static const int N = GF::N, K = MSG, NP = N - K;
	ValueType generator[NP+1];
	BoseChaudhuriHocquenghemEncoderReference(std::initializer_list<int> minimal_polynomials)
	{
		// $generator(x) = \prod_i(minpoly_i(x))$
		int generator_degree = 1;
		generator[0] = ValueType(1);
		for (int i = 1; i <= NP; ++i)
			generator[i] = ValueType(0);
		for (auto m: minimal_polynomials) {
			assert(0 < m && m < 1<<(GF::M+1));
			int m_degree = GF::M;
			while (!(m>>m_degree))
				--m_degree;
			assert(generator_degree + m_degree <= NP + 1);
			for (int i = generator_degree; i >= 0; --i) {
				if (!generator[i])
					continue;
				generator[i] = ValueType(m&1);
				for (int j = 1; j <= m_degree; ++j)
					generator[i+j] += ValueType((m>>j)&1);
			}
			generator_degree += m_degree;
		}
		assert(generator_degree == NP + 1);
		if (0) {
			IndexType root(FCR), pe(1);
			for (int i = 0; i < NR; ++i) {
				ValueType tmp(generator[NP]);
				for (int j = 1; j <= NP; ++j)
					tmp = fma(root, tmp, generator[NP-j]);
				assert(!tmp);
				root *= pe;
			}
			std::cerr << "generator =";
			for (int i = 0; i <= NP; ++i)
				std::cerr << " " << (int)generator[i];
			std::cerr << std::endl;
		}
	}
	void operator()(ValueType *code)
	{
		// $code = data * x^{NP} + (data * x^{NP}) \mod{generator}$
		for (int i = 0; i < NP; ++i)
			code[K+i] = ValueType(0);
		for (int i = 0; i < K; ++i) {
			if (code[i] != code[K]) {
				for (int j = 1; j < NP; ++j)
					code[K+j-1] = generator[NP-j] + code[K+j];
				code[N-1] = generator[0];
			} else {
				for (int j = 1; j < NP; ++j)
					code[K+j-1] = code[K+j];
				code[N-1] = ValueType(0);
			}
		}
	}
};

}

#endif

/*
Reed Solomon Encoder

Copyright 2018 Ahmet Inan <inan@aicodix.de>
*/

#ifndef REED_SOLOMON_ENCODER_HH
#define REED_SOLOMON_ENCODER_HH

namespace CODE {

template <int NR, int FCR, typename GF>
class ReedSolomonEncoder
{
public:
	typedef typename GF::value_type value_type;
	typedef typename GF::ValueType ValueType;
	typedef typename GF::IndexType IndexType;
	static const int N = GF::N, K = N - NR, NP = NR;
	IndexType generator[NR+1];
	ReedSolomonEncoder()
	{
		// $generator = \prod_{i=0}^{NR}(x-pe^{FCR+i})$
		ValueType tmp[NR+1];
		IndexType root(FCR), pe(1);
		for (int i = 0; i < NR; ++i) {
			tmp[i] = ValueType(1);
			for (int j = i; j > 0; --j)
				tmp[j] = fma(root, tmp[j], tmp[j-1]);
			tmp[0] *= root;
			root *= pe;
		}
		tmp[NR] = ValueType(1);
		if (0) {
			std::cerr << "generator = ";
			for (int i = NR; i > 0; --i) {
				if (!tmp[i])
					continue;
				if (tmp[i] != ValueType(1))
					std::cerr << (int)tmp[i] << "*";
				std::cerr << "x";
				if (i != 1)
					std::cerr << "^" << i;
				std::cerr << " + ";
			}
			std::cerr << (int)tmp[0] << std::endl;
		}
		for (int i = 0; i <= NR; ++i)
			generator[i] = index(tmp[i]);
	}
	void operator()(ValueType *code)
	{
		// $code = data * x^{NR} + (data * x^{NR}) \mod{generator}$
		for (int i = 0; i < NR; ++i)
			code[K+i] = ValueType(0);
		for (int i = 0; i < K; ++i) {
			ValueType feedback = code[i] + code[K];
			if (feedback) {
				IndexType fb = index(feedback);
				for (int j = 1; j < NR; ++j)
					code[K+j-1] = fma(fb, generator[NR-j], code[K+j]);
				code[N-1] = value(generator[0] * fb);
			} else {
				for (int j = 1; j < NR; ++j)
					code[K+j-1] = code[K+j];
				code[N-1] = ValueType(0);
			}
		}
	}
	void operator()(value_type *code)
	{
		(*this)(reinterpret_cast<ValueType *>(code));
	}
};

}

#endif

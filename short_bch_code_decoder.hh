/*
Decoder for short BCH codes

Copyright 2020 Ahmet Inan <inan@aicodix.de>
*/

#pragma once

namespace CODE {

template <int N, int K>
class ShortBCHCodeDecoder
{
	static const int P = N - K;
	static const int W = 1 << K;
	static const int R = 1 << P;
	int err[R];
	short par[W];
	static_assert(N < 8 * sizeof(err[0]), "codeword type not wide enough");
	static_assert(P < 8 * sizeof(par[0]), "parity type not wide enough");
	static int modgen(int inp, int gen)
	{
		for (int i = K-1; i >= 0; --i) {
			int tmp = inp >> (i+P);
			inp ^= (tmp & 1) * (gen << i);
		}
		return inp;
	}
public:
	ShortBCHCodeDecoder(int generator, int T)
	{
		for (int i = 0; i < W; ++i)
			par[i] = modgen(i << P, generator);
		err[0] = 0;
		assert(T > 0 && T <= 4);
		for (int a = 1<<(N-1); T >= 1 && a; a >>= 1) {
			err[modgen(a, generator)] = a;
			for (int b = a >> 1; T >= 2 && b; b >>= 1) {
				err[modgen(a|b, generator)] = a|b;
				for (int c = b >> 1; T >= 3 && c; c >>= 1) {
					err[modgen(a|b|c, generator)] = a|b|c;
					for (int d = c >> 1; T >= 4 && d; d >>= 1) {
						err[modgen(a|b|c|d, generator)] = a|b|c|d;
					}
				}
			}
		}
	}
	int operator()(int inp)
	{
		return inp ^ err[(par[inp>>P] ^ inp) & (R-1)];
	}
};

}


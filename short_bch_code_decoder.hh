/*
Soft decoder for short BCH codes

Copyright 2020 Ahmet Inan <inan@aicodix.de>
*/

#pragma once

namespace CODE {

template <int N, int K, int T>
class ShortBCHCodeDecoder
{
	static const int P = N - K;
	static const int W = 1 << K;
	static const int R = 1 << P;
	int err[R];
	short par[W];
	static_assert(N < 8 * sizeof(err[0]), "codeword type not wide enough");
	static_assert(P < 8 * sizeof(par[0]), "parity type not wide enough");
	static_assert(T > 0 && T <= 4, "unsupported radius T");
	static int modgen(int inp, int gen)
	{
		for (int i = K-1; i >= 0; --i) {
			int tmp = inp >> (i+P);
			inp ^= (tmp & 1) * (gen << i);
		}
		return inp;
	}
	static int metric(const int8_t *soft, int hard)
	{
		int lol = 8 * sizeof(hard) - 1;
		int sum = 0;
		for (int i = 0; i < N; ++i)
			sum += ((hard << (lol-i)) >> lol) * soft[i];
		return sum;
	}
public:
	ShortBCHCodeDecoder(int generator)
	{
		for (int i = 0; i < W; ++i)
			par[i] = modgen(i << P, generator);
		err[0] = 0;
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
	int operator()(const int8_t *code)
	{
		int cw = 0;
		for (int i = 0; i < N; ++i)
			cw |= (code[i] < 0) << i;
		int word = (*this)(cw);
		// hard decision
		if (0)
			return word;
		// metric of hard decision
		int best = metric(code, word);
		int next = -1;
		// flip each bit and see ..
		if (0) {
			for (int j = 0; j < N; ++j) {
				int tmp = 1 << j;
				int dec = (*this)(cw ^ tmp);
				if (dec == word)
					continue;
				int met = metric(code, dec);
				if (met > best) {
					next = best;
					best = met;
					word = dec;
				} else if (met > next) {
					next = met;
				}
			}
		}
		// Chase algorithm
		if (K > 6) {
			int worst[T] = { 0 };
			for (int k = 0; k < N; ++k) {
				int j = 0;
				while (j < T && std::abs(code[worst[j]]) < std::abs(code[k]))
					++j;
				for (int i = T-1; i > j; --i)
					worst[i] = worst[i-1];
				if (j < T)
					worst[j] = k;
			}
			for (int j = 1; j < (1 << T); ++j) {
				int tmp = 0;
				for (int i = 0; i < T; ++i)
					tmp |= ((j>>i)&1) << worst[i];
				int dec = (*this)(cw ^ tmp);
				if (dec == word)
					continue;
				int met = metric(code, dec);
				if (met > best) {
					next = best;
					best = met;
					word = dec;
				} else if (met > next) {
					next = met;
				}
			}
		}
		// maximum likelihood
		if (K <= 6) {
			for (int msg = 0; msg < W; ++msg) {
				int enc = (msg << P) | par[msg];
				if (enc == word)
					continue;
				int met = metric(code, enc);
				if (met > best) {
					next = best;
					best = met;
					word = enc;
				} else if (met > next) {
					next = met;
				}
			}
		}
		if (best == next)
			return -1;
		return word;
	}
};

}


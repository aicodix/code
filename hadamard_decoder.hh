/*
Soft decoder for augmented Hadamard codes

Copyright 2020 Ahmet Inan <inan@aicodix.de>
*/

#pragma once

namespace CODE {

template <int K>
class HadamardDecoder
{
	static const int W = 1 << K;
	static const int N = 1 << (K - 1);
public:
	int operator()(const int8_t *c)
	{
		int sum[N];
		for (int i = 0; i < N; i += 2) {
			sum[i] = c[i] + c[i+1];
			sum[i+1] = c[i] - c[i+1];
		}
		for (int h = 2; h < N; h *= 2) {
			for (int i = 0; i < N; i += 2 * h) {
				for (int j = i; j < i + h; ++j) {
					int x = sum[j] + sum[j+h];
					int y = sum[j] - sum[j+h];
					sum[j] = x;
					sum[j+h] = y;
				}
			}
		}
		int word = 0, best = 0, next = 0;
		for (int i = 0; i < N; ++i) {
			int mag = std::abs(sum[i]);
			int msg = i + N * (sum[i] < 0);
			if (mag > best) {
				next = best;
				best = mag;
				word = msg;
			} else if (mag > next) {
				next = mag;
			}
		}
		if (best == next)
			return -1;
		return word;
	}
};

template <>
class HadamardDecoder<2>
{
	static const int K = 2;
	static const int W = 1 << K;
public:
	int operator()(const int8_t *code)
	{
		int tmp[2] = {
			code[0] + code[1], code[0] - code[1],
		};
		int sum[W] = {
			+ tmp[0],
			+ tmp[1],
			- tmp[0],
			- tmp[1],
		};
		int word = 0, best = 0, next = 0;
		for (int msg = 0 ; msg < W; ++msg) {
			if (sum[msg] > best) {
				next = best;
				best = sum[msg];
				word = msg;
			} else if (sum[msg] > next) {
				next = sum[msg];
			}
		}
		if (best == next)
			return -1;
		return word;
	}
};

template <>
class HadamardDecoder<3>
{
	static const int K = 3;
	static const int W = 1 << K;
public:
	int operator()(const int8_t *c)
	{
		int d[4] = {
			c[0] + c[1], c[0] - c[1],
			c[2] + c[3], c[2] - c[3],
		};
		int e[4] = {
			d[0] + d[2], d[0] - d[2],
			d[1] + d[3], d[1] - d[3],
		};
		int sum[W] = {
			+ e[0],
			+ e[2],
			+ e[1],
			+ e[3],
			- e[0],
			- e[2],
			- e[1],
			- e[3],
		};
		int word = 0, best = 0, next = 0;
		for (int msg = 0 ; msg < W; ++msg) {
			if (sum[msg] > best) {
				next = best;
				best = sum[msg];
				word = msg;
			} else if (sum[msg] > next) {
				next = sum[msg];
			}
		}
		if (best == next)
			return -1;
		return word;
	}
};

template <>
class HadamardDecoder<4>
{
	static const int K = 4;
	static const int W = 1 << K;
public:
	int operator()(const int8_t *c)
	{
		int d[8] = {
			c[0] + c[1], c[0] - c[1],
			c[2] + c[3], c[2] - c[3],
			c[4] + c[5], c[4] - c[5],
			c[6] + c[7], c[6] - c[7],
		};
		int e[8] = {
			d[0] + d[2], d[0] - d[2],
			d[1] + d[3], d[1] - d[3],
			d[4] + d[6], d[4] - d[6],
			d[5] + d[7], d[5] - d[7],
		};
		int f[8] = {
			e[0] + e[4], e[0] - e[4],
			e[1] + e[5], e[1] - e[5],
			e[2] + e[6], e[2] - e[6],
			e[3] + e[7], e[3] - e[7],
		};
		int sum[W] = {
			+ f[0],
			+ f[4],
			+ f[2],
			+ f[6],
			+ f[1],
			+ f[5],
			+ f[3],
			+ f[7],
			- f[0],
			- f[4],
			- f[2],
			- f[6],
			- f[1],
			- f[5],
			- f[3],
			- f[7],
		};
		int word = 0, best = 0, next = 0;
		for (int msg = 0 ; msg < W; ++msg) {
			if (sum[msg] > best) {
				next = best;
				best = sum[msg];
				word = msg;
			} else if (sum[msg] > next) {
				next = sum[msg];
			}
		}
		if (best == next)
			return -1;
		return word;
	}
};

template <>
class HadamardDecoder<5>
{
	static const int K = 5;
	static const int W = 1 << K;
public:
	int operator()(const int8_t *c)
	{
		int d[16] = {
			c[0] + c[1], c[0] - c[1],
			c[2] + c[3], c[2] - c[3],
			c[4] + c[5], c[4] - c[5],
			c[6] + c[7], c[6] - c[7],
			c[8] + c[9], c[8] - c[9],
			c[10] + c[11], c[10] - c[11],
			c[12] + c[13], c[12] - c[13],
			c[14] + c[15], c[14] - c[15],
		};
		int e[16] = {
			d[0] + d[2], d[0] - d[2],
			d[1] + d[3], d[1] - d[3],
			d[4] + d[6], d[4] - d[6],
			d[5] + d[7], d[5] - d[7],
			d[8] + d[10], d[8] - d[10],
			d[9] + d[11], d[9] - d[11],
			d[12] + d[14], d[12] - d[14],
			d[13] + d[15], d[13] - d[15],
		};
		int f[16] = {
			e[0] + e[4], e[0] - e[4],
			e[1] + e[5], e[1] - e[5],
			e[2] + e[6], e[2] - e[6],
			e[3] + e[7], e[3] - e[7],
			e[8] + e[12], e[8] - e[12],
			e[9] + e[13], e[9] - e[13],
			e[10] + e[14], e[10] - e[14],
			e[11] + e[15], e[11] - e[15],
		};
		int g[16] = {
			f[0] + f[8], f[0] - f[8],
			f[1] + f[9], f[1] - f[9],
			f[2] + f[10], f[2] - f[10],
			f[3] + f[11], f[3] - f[11],
			f[4] + f[12], f[4] - f[12],
			f[5] + f[13], f[5] - f[13],
			f[6] + f[14], f[6] - f[14],
			f[7] + f[15], f[7] - f[15],
		};
		int sum[W] = {
			+ g[0],
			+ g[8],
			+ g[4],
			+ g[12],
			+ g[2],
			+ g[10],
			+ g[6],
			+ g[14],
			+ g[1],
			+ g[9],
			+ g[5],
			+ g[13],
			+ g[3],
			+ g[11],
			+ g[7],
			+ g[15],
			- g[0],
			- g[8],
			- g[4],
			- g[12],
			- g[2],
			- g[10],
			- g[6],
			- g[14],
			- g[1],
			- g[9],
			- g[5],
			- g[13],
			- g[3],
			- g[11],
			- g[7],
			- g[15],
		};
		int word = 0, best = 0, next = 0;
		for (int msg = 0 ; msg < W; ++msg) {
			if (sum[msg] > best) {
				next = best;
				best = sum[msg];
				word = msg;
			} else if (sum[msg] > next) {
				next = sum[msg];
			}
		}
		if (best == next)
			return -1;
		return word;
	}
};

template <>
class HadamardDecoder<6>
{
	static const int K = 6;
	static const int W = 1 << K;
public:
	int operator()(const int8_t *c)
	{
		int d[32] = {
			c[0] + c[1], c[0] - c[1],
			c[2] + c[3], c[2] - c[3],
			c[4] + c[5], c[4] - c[5],
			c[6] + c[7], c[6] - c[7],
			c[8] + c[9], c[8] - c[9],
			c[10] + c[11], c[10] - c[11],
			c[12] + c[13], c[12] - c[13],
			c[14] + c[15], c[14] - c[15],
			c[16] + c[17], c[16] - c[17],
			c[18] + c[19], c[18] - c[19],
			c[20] + c[21], c[20] - c[21],
			c[22] + c[23], c[22] - c[23],
			c[24] + c[25], c[24] - c[25],
			c[26] + c[27], c[26] - c[27],
			c[28] + c[29], c[28] - c[29],
			c[30] + c[31], c[30] - c[31],
		};
		int e[32] = {
			d[0] + d[2], d[0] - d[2],
			d[1] + d[3], d[1] - d[3],
			d[4] + d[6], d[4] - d[6],
			d[5] + d[7], d[5] - d[7],
			d[8] + d[10], d[8] - d[10],
			d[9] + d[11], d[9] - d[11],
			d[12] + d[14], d[12] - d[14],
			d[13] + d[15], d[13] - d[15],
			d[16] + d[18], d[16] - d[18],
			d[17] + d[19], d[17] - d[19],
			d[20] + d[22], d[20] - d[22],
			d[21] + d[23], d[21] - d[23],
			d[24] + d[26], d[24] - d[26],
			d[25] + d[27], d[25] - d[27],
			d[28] + d[30], d[28] - d[30],
			d[29] + d[31], d[29] - d[31],
		};
		int f[32] = {
			e[0] + e[4], e[0] - e[4],
			e[1] + e[5], e[1] - e[5],
			e[2] + e[6], e[2] - e[6],
			e[3] + e[7], e[3] - e[7],
			e[8] + e[12], e[8] - e[12],
			e[9] + e[13], e[9] - e[13],
			e[10] + e[14], e[10] - e[14],
			e[11] + e[15], e[11] - e[15],
			e[16] + e[20], e[16] - e[20],
			e[17] + e[21], e[17] - e[21],
			e[18] + e[22], e[18] - e[22],
			e[19] + e[23], e[19] - e[23],
			e[24] + e[28], e[24] - e[28],
			e[25] + e[29], e[25] - e[29],
			e[26] + e[30], e[26] - e[30],
			e[27] + e[31], e[27] - e[31],
		};
		int g[32] = {
			f[0] + f[8], f[0] - f[8],
			f[1] + f[9], f[1] - f[9],
			f[2] + f[10], f[2] - f[10],
			f[3] + f[11], f[3] - f[11],
			f[4] + f[12], f[4] - f[12],
			f[5] + f[13], f[5] - f[13],
			f[6] + f[14], f[6] - f[14],
			f[7] + f[15], f[7] - f[15],
			f[16] + f[24], f[16] - f[24],
			f[17] + f[25], f[17] - f[25],
			f[18] + f[26], f[18] - f[26],
			f[19] + f[27], f[19] - f[27],
			f[20] + f[28], f[20] - f[28],
			f[21] + f[29], f[21] - f[29],
			f[22] + f[30], f[22] - f[30],
			f[23] + f[31], f[23] - f[31],
		};
		int h[32] = {
			g[0] + g[16], g[0] - g[16],
			g[1] + g[17], g[1] - g[17],
			g[2] + g[18], g[2] - g[18],
			g[3] + g[19], g[3] - g[19],
			g[4] + g[20], g[4] - g[20],
			g[5] + g[21], g[5] - g[21],
			g[6] + g[22], g[6] - g[22],
			g[7] + g[23], g[7] - g[23],
			g[8] + g[24], g[8] - g[24],
			g[9] + g[25], g[9] - g[25],
			g[10] + g[26], g[10] - g[26],
			g[11] + g[27], g[11] - g[27],
			g[12] + g[28], g[12] - g[28],
			g[13] + g[29], g[13] - g[29],
			g[14] + g[30], g[14] - g[30],
			g[15] + g[31], g[15] - g[31],
		};
		int sum[W] = {
			+ h[0],
			+ h[16],
			+ h[8],
			+ h[24],
			+ h[4],
			+ h[20],
			+ h[12],
			+ h[28],
			+ h[2],
			+ h[18],
			+ h[10],
			+ h[26],
			+ h[6],
			+ h[22],
			+ h[14],
			+ h[30],
			+ h[1],
			+ h[17],
			+ h[9],
			+ h[25],
			+ h[5],
			+ h[21],
			+ h[13],
			+ h[29],
			+ h[3],
			+ h[19],
			+ h[11],
			+ h[27],
			+ h[7],
			+ h[23],
			+ h[15],
			+ h[31],
			- h[0],
			- h[16],
			- h[8],
			- h[24],
			- h[4],
			- h[20],
			- h[12],
			- h[28],
			- h[2],
			- h[18],
			- h[10],
			- h[26],
			- h[6],
			- h[22],
			- h[14],
			- h[30],
			- h[1],
			- h[17],
			- h[9],
			- h[25],
			- h[5],
			- h[21],
			- h[13],
			- h[29],
			- h[3],
			- h[19],
			- h[11],
			- h[27],
			- h[7],
			- h[23],
			- h[15],
			- h[31],
		};
		int word = 0, best = 0, next = 0;
		for (int msg = 0 ; msg < W; ++msg) {
			if (sum[msg] > best) {
				next = best;
				best = sum[msg];
				word = msg;
			} else if (sum[msg] > next) {
				next = sum[msg];
			}
		}
		if (best == next)
			return -1;
		return word;
	}
};

}


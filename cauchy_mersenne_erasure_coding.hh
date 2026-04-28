/*
Cauchy Mersenne 2^31-1 Prime Field Erasure Coding

Copyright 2026 Ahmet Inan <inan@aicodix.de>
*/

#pragma once

#include "mersenne_31.hh"

namespace CODE {

template <int MAX_LEN>
struct CauchyMersenneErasureCoding
{
	typedef Mersenne31 M31;
	M31 row_vec[MAX_LEN];
	// $a_{ij} = \frac{1}{x_i + y_j}$
	void cauchy_matrix_row(M31 *row_i, int i, int n)
	{
		for (int j = 0; j < n; j++) {
			M31 row(i), col(j);
			row_i[j] = rcp(row + col);
		}
	}
	// $b_{ij} = \frac{\prod_{k=1}^{n}{(x_j + y_k)(x_k + y_i)}}{(x_j + y_i)\prod_{k \ne j}^{n}{(x_j - x_k)}\prod_{k \ne i}^{n}{(y_i - y_k)}}$
	void inverse_cauchy_matrix_row(M31 *row_i, const int *rows, int i, int n)
	{
		M31 col_i(i);
		M31 row_num(1), row_den(1);
		for (int k = 0; k < n; k++) {
			M31 row_k(rows[k]), col_k(k);
			row_num *= row_k + col_i;
			if (k != i)
				row_den *= col_i - col_k;
		}
		for (int j = 0; j < n; j++) {
			M31 row_j(rows[j]);
			M31 num(row_num), den(row_den);
			for (int k = 0; k < n; k++) {
				M31 row_k(rows[k]), col_k(k);
				num *= row_j + col_k;
				if (k != j)
					den *= row_j - row_k;
			}
			row_i[j] = num / ((row_j + col_i) * den);
		}
	}
	void mac(M31 *c, const M31 *a, M31 b, int len, bool init)
	{
		if (init) {
			for (int i = 0; i < len; i++)
				c[i] = b * a[i];
		} else {
			for (int i = 0; i < len; i++)
				c[i] += b * a[i];
		}
	}
	void encode(const M31 *data, M31 *block, int block_id, int block_len, int block_cnt)
	{
		assert(block_id >= block_cnt && block_id < int(M31::P) / 2);
		assert(block_len <= MAX_LEN);
		cauchy_matrix_row(row_vec, block_id, block_cnt);
		for (int k = 0; k < block_cnt; k++)
			mac(block, data + block_len * k, row_vec[k], block_len, !k);
	}
	void decode(M31 *data, const M31 *blocks, const int *block_ids, int block_id, int block_len, int block_cnt)
	{
		assert(block_len <= MAX_LEN);
		inverse_cauchy_matrix_row(row_vec, block_ids, block_id, block_cnt);
		for (int k = 0; k < block_cnt; k++)
			mac(data, blocks + block_len * k, row_vec[k], block_len, !k);
	}
};

}


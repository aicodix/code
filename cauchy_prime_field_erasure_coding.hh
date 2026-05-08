/*
Cauchy Prime Field Erasure Coding

Copyright 2026 Ahmet Inan <inan@aicodix.de>
*/

#pragma once

#include "prime_field.hh"

namespace CODE {

template <typename PF>
struct CauchyPrimeFieldErasureCoding
{
	PF row_num, row_den;
	// $a_{ij} = \frac{1}{x_i + y_j}$
	PF cauchy_matrix(int i, int j)
	{
		PF row(i), col(j);
		return rcp(row + col);
	}
	// $b_{ij} = \frac{\prod_{k=1}^{n}{(x_j + y_k)(x_k + y_i)}}{(x_j + y_i)\prod_{k \ne j}^{n}{(x_j - x_k)}\prod_{k \ne i}^{n}{(y_i - y_k)}}$
	PF inverse_cauchy_matrix(const int *rows, int i, int j, int n)
	{
		PF row_j(rows[j]), col_i(i);
		if (j == 0) {
			PF num(1), den(1);
			for (int k = 0; k < n; k++) {
				PF row_k(rows[k]), col_k(k);
				num *= row_k + col_i;
				if (k != i)
					den *= col_i - col_k;
			}
			row_num = num;
			row_den = den;
		}
		PF num(row_num), den(row_den);
		for (int k = 0; k < n; k++) {
			PF row_k(rows[k]), col_k(k);
			num *= row_j + col_k;
			if (k != j)
				den *= row_j - row_k;
		}
		return num / ((row_j + col_i) * den);
	}
	void mac(PF *c, const PF *a, PF b, int len, bool init)
	{
		if (init) {
			for (int i = 0; i < len; i++)
				c[i] = b * a[i];
		} else {
			for (int i = 0; i < len; i++)
				c[i] += b * a[i];
		}
	}
	void encode(const PF *data, PF *block, int block_id, int block_len, int block_cnt)
	{
		assert(block_id >= block_cnt && block_id < int(PF::P) / 2);
		for (int k = 0; k < block_cnt; k++) {
			PF a_ik = cauchy_matrix(block_id, k);
			mac(block, data + block_len * k, a_ik, block_len, !k);
		}
	}
	void decode(PF *data, const PF *blocks, const int *block_ids, int block_id, int block_len, int block_cnt)
	{
		for (int k = 0; k < block_cnt; k++) {
			PF b_ik = inverse_cauchy_matrix(block_ids, block_id, k, block_cnt);
			mac(data, blocks + block_len * k, b_ik, block_len, !k);
		}
	}
};

}


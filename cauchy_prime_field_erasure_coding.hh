/*
Cauchy Prime Field Erasure Coding

Copyright 2026 Ahmet Inan <inan@aicodix.de>
*/

#pragma once

#include "prime_field.hh"

namespace CODE {

template <typename PF, int MAX_CNT>
struct CauchyPrimeFieldErasureCoding
{
	PF A[MAX_CNT], B[MAX_CNT];
	// $a_{ij} = \frac{1}{x_i + y_j}$
	PF cauchy_matrix(int i, int j)
	{
		PF row(i), col(j);
		return rcp(row + col);
	}
	// $b_{ij} = \frac{\prod_{k=1}^{n}{(x_j + y_k)}\prod_{k=1}^{n}{(x_k + y_i)}}{(x_j + y_i)\prod_{k \ne j}^{n}{(x_j - x_k)}\prod_{k \ne i}^{n}{(y_i - y_k)}}$
	void prepare_inverse_cauchy_matrix(const int *rows, int n)
	{
		for (int j = 0; j < n; j++) {
			PF row_j(rows[j]);
			PF num_x(1), den_x(1);
			for (int k = 0; k < n; k++) {
				PF row_k(rows[k]), col_k(k);
				num_x *= row_j + col_k;
				if (k != j)
					den_x *= row_j - row_k;
			}
			A[j] = num_x / den_x;
		}
		for (int i = 0; i < n; i++) {
			PF col_i(i);
			PF num_y(1), den_y(1);
			for (int k = 0; k < n; k++) {
				PF row_k(rows[k]), col_k(k);
				num_y *= row_k + col_i;
				if (k != i)
					den_y *= col_i - col_k;
			}
			B[i] = num_y / den_y;
		}
	}
	PF inverse_cauchy_matrix(const int *rows, int i, int j)
	{
		PF row_j(rows[j]), col_i(i);
		return (A[j] * B[i]) / (row_j + col_i);
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
		prepare_inverse_cauchy_matrix(block_ids, block_cnt);
		for (int k = 0; k < block_cnt; k++) {
			PF b_ik = inverse_cauchy_matrix(block_ids, block_id, k);
			mac(data, blocks + block_len * k, b_ik, block_len, !k);
		}
	}
};

}


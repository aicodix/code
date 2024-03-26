/*
Cauchy Reed Solomon Erasure Coding

Copyright 2024 Ahmet Inan <inan@aicodix.de>
*/

#pragma once

namespace CODE {

template <typename PF>
struct CauchyReedSolomonErasureCoding2
{
	PF row_num, row_den;
	// $a_{ij} = \frac{1}{x_i + y_j}$
	__attribute__((flatten))
	PF cauchy_matrix(int i, int j)
	{
		PF row(i), col(j);
		return rcp(row + col);
	}
	// $b_{ij} = \frac{\prod_{k=1}^{n}{(x_j + y_k)(x_k + y_i)}}{(x_j + y_i)\prod_{k \ne j}^{n}{(x_j - x_k)}\prod_{k \ne i}^{n}{(y_i - y_k)}}$
	__attribute__((flatten))
	PF inverse_cauchy_matrix(const PF *rows, int i, int j, int n)
	{
#if 0
		PF col_i(i);
		PF prod_xy(1), prod_x(1), prod_y(1);
		for (int k = 0; k < n; k++) {
			PF col_k(k);
			prod_xy *= (rows[j] + col_k) * (rows[k] + col_i);
			if (k != j)
				prod_x *= (rows[j] - rows[k]);
			if (k != i)
				prod_y *= (col_i - col_k);
		}
		return prod_xy / ((rows[j] + col_i) * prod_x * prod_y);
#else
		PF col_i(i);
		if (j == 0) {
			PF num(1), den(1);
			for (int k = 0, r = 2; k < n; k++, --r) {
				PF col_k(k);
				num = mul(num, add(rows[k], col_i));
				if (k != i)
					den = mul(den, sub(col_i, col_k));
				if (!r) {
					r = 3;
					num = reduce(num);
					den = reduce(den);
				}
			}
			row_num = reduce(num);
			row_den = reduce(den);
		}
		PF num(row_num), den(row_den);
		for (int k = 0, r = 2; k < n; k++, --r) {
			PF col_k(k);
			num = mul(num, add(rows[j], col_k));
			if (k != j)
				den = mul(den, sub(rows[j], rows[k]));
			if (!r) {
				r = 3;
				num = reduce(num);
				den = reduce(den);
			}
		}
		num = reduce(num);
		den = reduce(den);
		return num / (add(rows[j], col_i) * den);
#endif
	}
	__attribute__((flatten))
	static inline void multiply_accumulate(PF *c, const PF *a, PF b, int len, bool first, bool last)
	{
		if (first) {
			for (int i = 0; i < len; i++)
				c[i] = b * a[i];
		} else if (last) {
			for (int i = 0; i < len; i++)
				c[i] = reduce(add(c[i], b * a[i]));
		} else {
			for (int i = 0; i < len; i++)
				c[i] = add(c[i], b * a[i]);
		}
	}
	void encode(const PF *data, PF *block, int block_id, int block_len, int block_cnt)
	{
		assert(block_id >= block_cnt && block_id < int(PF::P) / 2);
		for (int k = 0; k < block_cnt; k++) {
			PF a_ik = cauchy_matrix(block_id, k);
			multiply_accumulate(block, data + block_len * k, a_ik, block_len, !k, k == block_cnt - 1);
		}
	}
	void decode(PF *data, const PF *blocks, const PF *block_ids, int block_idx, int block_len, int block_cnt)
	{
		for (int k = 0; k < block_cnt; k++) {
			PF b_ik = inverse_cauchy_matrix(block_ids, block_idx, k, block_cnt);
			multiply_accumulate(data, blocks + block_len * k, b_ik, block_len, !k, k == block_cnt - 1);
		}
	}
};

}


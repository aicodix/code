/*
Cauchy Reed Solomon Erasure Coding

Copyright 2023 Ahmet Inan <inan@aicodix.de>
*/

#pragma once

namespace CODE {

template <typename GF>
struct CauchyReedSolomonErasureCoding
{
	typedef typename GF::value_type value_type;
	typedef typename GF::ValueType ValueType;
	typedef typename GF::IndexType IndexType;
	// $a_{ij} = \frac{1}{x_i + y_j}$
	IndexType cauchy_matrix(int i, int j)
	{
		ValueType row(i), col(ValueType::N - j);
		return rcp(index(row + col));
	}
	// $b_{ij} = \frac{\prod_{k=1}^{n}{(x_j + y_k)(x_k + y_i)}}{(x_j + y_i)\prod_{k \ne j}^{n}{(x_j - x_k)}\prod_{k \ne i}^{n}{(y_i - y_k)}}$
	IndexType inverse_cauchy_matrix(const ValueType *rows, int i, int j, int n)
	{
		ValueType col_i(ValueType::N - i);
		IndexType prod_xy(0), prod_x(0), prod_y(0);
		for (int k = 0; k < n; k++) {
			ValueType col_k(ValueType::N - k);
			prod_xy *= index(rows[j] + col_k) * index(rows[k] + col_i);
			if (k != j)
				prod_x *= index(rows[j] + rows[k]);
			if (k != i)
				prod_y *= index(col_i + col_k);
		}
		return prod_xy / (index(rows[j] + col_i) * prod_x * prod_y);
	}
	void multiply_accumulate(ValueType *c, const ValueType *a, IndexType b, int len, bool init)
	{
		if (init) {
			for (int i = 0; i < len; i++)
				c[i] = b * a[i];
		} else {
			for (int i = 0; i < len; i++)
				c[i] = fma(b, a[i], c[i]);
		}
	}
	void encode(const ValueType *data, ValueType *block, int block_num, int block_len, int block_cnt)
	{
		assert(block_num <= ValueType::N - block_cnt);
		for (int k = 0; k < block_cnt; k++) {
			IndexType a_ik = cauchy_matrix(block_num, k);
			multiply_accumulate(block, data + block_len * k, a_ik, block_len, !k);
		}
	}
	void decode(ValueType *data, const ValueType *blocks, const ValueType *block_nums, int block_num, int block_len, int block_cnt)
	{
		for (int k = 0; k < block_cnt; k++) {
			IndexType b_ik = inverse_cauchy_matrix(block_nums, block_num, k, block_cnt);
			multiply_accumulate(data, blocks + block_len * k, b_ik, block_len, !k);
		}
	}
	void encode(const value_type *data, value_type *block, int block_num, int block_len, int block_cnt)
	{
		encode(reinterpret_cast<const ValueType *>(data), reinterpret_cast<ValueType *>(block), block_num, block_len, block_cnt);
	}
	void decode(value_type *data, const value_type *blocks, const value_type *block_nums, int block_num, int block_len, int block_cnt)
	{
		decode(reinterpret_cast<ValueType *>(data), reinterpret_cast<const ValueType *>(blocks), reinterpret_cast<const ValueType *>(block_nums), block_num, block_len, block_cnt);
	}
	void encode(const void *data, void *block, int block_number, int block_bytes, int block_count)
	{
		assert(block_bytes % sizeof(value_type) == 0);
		encode(reinterpret_cast<const value_type *>(data), reinterpret_cast<value_type *>(block), block_number, block_bytes / sizeof(value_type), block_count);
	}
	void decode(void *data, const void *blocks, const value_type *block_numbers, int block_number, int block_bytes, int block_count)
	{
		assert(block_bytes % sizeof(value_type) == 0);
		decode(reinterpret_cast<value_type *>(data), reinterpret_cast<const value_type *>(blocks), block_numbers, block_number, block_bytes / sizeof(value_type), block_count);
	}
};

}


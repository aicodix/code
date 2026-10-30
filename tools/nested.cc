/*
Generate nested frozen bit tables for polar codes

For each K = STEP * j + EXTRA, the largest erasure probability is chosen
for which the erasure probabilities of the K most reliable bits sum up to
at most EPSILON, the union bound on the block error rate of SC decoding.
EXTRA counts redundant bits like CRC, so the payload stays at STEP * j.
With BITS per entry and STEP = N >> BITS, the 2^BITS - 1 rates j = 1 .. 2^BITS - 1
and the value for always frozen fill all values of an entry.
If the resulting frozen sets are nested, a single table holds them all:
bit i is frozen for a payload of STEP * j bits, if entry i >= j.
Entries are packed into bytes, the first entry in the lowest bits.

Copyright 2026 Ahmet Inan <inan@aicodix.de>
*/

#include <cmath>
#include <limits>
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include "polar_freezer.hh"

const int M = 16, N = 1 << M;

// same recursion as the Bhattacharyya constructions in this library
double complement(double pe)
{
	bool comp = pe > 0.5;
	if (comp)
		pe = 1 - pe;
	if (pe < std::numeric_limits<double>::min())
		pe = std::numeric_limits<double>::min();
	return comp ? -pe : pe;
}

void compute(double *prob, double pe, int i, int h)
{
	if (h) {
		bool comp = pe < 0;
		double x = comp ? -pe : pe;
		double square = x * x;
		if (square < std::numeric_limits<double>::min())
			square = std::numeric_limits<double>::min();
		double grow = x * (2-x);
		bool flip = grow > 0.5;
		if (flip)
			grow = (1-x) * (1-x);
		if (comp != flip)
			grow = -grow;
		if (comp)
			square = -square;
		compute(prob, comp ? square : grow, i, h/2);
		compute(prob, comp ? grow : square, i+h, h/2);
	} else {
		prob[i] = pe < 0 ? 1 + pe : pe;
	}
}

double union_bound(double *prob, int K, double pe)
{
	compute(prob, complement(pe), 0, N / 2);
	std::nth_element(prob, prob+K, prob+N);
	double sum = 0;
	for (int i = 0; i < K; ++i)
		sum += prob[i];
	return sum;
}

int main(int argc, char **argv)
{
	if (argc > 4) {
		std::cerr << "usage: " << argv[0] << " [BITS] [EXTRA] [EPSILON]" << std::endl;
		return 1;
	}
	int bits = 8;
	if (argc >= 2)
		bits = std::atoi(argv[1]);
	if (bits != 1 && bits != 2 && bits != 4 && bits != 8) {
		std::cerr << "BITS must be 1, 2, 4 or 8" << std::endl;
		return 1;
	}
	const int RATES = (1 << bits) - 1, STEP = N >> bits, PER_BYTE = 8 / bits;
	int extra = 0;
	if (argc >= 3)
		extra = std::atoi(argv[2]);
	if (extra < 0 || extra >= STEP) {
		std::cerr << "EXTRA must be between 0 and " << STEP - 1 << std::endl;
		return 1;
	}
	double epsilon = 1e-3;
	if (argc == 4)
		epsilon = std::atof(argv[3]);
	if (!(epsilon > 0 && epsilon < 1)) {
		std::cerr << "EPSILON must be between 0 and 1" << std::endl;
		return 1;
	}
	auto prob = new double[N];
	auto frozen = new uint32_t[N/32];
	auto threshold = new uint8_t[N];
	auto freeze = new CODE::BhattacharyyaFreezer<M>;
	for (int i = 0; i < N; ++i)
		threshold[i] = RATES;
	int violations = 0;
	for (int j = 1; j <= RATES; ++j) {
		int K = STEP * j + extra;
		double lower = 0, upper = 1 - double(K) / N;
		for (int k = 0; k < 64; ++k) {
			double middle = (lower + upper) / 2;
			if (union_bound(prob, K, middle) <= epsilon)
				lower = middle;
			else
				upper = middle;
		}
		(*freeze)(frozen, M, K, lower);
		for (int i = 0; i < N; ++i) {
			bool info = !((frozen[i/32] >> (i%32)) & 1);
			if (info && threshold[i] >= j)
				threshold[i] = j - 1;
			// frozen now, but information bit at a lower rate already
			violations += !info && threshold[i] < j - 1;
		}
		std::cerr << "K = " << K << " erasure probability = " << lower << std::endl;
	}
	std::cerr << (violations ? "NOT nested, violations: " : "nested, violations: ") << violations << std::endl;
	std::cout << "// bit i is frozen for a payload of " << STEP << " * j bits plus " << extra << " extra bits, if" << std::endl;
	if (bits == 8)
		std::cout << "// threshold[i] >= j" << std::endl;
	else
		std::cout << "// ((threshold[i/" << PER_BYTE << "] >> (i%" << PER_BYTE << "*" << bits << ")) & " << RATES << ") >= j" << std::endl;
	std::cout << "static const uint8_t threshold[" << N / PER_BYTE << "] = { ";
	for (int i = 0; i < N; i += PER_BYTE) {
		int byte = 0;
		for (int k = 0; k < PER_BYTE; ++k)
			byte |= threshold[i+k] << (k * bits);
		std::cout << byte << ", ";
	}
	std::cout << "};" << std::endl;
	delete freeze;
	delete[] threshold;
	delete[] frozen;
	delete[] prob;
	return violations != 0;
}

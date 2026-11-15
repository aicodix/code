/*
Rank xorshift permutations of Hadamard codewords by burst erasure resilience

The augmented Hadamard code of HadamardEncoder<BITS+1> has 2 << BITS
codewords of N = 1 << BITS bits, which go onto N adjacent tones. A fade
of adjacent tones erases a burst of the codeword, and without a
permutation, losing half of the tones leaves two codewords alike. Like
permute() in the modem, index 0 stays and the rest follow the
XorShiftMask with BITS bits. For each burst length L, d(L) is the least
Hamming distance between two codewords on the tones outside any burst
of length L. The longest burst that still leaves d(L) above zero can't
exceed N - BITS - 1, as on any BITS positions some nonzero codeword is
zero. All triples with full period are listed,
the longest burst first, then the largest sum of d(L).

Copyright 2026 Ahmet Inan <inan@aicodix.de>
*/

#include <vector>
#include <cstdlib>
#include <iostream>
#include <algorithm>

int bits, mask, N;

int next(int y, int a, int b, int c)
{
	y ^= y << a;
	y &= mask;
	y ^= y >> b;
	y ^= y << c;
	y &= mask;
	return y;
}

bool full_period(int a, int b, int c)
{
	int y = 1;
	for (int i = mask; i; --i) {
		y = next(y, a, b, c);
		if ((i == 1) != (y == 1))
			return false;
	}
	return true;
}

struct Triple
{
	int a, b, c;
	int burst, sum;
	std::vector<int> d;
};

// p[t] is the code bit on tone t
Triple evaluate(int a, int b, int c, const std::vector<int> &p)
{
	Triple t = { a, b, c, 0, 0, std::vector<int>(N, N) };
	std::vector<int> sum(N + 1);
	// the code is linear, so the distances are the weights of the nonzero codewords outside the burst:
	// those of codeword m, of its complement and of the all ones codeword, whose weight outside is N - L
	for (int m = 1; m < N; ++m) {
		for (int i = 0; i < N; ++i)
			sum[i+1] = sum[i] + __builtin_parity(m & p[i]);
		for (int L = 1; L < N; ++L) {
			int most = 0, least = L;
			for (int s = 0; s + L <= N; ++s) {
				most = std::max(most, sum[s+L] - sum[s]);
				least = std::min(least, sum[s+L] - sum[s]);
			}
			int outside = std::min(sum[N] - most, N - L - sum[N] + least);
			t.d[L] = std::min(t.d[L], std::min(outside, N - L));
		}
	}
	for (int L = 1; L < N; ++L) {
		t.sum += t.d[L];
		if (t.d[L])
			t.burst = L;
	}
	return t;
}

std::vector<int> sequence(int a, int b, int c)
{
	std::vector<int> p(N);
	for (int i = 1, y = 1; i < N; ++i)
		p[i] = y = next(y, a, b, c);
	return p;
}

void print(const Triple &t)
{
	std::cout << bits << " " << t.a << " " << t.b << " " << t.c << " burst " << t.burst << " sum " << t.sum << " d";
	for (int i = 1; i < 8; ++i)
		std::cout << " " << t.d[i*N/8];
	std::cout << std::endl;
}

int main(int argc, char **argv)
{
	if (argc != 2 && argc != 3 && argc != 5) {
		std::cerr << "usage: " << argv[0] << " BITS [COUNT]" << std::endl;
		std::cerr << "       " << argv[0] << " BITS FIRST SECOND THIRD" << std::endl;
		return 1;
	}
	bits = std::atoi(argv[1]);
	if (bits < 3 || bits > 10) {
		std::cerr << "BITS must be between 3 and 10" << std::endl;
		return 1;
	}
	mask = (1 << bits) - 1;
	N = 1 << bits;
	std::vector<int> identity(N);
	for (int i = 0; i < N; ++i)
		identity[i] = i;
	Triple none = evaluate(0, 0, 0, identity);
	std::cerr << "without permutation: burst " << none.burst << " sum " << none.sum << std::endl;
	std::cerr << "d(L) listed for L = N / 8 to 7 N / 8" << std::endl;
	std::vector<Triple> list;
	int count = 10;
	if (argc == 5) {
		int a = std::atoi(argv[2]), b = std::atoi(argv[3]), c = std::atoi(argv[4]);
		if (a < 1 || a >= bits || b < 1 || b >= bits || c < 1 || c >= bits) {
			std::cerr << "FIRST, SECOND and THIRD must be between 1 and " << bits - 1 << std::endl;
			return 1;
		}
		if (!full_period(a, b, c))
			std::cerr << "no full period" << std::endl;
		list.push_back(evaluate(a, b, c, sequence(a, b, c)));
	} else {
		if (argc == 3)
			count = std::atoi(argv[2]);
		for (int a = 1; a < bits; ++a)
			for (int b = 1; b < bits; ++b)
				for (int c = 1; c < bits; ++c)
					if (full_period(a, b, c))
						list.push_back(evaluate(a, b, c, sequence(a, b, c)));
		std::stable_sort(list.begin(), list.end(), [](const Triple &x, const Triple &y) {
			return x.burst != y.burst ? x.burst > y.burst : x.sum > y.sum; });
		std::cerr << list.size() << " triples with full period" << std::endl;
	}
	for (int i = 0; i < (int)list.size() && i < count; ++i)
		print(list[i]);
	return 0;
}

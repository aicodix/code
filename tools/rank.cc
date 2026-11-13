/*
Rank xorshift permutations by how evenly neighbours spread

XorShiftMask with BITS bits is a linear map over GF(2), so the indices it
visits are linear functions of each other. For each lag l from 1 to LAGS
and each g from 4 up to BITS / 2, take the low g bits of an index together
with the low g bits of the index l steps later, and the same for the top g
bits. If these 2 g functions are linearly dependent, the pairs of indices
crowd into a fraction of the g x g grid, which shows as clusters when
plotting them. The deficiency is the missing rank, summed over all lags
and both ends of the index. All triples with full period are listed, the
least deficient first, comparing small g before large g.

Copyright 2026 Ahmet Inan <inan@aicodix.de>
*/

#include <vector>
#include <cstdlib>
#include <iostream>
#include <algorithm>

const int LAGS = 8, MIN_G = 4, MAX_G = 8;
int bits, mask;

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

int rank(std::vector<unsigned> rows, int cols)
{
	int r = 0;
	for (int col = 0; col < cols; ++col) {
		int pivot = r;
		while (pivot < (int)rows.size() && !((rows[pivot] >> col) & 1))
			++pivot;
		if (pivot == (int)rows.size())
			continue;
		std::swap(rows[r], rows[pivot]);
		for (int i = 0; i < (int)rows.size(); ++i)
			if (i != r && ((rows[i] >> col) & 1))
				rows[i] ^= rows[r];
		++r;
	}
	return r;
}

struct Triple
{
	int a, b, c;
	int deficiency[MAX_G+1];
};

Triple evaluate(int a, int b, int c)
{
	Triple t = { a, b, c, {} };
	for (int g = MIN_G; g <= MAX_G && 2 * g <= bits; ++g) {
		for (int l = 1; l <= LAGS; ++l) {
			for (int top = 0; top < 2; ++top) {
				// row j: which of the 2 g functions depend on bit j of the index
				std::vector<unsigned> rows(bits);
				for (int j = 0; j < bits; ++j) {
					int y = 1 << j, z = y;
					for (int k = 0; k < l; ++k)
						z = next(z, a, b, c);
					unsigned now = top ? y >> (bits - g) : y & ((1 << g) - 1);
					unsigned later = top ? z >> (bits - g) : z & ((1 << g) - 1);
					rows[j] = now | later << g;
				}
				t.deficiency[g] += 2 * g - rank(rows, 2 * g);
			}
		}
	}
	return t;
}

int main(int argc, char **argv)
{
	if (argc != 2 && argc != 3 && argc != 5) {
		std::cerr << "usage: " << argv[0] << " BITS [COUNT]" << std::endl;
		std::cerr << "       " << argv[0] << " BITS FIRST SECOND THIRD" << std::endl;
		return 1;
	}
	bits = std::atoi(argv[1]);
	if (bits < 2 * MIN_G || bits > 24) {
		std::cerr << "BITS must be between " << 2 * MIN_G << " and 24" << std::endl;
		return 1;
	}
	mask = (1 << bits) - 1;
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
		list.push_back(evaluate(a, b, c));
	} else {
		if (argc == 3)
			count = std::atoi(argv[2]);
		for (int a = 1; a < bits; ++a)
			for (int b = 1; b < bits; ++b)
				for (int c = 1; c < bits; ++c)
					if (full_period(a, b, c))
						list.push_back(evaluate(a, b, c));
		std::stable_sort(list.begin(), list.end(), [](const Triple &x, const Triple &y) {
			return std::lexicographical_compare(x.deficiency + MIN_G, x.deficiency + MAX_G + 1,
				y.deficiency + MIN_G, y.deficiency + MAX_G + 1); });
		std::cerr << list.size() << " triples with full period" << std::endl;
	}
	for (int i = 0; i < (int)list.size() && i < count; ++i) {
		std::cout << bits << " " << list[i].a << " " << list[i].b << " " << list[i].c << " deficiency";
		for (int g = MIN_G; g <= MAX_G && 2 * g <= bits; ++g)
			std::cout << " " << list[i].deficiency[g];
		std::cout << std::endl;
	}
	return 0;
}

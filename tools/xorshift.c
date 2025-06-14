/*
Exhaustive search of xorshift parameters

Copyright 2025 Ahmet Inan <inan@aicodix.de>
*/

#include <stdio.h>
#include <stdlib.h>

int next(int y, int a, int b, int c, int mask) {
	y ^= y << a;
	y &= mask;
	y ^= y >> b;
	y ^= y << c;
	y &= mask;
	return y;
}

int main(int argc, char **argv) {
	if (argc != 2 && argc != 5)
		return 1;
	int bits = atoi(argv[1]);
	int mask = (1 << bits) - 1;
	int seed = 1;
	if (argc == 5) {
		int a = atoi(argv[2]);
		int b = atoi(argv[3]);
		int c = atoi(argv[4]);
		for (int y = seed, i = mask; i; --i) {
			printf("%d\n", y);
			y = next(y, a, b, c, mask);
		}
		return 0;
	}
	for (int a = 1; a < bits; ++a) {
		for (int b = 1; b < bits; ++b) {
			for (int c = 1; c < bits; ++c) {
				int good = 1;
				for (int y = seed, i = mask; good && i; --i) {
					y = next(y, a, b, c, mask);
					good &= (i == 1) == (y == seed);
				}
				if (good)
					printf("%d %d %d %d\n", bits, a, b, c);
			}
		}
	}
	return 0;
}

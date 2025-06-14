/*
Exhaustive search of xorshift parameters

Copyright 2025 Ahmet Inan <inan@aicodix.de>
*/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	if (argc != 2)
		return 1;
	int bits = atoi(argv[1]);
	int mask = (1 << bits) - 1;
	int seed = 1;
	for (int a = 1; a < bits; ++a) {
		for (int b = 1; b < bits; ++b) {
			for (int c = 1; c < bits; ++c) {
				int good = 1;
				for (int y = seed, i = mask; good && i; --i) {
					y ^= y << a;
					y &= mask;
					y ^= y >> b;
					y ^= y << c;
					y &= mask;
					good &= (i == 1) == (y == seed);
				}
				if (good)
					printf("%d %d %d\n", a, b, c);
			}
		}
	}
	return 0;
}

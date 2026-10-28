/*
Generate polar code reliability sequences

Copyright 2026 Ahmet Inan <inan@aicodix.de>
*/

#include <cmath>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include "polar_sequence.hh"

int main(int argc, char **argv)
{
	if (argc < 2 || argc > 3) {
		std::cerr << "usage: " << argv[0] << " LEVEL [BETA]" << std::endl;
		return 1;
	}
	const int MAX_M = 20;
	int level = std::atoi(argv[1]);
	if (level < 1 || level > MAX_M) {
		std::cerr << "LEVEL must be between 1 and " << MAX_M << std::endl;
		return 1;
	}
	double beta = std::pow(2., .25);
	if (argc == 3)
		beta = std::atof(argv[2]);
	if (!(beta >= 1)) {
		std::cerr << "BETA must be at least 1" << std::endl;
		return 1;
	}
	int length = 1 << level;
	auto construct = new CODE::PolarizationWeightSequence<MAX_M>;
	auto sequence = new int[length];
	(*construct)(sequence, level, beta);
	std::cout << "static const int sequence[" << length << "] = { ";
	for (int i = 0; i < length; ++i)
		std::cout << sequence[i] << ", ";
	std::cout << "};" << std::endl;
	delete[] sequence;
	delete construct;
	return 0;
}

/*
Test for the short BCH codes encoder

Copyright 2020 Ahmet Inan <inan@aicodix.de>
*/

#include <cassert>
#include <iostream>
#include "short_bch_code_encoder.hh"

int main()
{
	if (1) {
		// Perfect binary Golay code using x^11+x^9+x^7+x^6+x^5+x+1
		CODE::ShortBCHCodeEncoder<23, 12> encode(0b101011100011);
		int message = 0b101011011011;
		int target = 0b10101101101111011111001;
		int codeword = encode(message);
		assert(target == codeword);
	}
	if (1) {
		// Perfect binary Golay code using x^11+x^10+x^6+x^5+x^4+x^2+1
		CODE::ShortBCHCodeEncoder<23, 12> encode(0b110001110101);
		int message = 0b101011011011;
		int target = 0b10101101101100100010101;
		int codeword = encode(message);
		assert(target == codeword);
	}
	if (1) {
		// NASA INTRO BCH(15, 5) T=3
		CODE::ShortBCHCodeEncoder<15, 5> encode(0b10100110111);
		int message = 0b11001;
		int target = 0b110010001111010;
		int codeword = encode(message);
		assert(target == codeword);
	}
	std::cerr << "Short BCH code encoder test passed!" << std::endl;
	return 0;
}


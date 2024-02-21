/*
Test for the Cyclic redundancy check

Copyright 2018 Ahmet Inan <inan@aicodix.de>
*/

#include <cstdint>
#include <cassert>
#include <iostream>
#include <bitset>
#include "crc.hh"

int main()
{
	if (1) {
		CODE::CRC<uint32_t> crc(0xEDB88320, 0xFFFFFFFF);
		for (uint8_t c: std::string("Hello World!")) crc(c);
		assert(~crc() == 0x1C291CA3);
	}
	if (1) {
		// Optimization of cyclic redundancy-check codes with 24 and 32 parity bits
		// by G. Castagnoli, S. Brauer and M. Herrmann - 1993
		// Hamming distance of at least 6 up to a payload of 32736 bits.
		CODE::CRC<uint32_t> crc(0xC8DF352F);
		for (uint8_t c: std::string("Hello World!")) crc(c);
		assert(!crc(uint32_t(0xAB94FCAB)));
	}
	if (1) {
		// Optimization of cyclic redundancy-check codes with 24 and 32 parity bits
		// by G. Castagnoli, S. Brauer and M. Herrmann - 1993
		// Hamming distance of at least 5 up to a payload of 65505 bits.
		CODE::CRC<uint32_t> crc(0xD419CC15);
		for (uint8_t c: std::string("Hello World!")) crc(c);
		assert(!crc(uint32_t(0xECFE423E)));
	}
	if (1) {
		// Performance of cyclic redundancy codes for embedded networks
		// by T. Chakravarty and P. Koopman - 2001
		// Optimal for payloads <= 64 bits
		CODE::CRC<uint16_t> crc(0xA8F4);
		for (uint8_t c: std::string("Hello World!")) crc(c);
		assert(!crc(uint16_t(0x8FEF)));
		crc.reset(0x9B38);
		std::bitset<32> hw("00000010001001110111110100100100");
		for (size_t i = 0; i < hw.size(); ++i) crc(hw[i]);
		//std::cerr << "0x" << std::hex << crc() << std::endl;
		assert(crc() == 0x915D);
	}
	if (1) {
		CODE::CRC<uint8_t> crc(0x8C);
		for (uint8_t c: std::string("Hello World!")) crc(c);
		assert(!crc(uint8_t(0x9E)));
	}
	std::cerr << "Cyclic redundancy check test passed!" << std::endl;
	return 0;
}


/*
Test for the Cyclic redundancy check

Copyright 2018 Ahmet Inan <inan@aicodix.de>
*/

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


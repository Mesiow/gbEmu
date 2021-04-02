#pragma once
#include <cstdint>
#include <cstring>
#include <assert.h>
#include <string>

namespace gbEmu {
	using byte = uint8_t;
	using word = uint16_t;

	/*
		Return result of setting the bit of the value
	*/
	byte setBit(byte val, byte bitToSet);

	/*
		Return result of reseting the bit of the value
	*/
	byte resetBit(byte val, byte bitToReset);

	/*
		Tests whether a bit is 1 or 0 in val
		Returns 1 is On and 0 if off
	*/
	byte testBit(byte val, byte bitToTest);
}
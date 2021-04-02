#pragma once
#include <cstdint>
#include <cstring>
#include <assert.h>
#include <string>

namespace gbEmu {
	using u8 = uint8_t;
	using s8 = int8_t;
	using u16 = uint16_t;
	using s16 = int16_t;
	using u32 = uint32_t;
	using s32 = int32_t;

	/*
		Return result of setting the bit of the value
	*/
	u8 setBit(u8 val, u8 bitToSet);

	/*
		Return result of reseting the bit of the value
	*/
	u8 resetBit(u8 val, u8 bitToReset);

	/*
		Tests whether a bit is 1 or 0 in val
		Returns 1 is On and 0 if off
	*/
	u8 testBit(u8 val, u8 bitToTest);
}
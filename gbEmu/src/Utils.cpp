#include "..\include\Utils.h"

namespace gbEmu {

	byte setBit(byte val, byte bitToSet)
	{
		byte bit = (1 << bitToSet);
		val |= bit;
		return val;
	}

	byte resetBit(byte val, byte bitToReset)
	{
		byte bit = (1 << bitToReset);
		if ((val & bit) == 0) return val; //bit is already reset

		val &= ~(bit);
		return val;
	}

	byte testBit(byte val, byte bitToTest)
	{
		byte bit = (1 << bitToTest);
		return ((val & bit) > 0 ? 1 : 0);
	}
}

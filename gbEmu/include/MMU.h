#pragma once
#include "Utils.h"

namespace gbEmu {
#define MAX_MEM 0x10000

	struct MMU {
		MMU();

		void write(word address, byte value);
		byte read(word address);
		byte memory[MAX_MEM];
	};
}
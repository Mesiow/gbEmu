#pragma once
#include "Utils.h"
#include <fstream>
#include <iostream>

namespace gbEmu {
#define MAX_MEM 0x10000

	struct MMU {
		MMU();

		void loadRom(const std::string& file, bool isBootRom = false);
		void write(word address, byte value);
		byte read(word address);

		byte memory[MAX_MEM];
	};
}

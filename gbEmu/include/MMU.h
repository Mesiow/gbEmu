#pragma once
#include "Utils.h"
#include <fstream>
#include <iostream>

namespace gbEmu {
#define MAX_MEM 0x10000


	struct MMU {
		MMU();

		void loadRom(const std::string& file, bool isBootRom = false);
		void write(u16 address, u8 value);
		u8 read(u16 address);

		u8 memory[MAX_MEM];
		u8 bootrom[0x100];
	};
}

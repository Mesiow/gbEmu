#pragma once
#include "Utils.h"
#include <string>
#include <fstream>

namespace gbEmu {
	#define ROM_SIZE 0x8000
	#define MBC1_SIZE 0x200000

	struct Cartridge {
		Cartridge();
		~Cartridge();
		void load(const std::string& file);

		//If mbc1 is enabled, 2mb of memory is allocated (banking allows up to 2mb)
		u8* memory;
		u32 size;
		bool mbc1;
	};
}
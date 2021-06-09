#pragma once
#include "Utils.h"
#include <string>
#include <fstream>

namespace gbEmu {
	#define ROM_SIZE0 0x8000 //32KB
    #define ROM_SIZE4 0x10000 //64KB (4 banks)
	#define ROM_SIZE8 0x20000 //128KB (8 banks)
	#define ROM_SIZE16 0x40000 //256KB (16 banks)
	#define ROM_SIZE32 0x80000 //512KB (32 banks)
	#define MBC1_SIZE 0x200000
	#define MAX_RAM_BANKS 0x8000

	struct Cartridge {
		Cartridge();
		~Cartridge();
		void load(const std::string& file);

		u8* memory;
		u32 cartSize;
		u8 ramBanks[MAX_RAM_BANKS]; //up to 32KB (4 ram banks as cartridge ram)

		bool mbc1;
		bool battery;
	};
}
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
	#define ROM_SIZE64 0x100000 //1MB (64 banks)

	#define RAM_SIZE1 0x800 //2KB
	#define RAM_SIZE2 0x2000 //8KB
	#define RAM_SIZE3 0x8000 //32KB (4 ram banks)
	#define RAM_SIZE4 0x20000 //128KB (16 ram banks)
	#define RAM_SIZE5 0x10000 //64KB (8 ram banks)

	struct Cartridge {
		Cartridge();
		~Cartridge();
		void load(const std::string& file);
		void allocateCartMem(u8 type, u8 romSize);
		void allocateRam(u8 ramSize);

		u8* memory;
		u8* ramBanks;
		u32 cartSize;
		u32 ramSize;

		bool mbc1;
		bool mbc3;
		bool externalRam;
		bool battery;
	};
}
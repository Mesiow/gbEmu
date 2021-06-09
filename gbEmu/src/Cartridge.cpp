#include "../include/Cartridge.h"
#include <iostream>

namespace gbEmu {
	Cartridge::Cartridge()
	{
		cartSize = ROM_SIZE0;
		ramSize = 0x0;

		memory = nullptr;
		ramBanks = nullptr;
		externalRam = true;
		mbc1 = false;
		mbc3 = false;
	}

	Cartridge::~Cartridge()
	{
		if (memory != nullptr) delete[] memory;
		if (ramBanks != nullptr) delete[] ramBanks;
	}

	void Cartridge::load(const std::string& file)
	{
		std::ifstream rom(file, std::ios::binary | std::ios::ate);

		if (rom.is_open()) {
			u32 size = static_cast<u32>(rom.tellg());

			if (size > 0x200000) {
				std::cerr << "Cartridge size is too large.\n";
				return;
			}

			u8* buf = new u8[size];

			rom.seekg(0, std::ios::beg);
			rom.read((char*)buf, size);
			rom.close();

			//Check cart type (ROM only or MBC1 etc..)
			u8 type = buf[0x147];
			u8 rom_size = buf[0x148];
			u8 ram_size = buf[0x149];

			allocateCartMem(type, rom_size);
			allocateRam(ram_size);

			for (size_t i = 0; i < cartSize; ++i) {
				memory[i] = buf[i];
			}
			delete[] buf;

			std::cout << file << " cart loaded sucessfully\n" << std::endl;
		}
		else {
			std::cerr << file << " cart failed to open\n";
			return;
		}

	}

	void Cartridge::allocateCartMem(u8 type, u8 romSize)
	{
		//MBC1
		if (type == 0x01 || type == 0x02 || type == 0x03) {
			mbc1 = true;
		}
		//MBC3
		else if (type == 0x13) {
			mbc3 = true;
		}

		switch (romSize) {
			case 0x00: cartSize = ROM_SIZE0; break;
			case 0x01: cartSize = ROM_SIZE4; break;
			case 0x02: cartSize = ROM_SIZE8; break;
			case 0x03: cartSize = ROM_SIZE16; break;
			case 0x04: cartSize = ROM_SIZE32; break;
			case 0x05: cartSize = ROM_SIZE64; break;
		}
		memory = new u8[cartSize];
	}

	void Cartridge::allocateRam(u8 ramSize)
	{
		switch (ramSize) {
			case 0x00: externalRam = false; break;
			case 0x01: ramSize = RAM_SIZE1; break;
			case 0x02: ramSize = RAM_SIZE2; break;
			case 0x03: ramSize = RAM_SIZE3; break;
			case 0x04: ramSize = RAM_SIZE4; break;
			case 0x05: ramSize = RAM_SIZE5; break;
		}

		if(externalRam)
			ramBanks = new u8[ramSize];
	}
}
#include "../include/Cartridge.h"
#include <iostream>

namespace gbEmu {
	Cartridge::Cartridge()
	{
		memory = nullptr;
		std::memset(ramBanks, 0x00, MAX_RAM_BANKS);
		mbc1 = false;
	}

	Cartridge::~Cartridge()
	{
		if (memory != nullptr) delete[] memory;
	}

	void Cartridge::load(const std::string& file)
	{
		std::ifstream rom(file, std::ios::binary | std::ios::ate);

		if (rom.is_open()) {
			size = static_cast<u32>(rom.tellg());

			if (size > 0x200000) {
				std::cerr << "Cartridge size is too large.\n";
				return;
			}

			u8* buf = new u8[size];

			rom.seekg(0, std::ios::beg);
			rom.read((char*)buf, size);
			rom.close();

			//Check cart type (ROM only or MBC1)
			u8 type = buf[0x147];

			if (type == 0x0) {
				memory = new u8[ROM_SIZE];
			}
			//MBC1
			else if (type == 0x01 || type == 0x02 || type == 0x03) {
				mbc1 = true;
				memory = new u8[MBC1_SIZE];
			}
			
			for (size_t i = 0; i < size; ++i) {
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
}
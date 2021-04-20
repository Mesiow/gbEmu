#include "../include/Cartridge.h"
#include <iostream>

namespace gbEmu {
	Cartridge::Cartridge()
	{
		memory = nullptr;
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
			u32 size = static_cast<u32>(rom.tellg());
			u8* buf = new u8[size];

			rom.seekg(0, std::ios::beg);
			rom.read((char*)buf, size);
			rom.close();

			//Check cart type (ROM only or MBC)
			u8 type = buf[0x147];

			if (type == 0x00) {
				memory = new u8[ROM_SIZE];
				this->size = ROM_SIZE;
			}
			//MBC1
			else if (type == 0x01) {
				mbc1 = true;
				memory = new u8[MBC1_SIZE];
				this->size = MBC1_SIZE;
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
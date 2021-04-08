#include "../include/MMU.h"

namespace gbEmu {

	MMU::MMU()
	{
		std::memset(memory, 0x00, MAX_MEM);
	}

	void MMU::loadRom(const std::string& file, bool isBootRom)
	{
		const std::string path = "roms/" + file;
		std::ifstream rom(path, std::ios::binary | std::ios::ate);

		
		if (rom.is_open()) {
			u32 size = static_cast<u32>(rom.tellg());
			u8* buf = new u8[size];

			rom.seekg(0, std::ios::beg);
			rom.read((char*)buf, size);
			rom.close();

			if (isBootRom) {		
				//Load bootrom into memory
				for (size_t i = 0; i < size; ++i) {
					bootrom[i] = buf[i];
				}
			}
			else {
				//Load into normal memory
				for (size_t i = 0; i < size; ++i) {
					memory[i] = buf[i];
				}
			}
			delete[] buf;

			std::cout << file << " rom loaded sucessfully\n" << std::endl;
		}
		else {
			std::cerr << file << " rom failed to open\n";
			return;
		}


	}

	void MMU::write(u16 address, u8 value)
	{
		assert(address >= 0 && address < MAX_MEM);
		memory[address] = value;
	}

	u8 MMU::read(u16 address)
	{
		assert(address >= 0 && address < MAX_MEM);
		return memory[address];
	}
}

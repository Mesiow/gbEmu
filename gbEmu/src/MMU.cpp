#include "../include/MMU.h"

namespace gbEmu {

	MMU::MMU()
	{
		std::memset(memory, 0x00, MAX_MEM);
	}

	void MMU::write(word address, byte value)
	{
		assert(address >= 0 && address < MAX_MEM);
		memory[address] = value;
	}

	byte MMU::read(word address)
	{
		assert(address >= 0 && address < MAX_MEM);
		return memory[address];
	}
}

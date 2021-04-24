#include "../include/MMU.h"
#include "../include/Cartridge.h"

namespace gbEmu {

	MMU::MMU()
	{
		cart = nullptr;
		std::memset(memory, 0x00, MAX_MEM);
		std::memset(bootrom, 0x0, 0x100);
	}

	void MMU::loadTestRom(const std::string& file)
	{
		std::ifstream rom(file, std::ios::binary | std::ios::ate);

		if (rom.is_open()) {
			u32 size = static_cast<u32>(rom.tellg());
			u8* buf = new u8[size];

			rom.seekg(0, std::ios::beg);
			rom.read((char*)buf, size);
			rom.close();

			for (size_t i = 0; i < size; ++i) {
				memory[i] = buf[i];
			}
			delete[] buf;
			std::cout << "Test " << file << " loaded\n";
		}
	}

	void MMU::loadBios(const std::string& file)
	{
		std::ifstream rom(file, std::ios::binary | std::ios::ate);

		if (rom.is_open()) {
			u32 size = static_cast<u32>(rom.tellg());
			u8* buf = new u8[size];

			rom.seekg(0, std::ios::beg);
			rom.read((char*)buf, size);
			rom.close();

			bootRomEnabled = true;
			//Load bootrom into memory
			for (size_t i = 0; i < size; ++i) {
				bootrom[i] = buf[i];
			}
			delete[] buf;
		}
	}

	void MMU::loadCartridge(Cartridge* cart)
	{
		this->cart = cart;

		u32 size = this->cart->size;
		u8 *cart_mem = this->cart->memory;

		for (size_t i = 0; i < size; ++i) {
			memory[i] = cart_mem[i];
		}
	}

	void MMU::write(u16 address, u8 value)
	{
		assert(address >= 0 && address < MAX_MEM);

		//Disable bootrom
		if (address == 0xFF50 && bootRomEnabled) {
			bootRomEnabled = false;
		}

		//Read character from serial
		if (address == 0xFF02 && value == 0x81) {
			std::cout << read(0xFF01);
			memory[0xFF01] = 0xFF;
		}


		//0x0000 - 0x3FFF (ROM) - not writeable
		//0x4000 - 0x7FFF (Switchable ROM Bank) - not writeable
		if (address >= 0x0000 && address <= 0x7FFF) {
			//If game tries to write to rom,
			//We need to figure out why and change banks accordingly
			handleBanking(address, value);
			return;
		}

		//Writing here also mirrors into WRAM
		else if (address >= 0xE000 && address < 0xFE00) {
			memory[address] = value;
			memory[address - 0x2000] = value;
		}
		
		//Not usable 0xFEA0 - 0xFEFF
		else if (address >= 0xFEA0 && address <= 0xFEFF)
			return;
		
		else {
			memory[address] = value;
		}

	}

	u8 MMU::read(u16 address)
	{
		assert(address >= 0 && address < MAX_MEM);

		if (address < 0x0100 && bootRomEnabled) {
			return bootrom[address];
		}

		//Switchable rom bank
		if (address >= 0x4000 && address <= 0x7FFF) {
			//Access specific bank based on currentRomBank value
			u16 newaddr = address - 0x4000;
			return cart->memory[newaddr + (currentRomBank * 0x4000)];
		}
		else if (address >= 0xA000 && address <= 0xBFFF) {
			//Access specific ram bank based on currentRomBank value
			u16 newaddr = address - 0xA000;
			return cart->ramBanks[newaddr + (currentRamBank * 0x2000)];
		}

		return memory[address];
	}
	void MMU::handleBanking(u16 address, u8 value)
	{
		//If address is between 0x0 and 0x2000, then it enables RAM 
		//bank writing
		if (address < 0x2000) {
			if (cart->mbc1) {
				enableRamBank(address, value);
			}
		}

		//Depending on the mem address where its trying to write 
		//to rom, we need to take action.
		//If address is between 0x2000 and 0x4000, it is a ROM bank change.

		else if (address >= 0x2000 && address <= 0x3FFF) {

		}

		//If the address is between 0x4000 and 0x6000, it is a RAM bank change
		//or a ROM bank change depending on what current ROM/RAM mode is selected.

		else if (address >= 0x4000 && address <= 0x5FFF) {

		}	
	}

	void MMU::enableRamBank(u16 address, u8 value)
	{
		//If the lower nibble of the data the game is writing to mem
		//is 0xA, then ram bank writing is enabled.
		//Else, if the lower nibble is 0, then ram bank writing is disabled.

		u8 nibble = value & 0xF;
		if (nibble == 0xA)
			ramBanking = true;
		else if (nibble == 0x0)
			ramBanking = false;
	}
}

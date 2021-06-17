#include "../include/MMU.h"
#include "../include/Cartridge.h"
#include "../include/Joypad.h"

namespace gbEmu {

	MMU::MMU()
	{
		cart = nullptr;
		std::memset(memory, 0x00, MAX_MEM);
		std::memset(bootrom, 0x0, 0x100);
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
			for (size_t i = 0; i < size; ++i) {
				bootrom[i] = buf[i];
			}
			delete[] buf;
		}
	}

	void MMU::loadCartridge(Cartridge* cart)
	{
		this->cart = cart;
		u8 *cart_mem = this->cart->memory;
		
		for (size_t i = 0; i < ROM_SIZE0; ++i) {
			memory[i] = cart_mem[i];
		}

		calcZeroBankNumber();
	}

	void MMU::write(u16 address, u8 value)
	{
		assert(address >= 0 && address < MAX_MEM);

		//Disable bootrom
		if (address == 0xFF50 && bootRomEnabled) {
			bootRomEnabled = false;
			memory[address] = value;

			return;
		}

		//Read character from serial
		if (address == 0xFF02 && value == 0x81) {
			std::cout << read(0xFF01);
			memory[0xFF01] = 0xFF;

			return;
		}

		////0x0000 - 0x3FFF (ROM) - not writeable
		////0x4000 - 0x7FFF (Switchable ROM Bank) - not writeable
		if (address >= 0x0000 && address <= 0x7FFF) {
			//If game tries to write to rom,
			//We need to figure out why and change banks accordingly
			handleBanking(address, value);
		}
		
		//external cartridge ram
		else if (address >= 0xA000 && address <= 0xBFFF) {
			if (ramBanking) {
				writeRamBank(address, value);
			}
		}

		//Writing here also mirrors into WRAM
		else if (address >= 0xE000 && address < 0xFE00) {
			memory[address] = value;
			memory[address - 0x2000] = value;
		}
		
		////Not usable 0xFEA0 - 0xFEFF
		else if (address >= 0xFEA0 && address <= 0xFEFF)
			return;

		else if (address == 0xFF44) {
			memory[address] = 0x00;
		}

		//DMA
		else if (address == 0xFF46) {
			dmaTransfer(value);
		}
		
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

		if (address == 0xFF00) {
			return joy->joypadState();
		}

		if ((address >= 0x0000 && address <= 0x3FFF) && romBanking) {
			u16 newaddr = (0x4000 * zeroBankNum) + address;
			return cart->memory[newaddr];
		}

		////Switchable rom bank
		if (address >= 0x4000 && address <= 0x7FFF) {
			//Access specific bank based on currentRomBank value
			u16 newaddr = address - 0x4000;
			return cart->memory[newaddr + (currentRomBank * 0x4000)];
		}
		else if (address >= 0xA000 && address <= 0xBFFF) {
			if (ramBanking) {
				return readRamBank(address);
			}
			else
				return 0xFF;
		}

		return memory[address];
	}

	void MMU::connectJoypad(Joypad* joy)
	{
		this->joy = joy;
	}

	void MMU::handleBanking(u16 address, u8 value)
	{
		//Depending on the mem address where its trying to write 
		//to rom, we need to take action.

		//If address is between 0x0 and 0x2000, then it enables RAM 
		//bank writing
		if (address <= 0x1FFF) {
			if (cart->mbc1 || cart->mbc3) {
				enableRamBank(address, value);
			}
		}

		//If address is between 0x2000 and 0x4000, it is a ROM bank change.
		else if (address >= 0x2000 && address <= 0x3FFF) {
			if (cart->mbc1 || cart->mbc3) {
				switchLoRomBank(value);
			}
		}

		//If the address is between 0x4000 and 0x6000, it is a RAM bank change
		//or a ROM bank change depending on what current ROM/RAM mode is selected.

		else if (address >= 0x4000 && address <= 0x5FFF) {
			//No rambank in mbc2, so always use rambank 0
			if (cart->mbc1 || cart->mbc3) {
				if (romBanking) {
					//If game tries to write to 0x4000 - 0x5FFF then it changes
					//bits 5 and 6  of the current rom bank but not bits 0 - 4

					//Switch hi rom bank
					switchHiRomBank(value);
				}
				else {
					//Game writing to 0x4000 - 0x5FFF,
					//change ram bank
					switchRamBank(value);
				}
			}
		}	

		//This will change whether we are doing rom banking
		//or ram banking with the above if statement
		else if (address >= 0x6000 && address <= 0x7FFF) {
			if (cart->mbc1 || cart->mbc3);
				switchRomRamMode(value);
		}
	}

	void MMU::enableRamBank(u16 address, u8 value)
	{
		//If the lower nibble of the data the game is writing to mem
		//is 0xA, then ram bank writing is enabled.
		//Else, if the lower nibble is 0, then ram bank writing is disabled.


		//handle MBC2 here
		
		
		//MBC1
		u8 nibble = value & 0xF;
		if (nibble == 0xA)
			ramBanking = true;
		else
			ramBanking = false;
	}

	void MMU::writeRamBank(u16 address, u8 value)
	{
		//If ram is 2kb or 8kb address = (address - 0xA000) % ramSize
		if (cart->ramSize == RAM_SIZE1 || cart->ramSize == RAM_SIZE2) {
			u16 addr = (address - 0xA000) % cart->ramSize;
			cart->ramBanks[addr] = value;
		}
		else if (cart->ramSize == RAM_SIZE3 && romBanking) {
			u16 addr = address - 0xA000;
			cart->ramBanks[addr + (currentRamBank * 0x2000)] = value;
		}
		else {
			//Mode flag is 0
			u16 addr = (address - 0xA000);
			cart->ramBanks[addr] = value;
		}
	}

	u8 MMU::readRamBank(u16 address)
	{
		if (cart->ramSize == RAM_SIZE1 || cart->ramSize == RAM_SIZE2) {
			u16 addr = (address - 0xA000) % cart->ramSize;
		}
		else if (cart->ramSize == RAM_SIZE3 && romBanking) {
			u16 addr = address - 0xA000;
			return cart->ramBanks[addr + (currentRamBank * 0x2000)];
		}
		else {
			u16 addr = (address - 0xA000);
			return cart->ramBanks[addr];
		}
	}

	void MMU::switchLoRomBank(u8 value)
	{

		if (cart->mbc1) {
			//MBC1

			//And value with associated bitmask depending on
			//rom size
			u8 mask = 0xFF;
			switch (cart->cartSize) {
				case ROM_SIZE0: mask = 0x1; break;
				case ROM_SIZE4: mask = 0x3; break;
				case ROM_SIZE8: mask = 0x7; break;
				case ROM_SIZE16: mask = 0xF; break;
				case ROM_SIZE32: mask = 0x1F; break;
			}

			//AND with bitmask
			u8 result = value & mask;

			//Clear lower 5 bits
			currentRomBank &= 0xE0;
			currentRomBank |= result;

			if (value == 0x00)
				currentRomBank = 1;
		}
		else if (cart->mbc3) {
			if (value == 0x00)
				currentRomBank = 1;
			else
				currentRomBank = value & 0x7F;
		}
	}

	void MMU::switchHiRomBank(u8 value)
	{
		if (cart->mbc1) {
			//clear first 5 bits
			u8 hi = value & 0xE0;

			//Clear bits 5 - 7
			currentRomBank &= 0x1F;
			currentRomBank |= hi;

			if (currentRomBank == 0)
				currentRomBank++;
		}
		else if (cart->mbc3) {
			/*
				If the written value is in the range $08 - $0C, the corresponding 
				RTC register is mapped to the 
				External RAM region ($A000 - $BFFF) instead
			*/
		}
	}

	void MMU::switchRamBank(u8 value)
	{
		currentRamBank = value & 0x3;
	}

	void MMU::switchRomRamMode(u8 value)
	{
		//If lsb of data writing here is 0, rom banking will be enabled
		//otherwise there will be a ram bank change

		u8 lsb = value & 0x1;
		romBanking = (lsb == 0) ? true : false;

		//Set current ram bank to 0 whenever romBanking is true because
		//the gameboy can only use rambank 0 in this mode
		if (romBanking) currentRamBank = 0;
	}

	void MMU::dmaTransfer(u8 value)
	{
		//Destination of DMA is the sprite RAM between 0xFE00 and 0xFE9F
		//Which means a total of 0xA0 bytes will be copied to this area.
		
		//The source address is the data being written to 0xFF46, except
		//this vale is the source address divided by 100, so to get the correct
		//address we multiply it by 100(or shift left by 8)

		u16 address = value << 8;
		for (size_t i = 0; i < 0xA0; ++i)
			write(0xFE00 + i, read(address + i));
	}

	void MMU::calcZeroBankNumber()
	{
		if (cart->cartSize <= ROM_SIZE32) {
			zeroBankNum = 0;
		}
	}
}

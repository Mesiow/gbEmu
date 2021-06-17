#pragma once
#include "Utils.h"
#include <fstream>
#include <iostream>

/*
	DMG Memory Map
	
Start	End		Description						Notes
==========================================================================================
0000	3FFF	16KB ROM bank 00				From cartridge, usually a fixed bank

4000	7FFF	16KB ROM Bank 01~NN				From cartridge, switchable bank via MBC (if any)

8000	9FFF	8KB Video RAM (VRAM)			Only bank 0 in Non-CGB mode
												Switchable bank 0/1 in CGB mode

A000	BFFF	8KB External RAM				In cartridge, switchable bank if any

C000	CFFF	4KB Work RAM (WRAM)bank 0	

D000	DFFF	4KB Work RAM (WRAM)				bank 1~N	Only bank 1 in Non-CGB mode
												Switchable bank 1~7 in CGB mode

E000	FDFF	Mirror of C000~DDFF (ECHO RAM)	Typically not used
FE00	FE9F	Sprite attribute table (OAM)	
FEA0	FEFF	Not Usable	
FF00	FF7F	I/O Registers	
FF80	FFFE	High RAM (HRAM)	
FFFF	FFFF	Interrupts Enable Register (IE)	

==========================================================================================

*/

namespace gbEmu {
	#define MAX_MEM 0x10000

	struct Cartridge;
	struct Joypad;
	struct MMU {
		MMU();

		void loadBios(const std::string& file);
		void loadCartridge(Cartridge* cart);

		void write(u16 address, u8 value);
		u8 read(u16 address);

		void connectJoypad(Joypad* joy);

		void handleBanking(u16 address, u8 value);
		void enableRamBank(u16 address, u8 value);

		void writeRamBank(u16 address, u8 value);
		u8 readRamBank(u16 address);

		//Two methods a game uses to change rom banks
		void switchLoRomBank(u8 value);
		void switchHiRomBank(u8 value);

		void switchRamBank(u8 value);
		void switchRomRamMode(u8 value);

		void dmaTransfer(u8 value);

		void calcZeroBankNumber();

		u8 memory[MAX_MEM];
		u8 bootrom[0x100];


		Cartridge* cart;
		u8 currentRomBank = 1;
		u8 currentRamBank = 0;
		u8 zeroBankNum = 0;

		bool romBanking = false;
		bool ramBanking = false;
		bool bootRomEnabled = false;

		Joypad* joy;
	};
}

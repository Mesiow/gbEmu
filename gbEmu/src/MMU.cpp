#include "../include/MMU.h"

namespace gbEmu {

	MMU::MMU()
	{
		std::memset(memory, 0x00, MAX_MEM);
	}

	void MMU::loadRom(const std::string& file, bool isBootRom)
	{
		std::ifstream rom(file, std::ios::binary | std::ios::ate);

		if (rom.is_open()) {
			u32 size = static_cast<u32>(rom.tellg());
			u8* buf = new u8[size];

			rom.seekg(0, std::ios::beg);
			rom.read((char*)buf, size);
			rom.close();

			if (isBootRom) {
				bootRomEnabled = true;
				//Load bootrom into memory
				for (size_t i = 0; i < size; ++i) {
					bootrom[i] = buf[i];
				}

				for (size_t i = 0; i < size; i++) {
					memory[i] = bootrom[i];
				}

				//Load scrolling Nintendo graphic into memory
				/*
					scrolling nintengo graphic
					CE ED 66 66 CC 0D 00 0B 03 73 00 83 00 0C 00 0D
					00 08 11 1F 88 89 00 0E DC CC 6E E6 DD DD D9 99
					BB BB 67 63 6E 0E EC CC DD DC 99 9F BB B9 33 3E
				*/
				u8 graphic[0x30];
				graphic[0] = 0xCE; graphic[1] = 0xED; graphic[2] = 0x66;
				graphic[3] = 0x66; graphic[4] = 0xCC; graphic[5] = 0x0D;
				graphic[6] = 0x00; graphic[7] = 0x0B; graphic[8] = 0x03;
				graphic[9] = 0x73; graphic[10] = 0x00; graphic[11] = 0x83;
				graphic[12] = 0x00; graphic[13] = 0x0C; graphic[14] = 0x00;
				graphic[15] = 0x0D; graphic[16] = 0x00; graphic[17] = 0x08;
				graphic[18] = 0x11; graphic[19] = 0x1F; graphic[20] = 0x88;
				graphic[21] = 0x89; graphic[22] = 0x00; graphic[23] = 0x0E;
				graphic[24] = 0xDC; graphic[25] = 0xCC; graphic[26] = 0x6E;
				graphic[27] = 0xE6; graphic[28] = 0xDD; graphic[29] = 0xDD;
				graphic[30] = 0xD9; graphic[31] = 0x99; graphic[32] = 0xBB;
				graphic[33] = 0xBB; graphic[34] = 0x67; graphic[35] = 0x63;
				graphic[36] = 0x6E; graphic[37] = 0x0E; graphic[38] = 0xEC;
				graphic[39] = 0xCC; graphic[40] = 0xDD; graphic[41] = 0xDC;
				graphic[42] = 0x99; graphic[43] = 0x9F; graphic[44] = 0xBB;
				graphic[45] = 0xB9; graphic[46] = 0x33; graphic[47] = 0x3E;

				//Load nintendo graphic
				for (size_t i = 0; i < 0x30; ++i) {
					memory[0x104 + i] = graphic[i];
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

		if (address == 0xFF02 && value == 0x81) {
			std::cout << (int)read(0xFF01);
		}
		memory[address] = value;
	}

	u8 MMU::read(u16 address)
	{
		assert(address >= 0 && address < MAX_MEM);
		return memory[address];
	}
}

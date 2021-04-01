#include "../include/Cpu.h"

namespace gbEmu {
	void Cpu::mapOpcodes() {
		/*
			Opcode table setup
		*/
		table.resize(0x100);
		table[0x00] = { "NOP", bind(&Cpu::op0x00), 4 }; table[0x01] = { "LD", bind(&Cpu::op0x01), 12 };
		table[0x02] = { "LD", bind(&Cpu::op0x02), 8 }; table[0x03] = { "INC", bind(&Cpu::op0x03), 8 };
		table[0x04] = { "INC", bind(&Cpu::op0x04), 4 }; table[0x05] = { "DEC", bind(&Cpu::op0x05), 4 };
		table[0x06] = { "LD", bind(&Cpu::op0x06), 8 }; table[0x07] = { "RLCA", bind(&Cpu::op0x07), 5 };
	}
}
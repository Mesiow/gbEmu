#include "../include/Cpu.h"

namespace gbEmu {
	void Cpu::mapOpcodes() {
		/*
			Opcode table setup
		*/
		table.resize(0x100);

		/* First row */
		table[0x00] = { "NOP", bind(&Cpu::op0x00), 4 }; table[0x01] = { "LD", bind(&Cpu::op0x01), 12 };
		table[0x02] = { "LD", bind(&Cpu::op0x02), 8 }; table[0x03] = { "INC", bind(&Cpu::op0x03), 8 };
		table[0x04] = { "INC", bind(&Cpu::op0x04), 4 }; table[0x05] = { "DEC", bind(&Cpu::op0x05), 4 };
		table[0x06] = { "LD", bind(&Cpu::op0x06), 8 }; table[0x07] = { "RLCA", bind(&Cpu::op0x07), 5 };
		table[0x08] = { "LD", bind(&Cpu::op0x08), 20 }; table[0x09] = { "ADD", bind(&Cpu::op0x09), 8 };
		table[0x0A] = { "LD", bind(&Cpu::op0x0A), 8 }; table[0x0B] = { "DEC", bind(&Cpu::op0x0B), 8 };
		table[0x0C] = { "INC", bind(&Cpu::op0x0C), 4 }; table[0x0D] = { "DEC", bind(&Cpu::op0x0D), 4 };
		table[0x0E] = { "LD", bind(&Cpu::op0x0E), 8 }; table[0x0F] = { "RRCA", bind(&Cpu::op0x0F), 4 };

		/* Second row */
		table[0x10] = { "STOP", bind(&Cpu::op0x10), 4 }; table[0x11] = { "LD", bind(&Cpu::op0x11), 12 };
		table[0x12] = { "LD", bind(&Cpu::op0x12), 8 }; table[0x13] = { "INC", bind(&Cpu::op0x13), 8 };
		table[0x14] = { "INC", bind(&Cpu::op0x14), 4 }; table[0x15] = { "DEC", bind(&Cpu::op0x15), 4 };
		table[0x16] = { "LD", bind(&Cpu::op0x16), 8 }; table[0x17] = { "RLA", bind(&Cpu::op0x17), 4 };
		table[0x18] = { "JR", bind(&Cpu::op0x18), 12 }; table[0x19] = { "ADD", bind(&Cpu::op0x19), 8 };
		table[0x1A] = { "LD", bind(&Cpu::op0x1A), 8 }; table[0x1B] = { "DEC", bind(&Cpu::op0x1B), 8 };
		table[0x1C] = { "INC", bind(&Cpu::op0x1C), 4 }; table[0x1D] = { "DEC", bind(&Cpu::op0x1D), 4 };
		table[0x1E] = { "LD", bind(&Cpu::op0x1E), 8 }; table[0x1F] = { "RRA", bind(&Cpu::op0x1F), 4 };

		/* Third row */
		table[0x20] = { "JR", bind(&Cpu::op0x20), 8 }; table[0x21] = { "LD", bind(&Cpu::op0x21), 12 };
		table[0x22] = { "LD", bind(&Cpu::op0x22), 8 }; table[0x23] = { "INC", bind(&Cpu::op0x23), 8 };
		table[0x24] = { "INC", bind(&Cpu::op0x24), 4 }; table[0x25] = { "DEC", bind(&Cpu::op0x25), 4 };
		table[0x26] = { "LD", bind(&Cpu::op0x26), 8 }; table[0x27] = { "DAA", bind(&Cpu::op0x27), 4 };
		table[0x28] = { "JR", bind(&Cpu::op0x28), 8 }; table[0x29] = { "ADD", bind(&Cpu::op0x29), 8 };
		table[0x2A] = { "LD", bind(&Cpu::op0x2A), 8 }; table[0x2B] = { "DEC", bind(&Cpu::op0x2B), 8 };
		table[0x2C] = { "INC", bind(&Cpu::op0x2C), 4 }; table[0x2D] = { "DEC", bind(&Cpu::op0x2D), 4 };
		table[0x2E] = { "LD", bind(&Cpu::op0x2E), 8 }; table[0x2F] = { "CPL", bind(&Cpu::op0x2F), 4 };
	}
}
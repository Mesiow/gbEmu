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

		/* Fourth row */
		table[0x30] = { "JR", bind(&Cpu::op0x30), 8 };  table[0x31] = { "LD", bind(&Cpu::op0x31), 12 };
		table[0x32] = { "LD", bind(&Cpu::op0x32), 8 };  table[0x33] = { "INC", bind(&Cpu::op0x33), 8 };
		table[0x34] = { "INC", bind(&Cpu::op0x34), 4 }; table[0x35] = { "DEC", bind(&Cpu::op0x35), 4 };
		table[0x36] = { "LD", bind(&Cpu::op0x36), 12 };  table[0x37] = { "SCF", bind(&Cpu::op0x37), 4 };
		table[0x38] = { "JR", bind(&Cpu::op0x38), 8 };  table[0x39] = { "ADD", bind(&Cpu::op0x39), 8 };
		table[0x3A] = { "LD", bind(&Cpu::op0x3A), 8 };  table[0x3B] = { "DEC", bind(&Cpu::op0x3B), 8 };
		table[0x3C] = { "INC", bind(&Cpu::op0x3C), 4 }; table[0x3D] = { "DEC", bind(&Cpu::op0x3D), 4 };
		table[0x3E] = { "LD", bind(&Cpu::op0x3E), 8 };  table[0x3F] = { "CCF", bind(&Cpu::op0x3F), 4 };

		/* Fifth row */
		table[0x40] = { "LD", bind(&Cpu::op0x40), 4 };  table[0x41] = { "LD", bind(&Cpu::op0x41), 4 };
		table[0x42] = { "LD", bind(&Cpu::op0x42), 4 };  table[0x43] = { "LD", bind(&Cpu::op0x43), 4 };
		table[0x44] = { "LD", bind(&Cpu::op0x44), 4 }; table[0x45] = { "LD", bind(&Cpu::op0x45), 4 };
		table[0x46] = { "LD", bind(&Cpu::op0x46), 8 }; table[0x47] = { "LD", bind(&Cpu::op0x47), 4 };
		table[0x48] = { "LD", bind(&Cpu::op0x48), 4 };  table[0x49] = { "LD", bind(&Cpu::op0x49), 4 };
		table[0x4A] = { "LD", bind(&Cpu::op0x4A), 4 };  table[0x4B] = { "LD", bind(&Cpu::op0x4B), 4 };
		table[0x4C] = { "LD", bind(&Cpu::op0x4C), 4 }; table[0x4D] = { "LD", bind(&Cpu::op0x4D), 4 };
		table[0x4E] = { "LD", bind(&Cpu::op0x4E), 8 };  table[0x4F] = { "LD", bind(&Cpu::op0x4F), 4 };

		/* Sixth row */
		table[0x50] = { "LD", bind(&Cpu::op0x50), 4 };  table[0x51] = { "LD", bind(&Cpu::op0x51), 4 };
		table[0x52] = { "LD", bind(&Cpu::op0x52), 4 };  table[0x53] = { "LD", bind(&Cpu::op0x53), 4 };
		table[0x54] = { "LD", bind(&Cpu::op0x54), 4 };  table[0x55] = { "LD", bind(&Cpu::op0x55), 4 };
		table[0x56] = { "LD", bind(&Cpu::op0x56), 8 };  table[0x57] = { "LD", bind(&Cpu::op0x57), 4 };
		table[0x58] = { "LD", bind(&Cpu::op0x58), 4 };  table[0x59] = { "LD", bind(&Cpu::op0x59), 4 };
		table[0x5A] = { "LD", bind(&Cpu::op0x5A), 4 };  table[0x5B] = { "LD", bind(&Cpu::op0x5B), 4 };
		table[0x5C] = { "LD", bind(&Cpu::op0x5C), 4 };  table[0x5D] = { "LD", bind(&Cpu::op0x5D), 4 };
		table[0x5E] = { "LD", bind(&Cpu::op0x5E), 8 };  table[0x5F] = { "LD", bind(&Cpu::op0x5F), 4 };

		/* Seventh row */
		table[0x60] = { "LD", bind(&Cpu::op0x60), 4 };  table[0x61] = { "LD", bind(&Cpu::op0x61), 4 };
		table[0x62] = { "LD", bind(&Cpu::op0x62), 4 };  table[0x63] = { "LD", bind(&Cpu::op0x63), 4 };
		table[0x64] = { "LD", bind(&Cpu::op0x64), 4 };  table[0x65] = { "LD", bind(&Cpu::op0x65), 4 };
		table[0x66] = { "LD", bind(&Cpu::op0x66), 8 };  table[0x67] = { "LD", bind(&Cpu::op0x67), 4 };
		table[0x68] = { "LD", bind(&Cpu::op0x68), 4 };  table[0x69] = { "LD", bind(&Cpu::op0x69), 4 };
		table[0x6A] = { "LD", bind(&Cpu::op0x6A), 4 };  table[0x6B] = { "LD", bind(&Cpu::op0x6B), 4 };
		table[0x6C] = { "LD", bind(&Cpu::op0x6C), 4 };  table[0x6D] = { "LD", bind(&Cpu::op0x6D), 4 };
		table[0x6E] = { "LD", bind(&Cpu::op0x6E), 8 };  table[0x6F] = { "LD", bind(&Cpu::op0x6F), 4 };

		/* Eighth row */
		table[0x70] = { "LD", bind(&Cpu::op0x70), 8 };  table[0x71] = { "LD", bind(&Cpu::op0x71), 8 };
		table[0x72] = { "LD", bind(&Cpu::op0x72), 8 };  table[0x73] = { "LD", bind(&Cpu::op0x73), 8 };
		table[0x74] = { "LD", bind(&Cpu::op0x74), 8 };  table[0x75] = { "LD", bind(&Cpu::op0x75), 8 };
		table[0x76] = { "HALT", bind(&Cpu::op0x76), 4 };  table[0x77] = { "LD", bind(&Cpu::op0x77), 8 };
		table[0x78] = { "LD", bind(&Cpu::op0x78), 4 };  table[0x79] = { "LD", bind(&Cpu::op0x79), 4 };
		table[0x7A] = { "LD", bind(&Cpu::op0x7A), 4 };  table[0x7B] = { "LD", bind(&Cpu::op0x7B), 4 };
		table[0x7C] = { "LD", bind(&Cpu::op0x7C), 4 };  table[0x7D] = { "LD", bind(&Cpu::op0x7D), 4 };
		table[0x7E] = { "LD", bind(&Cpu::op0x7E), 8 };  table[0x7F] = { "LD", bind(&Cpu::op0x7F), 4 };
	}
}
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

		/* Ninth row */
		table[0x80] = { "ADD", bind(&Cpu::op0x80), 4 };  table[0x81] = { "ADD", bind(&Cpu::op0x81), 4 };
		table[0x82] = { "ADD", bind(&Cpu::op0x82), 4 };  table[0x83] = { "ADD", bind(&Cpu::op0x83), 4 };
		table[0x84] = { "ADD", bind(&Cpu::op0x84), 4 };  table[0x85] = { "ADD", bind(&Cpu::op0x85), 4 };
		table[0x86] = { "ADD", bind(&Cpu::op0x86), 8 };table[0x87] = { "ADD", bind(&Cpu::op0x87), 4 };
		table[0x88] = { "ADC", bind(&Cpu::op0x88), 4 };  table[0x89] = { "ADC", bind(&Cpu::op0x89), 4 };
		table[0x8A] = { "ADC", bind(&Cpu::op0x8A), 4 };  table[0x8B] = { "ADC", bind(&Cpu::op0x8B), 4 };
		table[0x8C] = { "ADC", bind(&Cpu::op0x8C), 4 };  table[0x8D] = { "ADC", bind(&Cpu::op0x8D), 4 };
		table[0x8E] = { "ADC", bind(&Cpu::op0x8E), 8 };  table[0x8F] = { "ADC", bind(&Cpu::op0x8F), 4 };

		/* Tenth row */
		table[0x90] = { "SUB", bind(&Cpu::op0x90), 4 };  table[0x91] = { "SUB", bind(&Cpu::op0x91), 4 };
		table[0x92] = { "SUB", bind(&Cpu::op0x92), 4 };  table[0x93] = { "SUB", bind(&Cpu::op0x93), 4 };
		table[0x94] = { "SUB", bind(&Cpu::op0x94), 4 };  table[0x95] = { "SUB", bind(&Cpu::op0x95), 4 };
		table[0x96] = { "SUB", bind(&Cpu::op0x96), 8 };  table[0x97] = { "SUB", bind(&Cpu::op0x97), 4 };
		table[0x98] = { "SBC", bind(&Cpu::op0x98), 4 };  table[0x99] = { "SBC", bind(&Cpu::op0x99), 4 };
		table[0x9A] = { "SBC", bind(&Cpu::op0x9A), 4 };  table[0x9B] = { "SBC", bind(&Cpu::op0x9B), 4 };
		table[0x9C] = { "SBC", bind(&Cpu::op0x9C), 4 };  table[0x9D] = { "SBC", bind(&Cpu::op0x9D), 4 };
		table[0x9E] = { "SBC", bind(&Cpu::op0x9E), 8 };  table[0x9F] = { "SBC", bind(&Cpu::op0x9F), 4 };

		/* Tenth row */
		table[0xA0] = { "AND", bind(&Cpu::op0xA0), 4 };  table[0xA1] = { "AND", bind(&Cpu::op0xA1), 4 };
		table[0xA2] = { "AND", bind(&Cpu::op0xA2), 4 };  table[0xA3] = { "AND", bind(&Cpu::op0xA3), 4 };
		table[0xA4] = { "AND", bind(&Cpu::op0xA4), 4 };  table[0xA5] = { "AND", bind(&Cpu::op0xA5), 4 };
		table[0xA6] = { "AND", bind(&Cpu::op0xA6), 8 };  table[0xA7] = { "AND", bind(&Cpu::op0xA7), 4 };
		table[0xA8] = { "XOR", bind(&Cpu::op0xA8), 4 };  table[0xA9] = { "XOR", bind(&Cpu::op0xA9), 4 };
		table[0xAA] = { "XOR", bind(&Cpu::op0xAA), 4 };  table[0xAB] = { "XOR", bind(&Cpu::op0xAB), 4 };
		table[0xAC] = { "XOR", bind(&Cpu::op0xAC), 4 };  table[0xAD] = { "XOR", bind(&Cpu::op0xAD), 4 };
		table[0xAE] = { "XOR", bind(&Cpu::op0xAE), 8 };  table[0xAF] = { "XOR", bind(&Cpu::op0xAF), 4 };

		/* Eleventh row */
		table[0xB0] = { "OR", bind(&Cpu::op0xB0), 4 };  table[0xB1] = { "OR", bind(&Cpu::op0xB1), 4 };
		table[0xB2] = { "OR", bind(&Cpu::op0xB2), 4 };  table[0xB3] = { "OR", bind(&Cpu::op0xB3), 4 };
		table[0xB4] = { "OR", bind(&Cpu::op0xB4), 4 };  table[0xB5] = { "OR", bind(&Cpu::op0xB5), 4 };
		table[0xB6] = { "OR", bind(&Cpu::op0xB6), 8 };  table[0xB7] = { "OR", bind(&Cpu::op0xB7), 4 };
		table[0xB8] = { "CP", bind(&Cpu::op0xB8), 4 };  table[0xB9] = { "CP", bind(&Cpu::op0xB9), 4 };
		table[0xBA] = { "CP", bind(&Cpu::op0xBA), 4 };  table[0xBB] = { "CP", bind(&Cpu::op0xBB), 4 };
		table[0xBC] = { "CP", bind(&Cpu::op0xBC), 4 };  table[0xBD] = { "CP", bind(&Cpu::op0xBD), 4 };
		table[0xBE] = { "CP", bind(&Cpu::op0xBE), 8 };  table[0xBF] = { "CP", bind(&Cpu::op0xBF), 4 };

		/* Twelfth row */
		table[0xC0] = { "RET", bind(&Cpu::op0xC0), 8 };  table[0xC1] = { "POP", bind(&Cpu::op0xC1), 12 };
		table[0xC2] = { "JP", bind(&Cpu::op0xC2), 12 };  table[0xC3] = { "JP", bind(&Cpu::op0xC3), 16 };
		table[0xC4] = { "CALL", bind(&Cpu::op0xC4), 12 };  table[0xC5] = { "PUSH", bind(&Cpu::op0xC5), 16 };
		table[0xC6] = { "ADD", bind(&Cpu::op0xC6), 8 };  table[0xC7] = { "RST", bind(&Cpu::op0xC7), 16 };
		table[0xC8] = { "RET", bind(&Cpu::op0xC8), 8 };  table[0xC9] = { "RET", bind(&Cpu::op0xC9), 16 };
		table[0xCA] = { "JP", bind(&Cpu::op0xCA), 12 };  table[0xCB] = { "CB", bind(&Cpu::op0xCB), 4 };
		table[0xCC] = { "CALL", bind(&Cpu::op0xCC), 12 };  table[0xCD] = { "CALL", bind(&Cpu::op0xCD), 24 };
		table[0xCE] = { "ADC", bind(&Cpu::op0xCE), 8 };  table[0xCF] = { "RST", bind(&Cpu::op0xCF), 16 };

		/* Thirteenth row */
		table[0xD0] = { "RET", bind(&Cpu::op0xD0), 8 };  table[0xD1] = { "POP", bind(&Cpu::op0xD1), 12 };
		table[0xD2] = { "JP", bind(&Cpu::op0xD2), 12 };  table[0xD3] = { "NONE", nullptr, 0 };
		table[0xD4] = { "CALL", bind(&Cpu::op0xD4), 12 };table[0xD5] = { "PUSH", bind(&Cpu::op0xD5), 16 };
		table[0xD6] = { "SUB", bind(&Cpu::op0xD6), 8 };  table[0xD7] = { "RST", bind(&Cpu::op0xD7), 16 };
		table[0xD8] = { "RET", bind(&Cpu::op0xD8), 8 };  table[0xD9] = { "RET", bind(&Cpu::op0xD9), 16 };
		table[0xDA] = { "JP", bind(&Cpu::op0xDA), 12 };  table[0xDB] = { "NONE", nullptr, 0 };
		table[0xDC] = { "CALL", bind(&Cpu::op0xDC), 12 };table[0xDD] = { "NONE", nullptr, 0 };
		table[0xDE] = { "SBC", bind(&Cpu::op0xDE), 8 };  table[0xDF] = { "RST", bind(&Cpu::op0xDF), 16 };

		/* Fourteenth row */
		table[0xE0] = { "LD", bind(&Cpu::op0xE0), 12 };  table[0xE1] = { "POP", bind(&Cpu::op0xE1), 12 };
		table[0xE2] = { "LD", bind(&Cpu::op0xE2), 8 };   table[0xE3] = { "NONE", nullptr, 0 };
		table[0xE4] = { "NONE", nullptr, 0 };            table[0xE5] = { "PUSH", bind(&Cpu::op0xE5), 16 };
		table[0xE6] = { "AND", bind(&Cpu::op0xE6), 8 };  table[0xE7] = { "RST", bind(&Cpu::op0xE7), 16 };
		table[0xE8] = { "ADD", bind(&Cpu::op0xE8), 16 }; table[0xE9] = { "JP", bind(&Cpu::op0xE9), 4 };
		table[0xEA] = { "LD", bind(&Cpu::op0xEA), 16 };  table[0xEB] = { "NONE", nullptr, 0 };
		table[0xEC] = { "NONE", nullptr, 0 };            table[0xED] = { "NONE", nullptr, 0 };
		table[0xEE] = { "XOR", bind(&Cpu::op0xEE), 8 };  table[0xEF] = { "RST", bind(&Cpu::op0xEF), 16 };


		/* Fifteenth row */
		table[0xF0] = { "LD", bind(&Cpu::op0xF0), 12 };  table[0xF1] = { "POP", bind(&Cpu::op0xF1), 12 };
		table[0xF2] = { "LD", bind(&Cpu::op0xF2), 8 };   table[0xF3] = { "DI", bind(&Cpu::op0xF3), 4 };
		table[0xF4] = { "NONE", nullptr, 0 };            table[0xF5] = { "PUSH", bind(&Cpu::op0xF5), 16 };
		table[0xF6] = { "OR", bind(&Cpu::op0xF6), 8 };   table[0xF7] = { "RST", bind(&Cpu::op0xF7), 16 };
		table[0xF8] = { "LD", bind(&Cpu::op0xF8), 12 };  table[0xF9] = { "LD", bind(&Cpu::op0xF9), 8 };
		table[0xFA] = { "LD", bind(&Cpu::op0xFA), 16 };  table[0xFB] = { "EI", bind(&Cpu::op0xFB), 4 };
		table[0xFC] = { "NONE", nullptr, 0 };            table[0xFD] = { "NONE", nullptr, 0 };
		table[0xFE] = { "CP", bind(&Cpu::op0xFE), 8 };   table[0xFF] = { "RST", bind(&Cpu::op0xFF), 16 };
	}
}
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
		table[0x34] = { "INC", bind(&Cpu::op0x34), 12 }; table[0x35] = { "DEC", bind(&Cpu::op0x35), 12 };
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

		/* Eleventh row */
		table[0xA0] = { "AND", bind(&Cpu::op0xA0), 4 };  table[0xA1] = { "AND", bind(&Cpu::op0xA1), 4 };
		table[0xA2] = { "AND", bind(&Cpu::op0xA2), 4 };  table[0xA3] = { "AND", bind(&Cpu::op0xA3), 4 };
		table[0xA4] = { "AND", bind(&Cpu::op0xA4), 4 };  table[0xA5] = { "AND", bind(&Cpu::op0xA5), 4 };
		table[0xA6] = { "AND", bind(&Cpu::op0xA6), 8 };  table[0xA7] = { "AND", bind(&Cpu::op0xA7), 4 };
		table[0xA8] = { "XOR", bind(&Cpu::op0xA8), 4 };  table[0xA9] = { "XOR", bind(&Cpu::op0xA9), 4 };
		table[0xAA] = { "XOR", bind(&Cpu::op0xAA), 4 };  table[0xAB] = { "XOR", bind(&Cpu::op0xAB), 4 };
		table[0xAC] = { "XOR", bind(&Cpu::op0xAC), 4 };  table[0xAD] = { "XOR", bind(&Cpu::op0xAD), 4 };
		table[0xAE] = { "XOR", bind(&Cpu::op0xAE), 8 };  table[0xAF] = { "XOR", bind(&Cpu::op0xAF), 4 };

		/* Twelfth row */
		table[0xB0] = { "OR", bind(&Cpu::op0xB0), 4 };  table[0xB1] = { "OR", bind(&Cpu::op0xB1), 4 };
		table[0xB2] = { "OR", bind(&Cpu::op0xB2), 4 };  table[0xB3] = { "OR", bind(&Cpu::op0xB3), 4 };
		table[0xB4] = { "OR", bind(&Cpu::op0xB4), 4 };  table[0xB5] = { "OR", bind(&Cpu::op0xB5), 4 };
		table[0xB6] = { "OR", bind(&Cpu::op0xB6), 8 };  table[0xB7] = { "OR", bind(&Cpu::op0xB7), 4 };
		table[0xB8] = { "CP", bind(&Cpu::op0xB8), 4 };  table[0xB9] = { "CP", bind(&Cpu::op0xB9), 4 };
		table[0xBA] = { "CP", bind(&Cpu::op0xBA), 4 };  table[0xBB] = { "CP", bind(&Cpu::op0xBB), 4 };
		table[0xBC] = { "CP", bind(&Cpu::op0xBC), 4 };  table[0xBD] = { "CP", bind(&Cpu::op0xBD), 4 };
		table[0xBE] = { "CP", bind(&Cpu::op0xBE), 8 };  table[0xBF] = { "CP", bind(&Cpu::op0xBF), 4 };

		/* Thirteenth row */
		table[0xC0] = { "RET", bind(&Cpu::op0xC0), 8 };  table[0xC1] = { "POP", bind(&Cpu::op0xC1), 12 };
		table[0xC2] = { "JP", bind(&Cpu::op0xC2), 12 };  table[0xC3] = { "JP", bind(&Cpu::op0xC3), 16 };
		table[0xC4] = { "CALL", bind(&Cpu::op0xC4), 12 };  table[0xC5] = { "PUSH", bind(&Cpu::op0xC5), 16 };
		table[0xC6] = { "ADD", bind(&Cpu::op0xC6), 8 };  table[0xC7] = { "RST", bind(&Cpu::op0xC7), 16 };
		table[0xC8] = { "RET", bind(&Cpu::op0xC8), 8 };  table[0xC9] = { "RET", bind(&Cpu::op0xC9), 16 };
		table[0xCA] = { "JP", bind(&Cpu::op0xCA), 12 };  table[0xCB] = { "CB", bind(&Cpu::op0xCB), 4 };
		table[0xCC] = { "CALL", bind(&Cpu::op0xCC), 12 };  table[0xCD] = { "CALL", bind(&Cpu::op0xCD), 24 };
		table[0xCE] = { "ADC", bind(&Cpu::op0xCE), 8 };  table[0xCF] = { "RST", bind(&Cpu::op0xCF), 16 };

		/* Fourteenth row */
		table[0xD0] = { "RET", bind(&Cpu::op0xD0), 8 };  table[0xD1] = { "POP", bind(&Cpu::op0xD1), 12 };
		table[0xD2] = { "JP", bind(&Cpu::op0xD2), 12 };  table[0xD3] = { "NONE", bind(&Cpu::op0xD3), 0 };
		table[0xD4] = { "CALL", bind(&Cpu::op0xD4), 12 };table[0xD5] = { "PUSH", bind(&Cpu::op0xD5), 16 };
		table[0xD6] = { "SUB", bind(&Cpu::op0xD6), 8 };  table[0xD7] = { "RST", bind(&Cpu::op0xD7), 16 };
		table[0xD8] = { "RET", bind(&Cpu::op0xD8), 8 };  table[0xD9] = { "RET", bind(&Cpu::op0xD9), 16 };
		table[0xDA] = { "JP", bind(&Cpu::op0xDA), 12 };  table[0xDB] = { "NONE", bind(&Cpu::op0xDB), 0 };
		table[0xDC] = { "CALL", bind(&Cpu::op0xDC), 12 };table[0xDD] = { "NONE", bind(&Cpu::op0xDD), 0 };
		table[0xDE] = { "SBC", bind(&Cpu::op0xDE), 8 };  table[0xDF] = { "RST", bind(&Cpu::op0xDF), 16 };

		/* Fifteenth row */
		table[0xE0] = { "LD", bind(&Cpu::op0xE0), 12 };  table[0xE1] = { "POP", bind(&Cpu::op0xE1), 12 };
		table[0xE2] = { "LD", bind(&Cpu::op0xE2), 8 };   table[0xE3] = { "NONE", bind(&Cpu::op0xE3), 0 };
		table[0xE4] = { "NONE", bind(&Cpu::op0xE4), 0 }; table[0xE5] = { "PUSH", bind(&Cpu::op0xE5), 16 };
		table[0xE6] = { "AND", bind(&Cpu::op0xE6), 8 };  table[0xE7] = { "RST", bind(&Cpu::op0xE7), 16 };
		table[0xE8] = { "ADD", bind(&Cpu::op0xE8), 16 }; table[0xE9] = { "JP", bind(&Cpu::op0xE9), 4 };
		table[0xEA] = { "LD", bind(&Cpu::op0xEA), 16 };  table[0xEB] = { "NONE", bind(&Cpu::op0xEB), 0 };
		table[0xEC] = { "NONE", bind(&Cpu::op0xEC), 0 }; table[0xED] = { "NONE", bind(&Cpu::op0xED), 0 };
		table[0xEE] = { "XOR", bind(&Cpu::op0xEE), 8 };  table[0xEF] = { "RST", bind(&Cpu::op0xEF), 16 };


		/* Sixteenth row */
		table[0xF0] = { "LD", bind(&Cpu::op0xF0), 12 };  table[0xF1] = { "POP", bind(&Cpu::op0xF1), 12 };
		table[0xF2] = { "LD", bind(&Cpu::op0xF2), 8 };   table[0xF3] = { "DI", bind(&Cpu::op0xF3), 4 };
		table[0xF4] = { "NONE", bind(&Cpu::op0xF4), 0 }; table[0xF5] = { "PUSH", bind(&Cpu::op0xF5), 16 };
		table[0xF6] = { "OR", bind(&Cpu::op0xF6), 8 };   table[0xF7] = { "RST", bind(&Cpu::op0xF7), 16 };
		table[0xF8] = { "LD", bind(&Cpu::op0xF8), 12 };  table[0xF9] = { "LD", bind(&Cpu::op0xF9), 8 };
		table[0xFA] = { "LD", bind(&Cpu::op0xFA), 16 };  table[0xFB] = { "EI", bind(&Cpu::op0xFB), 4 };
		table[0xFC] = { "NONE", bind(&Cpu::op0xFC), 0 }; table[0xFD] = { "NONE", bind(&Cpu::op0xFD), 0 };
		table[0xFE] = { "CP", bind(&Cpu::op0xFE), 8 };   table[0xFF] = { "RST", bind(&Cpu::op0xFF), 16 };






		/*
			0xCB Table
		*/
		cbTable.resize(0x100);

		cbTable[0x00] = { "RLC", bind(&Cpu::opCB0x00), 8 }; cbTable[0x01] = { "RLC", bind(&Cpu::opCB0x01), 8 };
		cbTable[0x02] = { "RLC", bind(&Cpu::opCB0x02), 8 }; cbTable[0x03] = { "RLC", bind(&Cpu::opCB0x03), 8 };
		cbTable[0x04] = { "RLC", bind(&Cpu::opCB0x04), 8 }; cbTable[0x05] = { "RLC", bind(&Cpu::opCB0x05), 8 };
		cbTable[0x06] = { "RLC", bind(&Cpu::opCB0x06), 16 };cbTable[0x07] = { "RLC", bind(&Cpu::opCB0x07), 8 };
		cbTable[0x08] = { "RRC", bind(&Cpu::opCB0x08), 8 }; cbTable[0x09] = { "RRC", bind(&Cpu::opCB0x09), 8 };
		cbTable[0x0A] = { "RRC", bind(&Cpu::opCB0x0A), 8 }; cbTable[0x0B] = { "RRC", bind(&Cpu::opCB0x0B), 8 };
		cbTable[0x0C] = { "RRC", bind(&Cpu::opCB0x0C), 8 }; cbTable[0x0D] = { "RRC", bind(&Cpu::opCB0x0D), 8 };
		cbTable[0x0E] = { "RRC", bind(&Cpu::opCB0x0E), 16 };cbTable[0x0F] = { "RRC", bind(&Cpu::opCB0x0F), 8 };

		/* Second row */
		cbTable[0x10] = { "RL", bind(&Cpu::opCB0x10), 8 }; cbTable[0x11] = { "RL", bind(&Cpu::opCB0x11), 8 };
		cbTable[0x12] = { "RL", bind(&Cpu::opCB0x12), 8 }; cbTable[0x13] = { "RL", bind(&Cpu::opCB0x13), 8 };
		cbTable[0x14] = { "RL", bind(&Cpu::opCB0x14), 8 }; cbTable[0x15] = { "RL", bind(&Cpu::opCB0x15), 8 };
		cbTable[0x16] = { "RL", bind(&Cpu::opCB0x16), 16 };cbTable[0x17] = { "RL", bind(&Cpu::opCB0x17), 8 };
		cbTable[0x18] = { "RR", bind(&Cpu::opCB0x18), 8 }; cbTable[0x19] = { "RR", bind(&Cpu::opCB0x19), 8 };
		cbTable[0x1A] = { "RR", bind(&Cpu::opCB0x1A), 8 }; cbTable[0x1B] = { "RR", bind(&Cpu::opCB0x1B), 8 };
		cbTable[0x1C] = { "RR", bind(&Cpu::opCB0x1C), 8 }; cbTable[0x1D] = { "RR", bind(&Cpu::opCB0x1D), 8 };
		cbTable[0x1E] = { "RR", bind(&Cpu::opCB0x1E), 16 };cbTable[0x1F] = { "RR", bind(&Cpu::opCB0x1F), 8 };

		/* Third row */
		cbTable[0x20] = { "SLA", bind(&Cpu::opCB0x20), 8 }; cbTable[0x21] = { "SLA", bind(&Cpu::opCB0x21), 8 };
		cbTable[0x22] = { "SLA", bind(&Cpu::opCB0x22), 8 }; cbTable[0x23] = { "SLA", bind(&Cpu::opCB0x23), 8 };
		cbTable[0x24] = { "SLA", bind(&Cpu::opCB0x24), 8 }; cbTable[0x25] = { "SLA", bind(&Cpu::opCB0x25), 8 };
		cbTable[0x26] = { "SLA", bind(&Cpu::opCB0x26), 16 };cbTable[0x27] = { "SLA", bind(&Cpu::opCB0x27), 8 };
		cbTable[0x28] = { "SRA", bind(&Cpu::opCB0x28), 8 }; cbTable[0x29] = { "SRA", bind(&Cpu::opCB0x29), 8 };
		cbTable[0x2A] = { "SRA", bind(&Cpu::opCB0x2A), 8 }; cbTable[0x2B] = { "SRA", bind(&Cpu::opCB0x2B), 8 };
		cbTable[0x2C] = { "SRA", bind(&Cpu::opCB0x2C), 8 }; cbTable[0x2D] = { "SRA", bind(&Cpu::opCB0x2D), 8 };
		cbTable[0x2E] = { "SRA", bind(&Cpu::opCB0x2E), 16 };cbTable[0x2F] = { "SRA", bind(&Cpu::opCB0x2F), 8 };

		/* Fourth row */
		cbTable[0x30] = { "SWAP", bind(&Cpu::opCB0x30), 8 }; cbTable[0x31] = { "SWAP", bind(&Cpu::opCB0x31), 8 };
		cbTable[0x32] = { "SWAP", bind(&Cpu::opCB0x32), 8 }; cbTable[0x33] = { "SWAP", bind(&Cpu::opCB0x33), 8 };
		cbTable[0x34] = { "SWAP", bind(&Cpu::opCB0x34), 8 }; cbTable[0x35] = { "SWAP", bind(&Cpu::opCB0x35), 8 };
		cbTable[0x36] = { "SWAP", bind(&Cpu::opCB0x36), 16 };cbTable[0x37] = { "SWAP", bind(&Cpu::opCB0x37), 8 };
		cbTable[0x38] = { "SRL", bind(&Cpu::opCB0x38), 8 };  cbTable[0x39] = { "SRL", bind(&Cpu::opCB0x39), 8 };
		cbTable[0x3A] = { "SRL", bind(&Cpu::opCB0x3A), 8 };  cbTable[0x3B] = { "SRL", bind(&Cpu::opCB0x3B), 8 };
		cbTable[0x3C] = { "SRL", bind(&Cpu::opCB0x3C), 8 };  cbTable[0x3D] = { "SRL", bind(&Cpu::opCB0x3D), 8 };
		cbTable[0x3E] = { "SRL", bind(&Cpu::opCB0x3E), 16 }; cbTable[0x3F] = { "SRL", bind(&Cpu::opCB0x3F), 8 };

		/* Fifth row */
		cbTable[0x40] = { "BIT", bind(&Cpu::opCB0x40), 8 }; cbTable[0x41] = { "BIT", bind(&Cpu::opCB0x41), 8 };
		cbTable[0x42] = { "BIT", bind(&Cpu::opCB0x42), 8 }; cbTable[0x43] = { "BIT", bind(&Cpu::opCB0x43), 8 };
		cbTable[0x44] = { "BIT", bind(&Cpu::opCB0x44), 8 }; cbTable[0x45] = { "BIT", bind(&Cpu::opCB0x45), 8 };
		cbTable[0x46] = { "BIT", bind(&Cpu::opCB0x46), 12 };cbTable[0x47] = { "BIT", bind(&Cpu::opCB0x47), 8 };
		cbTable[0x48] = { "BIT", bind(&Cpu::opCB0x48), 8 }; cbTable[0x49] = { "BIT", bind(&Cpu::opCB0x49), 8 };
		cbTable[0x4A] = { "BIT", bind(&Cpu::opCB0x4A), 8 }; cbTable[0x4B] = { "BIT", bind(&Cpu::opCB0x4B), 8 };
		cbTable[0x4C] = { "BIT", bind(&Cpu::opCB0x4C), 8 }; cbTable[0x4D] = { "BIT", bind(&Cpu::opCB0x4D), 8 };
		cbTable[0x4E] = { "BIT", bind(&Cpu::opCB0x4E), 12 };cbTable[0x4F] = { "BIT", bind(&Cpu::opCB0x4F), 8 };

		/* Sixth row */
		cbTable[0x50] = { "BIT", bind(&Cpu::opCB0x50), 8 };  cbTable[0x51] = { "BIT", bind(&Cpu::opCB0x51), 8 };
		cbTable[0x52] = { "BIT", bind(&Cpu::opCB0x52), 8 };  cbTable[0x53] = { "BIT", bind(&Cpu::opCB0x53), 8 };
		cbTable[0x54] = { "BIT", bind(&Cpu::opCB0x54), 8 };  cbTable[0x55] = { "BIT", bind(&Cpu::opCB0x55), 8 };
		cbTable[0x56] = { "BIT", bind(&Cpu::opCB0x56), 12 }; cbTable[0x57] = { "BIT", bind(&Cpu::opCB0x57), 8 };
		cbTable[0x58] = { "BIT", bind(&Cpu::opCB0x58), 8 };  cbTable[0x59] = { "BIT", bind(&Cpu::opCB0x59), 8 };
		cbTable[0x5A] = { "BIT", bind(&Cpu::opCB0x5A), 8 };  cbTable[0x5B] = { "BIT", bind(&Cpu::opCB0x5B), 8 };
		cbTable[0x5C] = { "BIT", bind(&Cpu::opCB0x5C), 8 };  cbTable[0x5D] = { "BIT", bind(&Cpu::opCB0x5D), 8 };
		cbTable[0x5E] = { "BIT", bind(&Cpu::opCB0x5E), 12 }; cbTable[0x5F] = { "BIT", bind(&Cpu::opCB0x5F), 8 };

		/* Seventh row */
		cbTable[0x60] = { "BIT", bind(&Cpu::opCB0x60), 8 };  cbTable[0x61] = { "BIT", bind(&Cpu::opCB0x61), 8 };
		cbTable[0x62] = { "BIT", bind(&Cpu::opCB0x62), 8 };  cbTable[0x63] = { "BIT", bind(&Cpu::opCB0x63), 8 };
		cbTable[0x64] = { "BIT", bind(&Cpu::opCB0x64), 8 };  cbTable[0x65] = { "BIT", bind(&Cpu::opCB0x65), 8 };
		cbTable[0x66] = { "BIT", bind(&Cpu::opCB0x66), 12 }; cbTable[0x67] = { "BIT", bind(&Cpu::opCB0x67), 8 };
		cbTable[0x68] = { "BIT", bind(&Cpu::opCB0x68), 8 };  cbTable[0x69] = { "BIT", bind(&Cpu::opCB0x69), 8 };
		cbTable[0x6A] = { "BIT", bind(&Cpu::opCB0x6A), 8 };  cbTable[0x6B] = { "BIT", bind(&Cpu::opCB0x6B), 8 };
		cbTable[0x6C] = { "BIT", bind(&Cpu::opCB0x6C), 8 };  cbTable[0x6D] = { "BIT", bind(&Cpu::opCB0x6D), 8 };
		cbTable[0x6E] = { "BIT", bind(&Cpu::opCB0x6E), 12 }; cbTable[0x6F] = { "BIT", bind(&Cpu::opCB0x6F), 8 };

		/* Eighth row */
		cbTable[0x70] = { "BIT", bind(&Cpu::opCB0x70), 8 };  cbTable[0x71] = { "BIT", bind(&Cpu::opCB0x71), 8 };
		cbTable[0x72] = { "BIT", bind(&Cpu::opCB0x72), 8 };  cbTable[0x73] = { "BIT", bind(&Cpu::opCB0x73), 8 };
		cbTable[0x74] = { "BIT", bind(&Cpu::opCB0x74), 8 };  cbTable[0x75] = { "BIT", bind(&Cpu::opCB0x75), 8 };
		cbTable[0x76] = { "BIT", bind(&Cpu::opCB0x76), 12 }; cbTable[0x77] = { "BIT", bind(&Cpu::opCB0x77), 8 };
		cbTable[0x78] = { "BIT", bind(&Cpu::opCB0x78), 8 };  cbTable[0x79] = { "BIT", bind(&Cpu::opCB0x79), 8 };
		cbTable[0x7A] = { "BIT", bind(&Cpu::opCB0x7A), 8 };  cbTable[0x7B] = { "BIT", bind(&Cpu::opCB0x7B), 8 };
		cbTable[0x7C] = { "BIT", bind(&Cpu::opCB0x7C), 8 };  cbTable[0x7D] = { "BIT", bind(&Cpu::opCB0x7D), 8 };
		cbTable[0x7E] = { "BIT", bind(&Cpu::opCB0x7E), 12 }; cbTable[0x7F] = { "BIT", bind(&Cpu::opCB0x7F), 8 };

		/* Ninth row */
		cbTable[0x80] = { "RES", bind(&Cpu::opCB0x80), 8 };  cbTable[0x81] = { "RES", bind(&Cpu::opCB0x81), 8 };
		cbTable[0x82] = { "RES", bind(&Cpu::opCB0x82), 8 };  cbTable[0x83] = { "RES", bind(&Cpu::opCB0x83), 8 };
		cbTable[0x84] = { "RES", bind(&Cpu::opCB0x84), 8 };  cbTable[0x85] = { "RES", bind(&Cpu::opCB0x85), 8 };
		cbTable[0x86] = { "RES", bind(&Cpu::opCB0x86), 16 }; cbTable[0x87] = { "RES", bind(&Cpu::opCB0x87), 8 };
		cbTable[0x88] = { "RES", bind(&Cpu::opCB0x88), 8 };  cbTable[0x89] = { "RES", bind(&Cpu::opCB0x89), 8 };
		cbTable[0x8A] = { "RES", bind(&Cpu::opCB0x8A), 8 };  cbTable[0x8B] = { "RES", bind(&Cpu::opCB0x8B), 8 };
		cbTable[0x8C] = { "RES", bind(&Cpu::opCB0x8C), 8 };  cbTable[0x8D] = { "RES", bind(&Cpu::opCB0x8D), 8 };
		cbTable[0x8E] = { "RES", bind(&Cpu::opCB0x8E), 16 }; cbTable[0x8F] = { "RES", bind(&Cpu::opCB0x8F), 8 };

		/* Tenth row */
		cbTable[0x90] = { "RES", bind(&Cpu::opCB0x90), 8 };  cbTable[0x91] = { "RES", bind(&Cpu::opCB0x91), 8 };
		cbTable[0x92] = { "RES", bind(&Cpu::opCB0x92), 8 };  cbTable[0x93] = { "RES", bind(&Cpu::opCB0x93), 8 };
		cbTable[0x94] = { "RES", bind(&Cpu::opCB0x94), 8 };  cbTable[0x95] = { "RES", bind(&Cpu::opCB0x95), 8 };
		cbTable[0x96] = { "RES", bind(&Cpu::opCB0x96), 16 }; cbTable[0x97] = { "RES", bind(&Cpu::opCB0x97), 8 };
		cbTable[0x98] = { "RES", bind(&Cpu::opCB0x98), 8 };  cbTable[0x99] = { "RES", bind(&Cpu::opCB0x99), 8 };
		cbTable[0x9A] = { "RES", bind(&Cpu::opCB0x9A), 8 };  cbTable[0x9B] = { "RES", bind(&Cpu::opCB0x9B), 8 };
		cbTable[0x9C] = { "RES", bind(&Cpu::opCB0x9C), 8 };  cbTable[0x9D] = { "RES", bind(&Cpu::opCB0x9D), 8 };
		cbTable[0x9E] = { "RES", bind(&Cpu::opCB0x9E), 16 }; cbTable[0x9F] = { "RES", bind(&Cpu::opCB0x9F), 8 };

		/* Eleventh row */
		cbTable[0xA0] = { "RES", bind(&Cpu::opCB0xA0), 8 };  cbTable[0xA1] = { "RES", bind(&Cpu::opCB0xA1), 8 };
		cbTable[0xA2] = { "RES", bind(&Cpu::opCB0xA2), 8 };  cbTable[0xA3] = { "RES", bind(&Cpu::opCB0xA3), 8 };
		cbTable[0xA4] = { "RES", bind(&Cpu::opCB0xA4), 8 };  cbTable[0xA5] = { "RES", bind(&Cpu::opCB0xA5), 8 };
		cbTable[0xA6] = { "RES", bind(&Cpu::opCB0xA6), 16 }; cbTable[0xA7] = { "RES", bind(&Cpu::opCB0xA7), 8 };
		cbTable[0xA8] = { "RES", bind(&Cpu::opCB0xA8), 8 };  cbTable[0xA9] = { "RES", bind(&Cpu::opCB0xA9), 8 };
		cbTable[0xAA] = { "RES", bind(&Cpu::opCB0xAA), 8 };  cbTable[0xAB] = { "RES", bind(&Cpu::opCB0xAB), 8 };
		cbTable[0xAC] = { "RES", bind(&Cpu::opCB0xAC), 8 };  cbTable[0xAD] = { "RES", bind(&Cpu::opCB0xAD), 8 };
		cbTable[0xAE] = { "RES", bind(&Cpu::opCB0xAE), 16 }; cbTable[0xAF] = { "RES", bind(&Cpu::opCB0xAF), 8 };

		/* Twelfth row */
		cbTable[0xB0] = { "RES", bind(&Cpu::opCB0xB0), 8 }; cbTable[0xB1] = { "RES", bind(&Cpu::opCB0xB1), 8 };
		cbTable[0xB2] = { "RES", bind(&Cpu::opCB0xB2), 8 }; cbTable[0xB3] = { "RES", bind(&Cpu::opCB0xB3), 8 };
		cbTable[0xB4] = { "RES", bind(&Cpu::opCB0xB4), 8 }; cbTable[0xB5] = { "RES", bind(&Cpu::opCB0xB5), 8 };
		cbTable[0xB6] = { "RES", bind(&Cpu::opCB0xB6), 16 };cbTable[0xB7] = { "RES", bind(&Cpu::opCB0xB7), 8 };
		cbTable[0xB8] = { "RES", bind(&Cpu::opCB0xB8), 8 }; cbTable[0xB9] = { "RES", bind(&Cpu::opCB0xB9), 8 };
		cbTable[0xBA] = { "RES", bind(&Cpu::opCB0xBA), 8 }; cbTable[0xBB] = { "RES", bind(&Cpu::opCB0xBB), 8 };
		cbTable[0xBC] = { "RES", bind(&Cpu::opCB0xBC), 8 }; cbTable[0xBD] = { "RES", bind(&Cpu::opCB0xBD), 8 };
		cbTable[0xBE] = { "RES", bind(&Cpu::opCB0xBE), 16 };cbTable[0xBF] = { "RES", bind(&Cpu::opCB0xBF), 8 };

		/* Thirteenth row */
		cbTable[0xC0] = { "SET", bind(&Cpu::opCB0xC0), 8 };  cbTable[0xC1] = { "SET", bind(&Cpu::opCB0xC1), 8 };
		cbTable[0xC2] = { "SET", bind(&Cpu::opCB0xC2), 8 };  cbTable[0xC3] = { "SET", bind(&Cpu::opCB0xC3), 8 };
		cbTable[0xC4] = { "SET", bind(&Cpu::opCB0xC4), 8 };  cbTable[0xC5] = { "SET", bind(&Cpu::opCB0xC5), 8 };
		cbTable[0xC6] = { "SET", bind(&Cpu::opCB0xC6), 16 }; cbTable[0xC7] = { "SET", bind(&Cpu::opCB0xC7), 8 };
		cbTable[0xC8] = { "SET", bind(&Cpu::opCB0xC8), 8 };  cbTable[0xC9] = { "SET", bind(&Cpu::opCB0xC9), 8 };
		cbTable[0xCA] = { "SET", bind(&Cpu::opCB0xCA), 8 };  cbTable[0xCB] = { "SET", bind(&Cpu::opCB0xCB), 8 };
		cbTable[0xCC] = { "SET", bind(&Cpu::opCB0xCC), 8 };  cbTable[0xCD] = { "SET", bind(&Cpu::opCB0xCD), 8 };
		cbTable[0xCE] = { "SET", bind(&Cpu::opCB0xCE), 16 }; cbTable[0xCF] = { "SET", bind(&Cpu::opCB0xCF), 8 };

		/* Fourteenth row */
		cbTable[0xD0] = { "SET", bind(&Cpu::opCB0xD0), 8 };  cbTable[0xD1] = { "SET", bind(&Cpu::opCB0xD1), 8 };
		cbTable[0xD2] = { "SET", bind(&Cpu::opCB0xD2), 8 };  cbTable[0xD3] = { "SET", bind(&Cpu::opCB0xD3), 8 };
		cbTable[0xD4] = { "SET", bind(&Cpu::opCB0xD4), 8 };  cbTable[0xD5] = { "SET", bind(&Cpu::opCB0xD5), 8 };
		cbTable[0xD6] = { "SET", bind(&Cpu::opCB0xD6), 16 }; cbTable[0xD7] = { "SET", bind(&Cpu::opCB0xD7), 8 };
		cbTable[0xD8] = { "SET", bind(&Cpu::opCB0xD8), 8 };  cbTable[0xD9] = { "SET", bind(&Cpu::opCB0xD9), 8 };
		cbTable[0xDA] = { "SET", bind(&Cpu::opCB0xDA), 8 };  cbTable[0xDB] = { "SET", bind(&Cpu::opCB0xDB), 8 };
		cbTable[0xDC] = { "SET", bind(&Cpu::opCB0xDC), 8 };  cbTable[0xDD] = { "SET", bind(&Cpu::opCB0xDD), 8 };
		cbTable[0xDE] = { "SET", bind(&Cpu::opCB0xDE), 16 }; cbTable[0xDF] = { "SET", bind(&Cpu::opCB0xDF), 8 };

		/* Fifteenth row */
		cbTable[0xE0] = { "SET", bind(&Cpu::opCB0xE0), 8 };  cbTable[0xE1] = { "SET", bind(&Cpu::opCB0xE1), 8 };
		cbTable[0xE2] = { "SET", bind(&Cpu::opCB0xE2), 8 };  cbTable[0xE3] = { "SET", bind(&Cpu::opCB0xE3), 8 };
		cbTable[0xE4] = { "SET", bind(&Cpu::opCB0xE4), 8 };  cbTable[0xE5] = { "SET", bind(&Cpu::opCB0xE5), 8 };
		cbTable[0xE6] = { "SET", bind(&Cpu::opCB0xE6), 16 }; cbTable[0xE7] = { "SET", bind(&Cpu::opCB0xE7), 8 };
		cbTable[0xE8] = { "SET", bind(&Cpu::opCB0xE8), 8 };  cbTable[0xE9] = { "SET", bind(&Cpu::opCB0xE9), 8 };
		cbTable[0xEA] = { "SET", bind(&Cpu::opCB0xEA), 8 };  cbTable[0xEB] = { "SET", bind(&Cpu::opCB0xEB), 8 };
		cbTable[0xEC] = { "SET", bind(&Cpu::opCB0xEC), 8 };  cbTable[0xED] = { "SET", bind(&Cpu::opCB0xED), 8 };
		cbTable[0xEE] = { "SET", bind(&Cpu::opCB0xEE), 16 }; cbTable[0xEF] = { "SET", bind(&Cpu::opCB0xEF), 8 };


		/* Sixteenth row */
		cbTable[0xF0] = { "SET", bind(&Cpu::opCB0xF0), 8 };  cbTable[0xF1] = { "SET", bind(&Cpu::opCB0xF1), 8 };
		cbTable[0xF2] = { "SET", bind(&Cpu::opCB0xF2), 8 };  cbTable[0xF3] = { "SET", bind(&Cpu::opCB0xF3), 8 };
		cbTable[0xF4] = { "SET", bind(&Cpu::opCB0xF4), 8 };  cbTable[0xF5] = { "SET", bind(&Cpu::opCB0xF5), 8 };
		cbTable[0xF6] = { "SET", bind(&Cpu::opCB0xF6), 16 }; cbTable[0xF7] = { "SET", bind(&Cpu::opCB0xF7), 8 };
		cbTable[0xF8] = { "SET", bind(&Cpu::opCB0xF8), 8 };  cbTable[0xF9] = { "SET", bind(&Cpu::opCB0xF9), 8 };
		cbTable[0xFA] = { "SET", bind(&Cpu::opCB0xFA), 8 };  cbTable[0xFB] = { "SET", bind(&Cpu::opCB0xFB), 8 };
		cbTable[0xFC] = { "SET", bind(&Cpu::opCB0xFC), 8 };  cbTable[0xFD] = { "SET", bind(&Cpu::opCB0xFD), 8 };
		cbTable[0xFE] = { "SET", bind(&Cpu::opCB0xFE), 16 }; cbTable[0xFF] = { "SET", bind(&Cpu::opCB0xFF), 8 };
	}
}
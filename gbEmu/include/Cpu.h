#pragma once
#include "Utils.h"
#include <unordered_map>
#include <functional>
#include <vector>

//For function pointer binding in opcode table setup
#define bind(f) std::bind(f, this)

/*
	*Gameboy CPU:
	*8 bit CPU - Sharp LR35902
*/

namespace gbEmu {

#define FLAG_BIT_Z 7
#define FLAG_BIT_N 6
#define FLAG_BIT_H 5
#define FLAG_BIT_C 4

#define FLAG_Z (1 << 7) //Zero flag
#define FLAG_N (1 << 6) //Subtract flag
#define FLAG_H (1 << 5) //Half Carry flag
#define FLAG_C (1 << 4) //Carry flag
	
	struct MMU;

	union Register {
		struct {
			u8 lo;
			u8 hi;
		};
		u16 value;
	};

	struct Cpu {
		Cpu(MMU *mmu);
		~Cpu();

		void reset();

		/*
			Area to test instructions
		*/
		void testFunc();

		void clock();
		
		u8 read(u16 address);
		void write(u16 address, u8 value);

		void connectMMU(MMU* mmu);

		
		/*
			Fetching returns the data but also steps
			the program counter
		*/
		u16 fetchU16();
		u8 fetchU8();

		/*
			Reading simply reads the data and does not modify
			anything
		*/
		u16 readU16();
		u8 readU8();

		/*
			Returns the state of a status flag
		*/
		u8 getFlag(u8 flag);

		/*
			Set the flag if the condition is true
		*/
		void setFlag(u8 flags, bool condition);
		void setFlag(u8 flags);
		void clearFlag(u8 flags);

		/*
			Maps all opcodes to their instruction
		*/
		void mapOpcodes();



		/*
			Cpu helper functions
		*/

		//8 bit increment
		u8 INC_N(u8& reg);
		//8 bit decrement
		u8 DEC_N(u8& reg);
		//Add reg to HL
		u8 ADD_HL_NN(u16& reg);



		/*
			*Instruction implementations.
			*Returns extra cycles if needed
		*/
		u8 op0x00(); u8 op0x01();        
		u8 op0x02(); u8 op0x03();
		u8 op0x04(); u8 op0x05();
		u8 op0x06(); u8 op0x07();
		u8 op0x08(); u8 op0x09();
		u8 op0x0A(); u8 op0x0B();
		u8 op0x0C(); u8 op0x0D();
		u8 op0x0E(); u8 op0x0F();

		u8 op0x10(); u8 op0x11();
		u8 op0x12(); u8 op0x13();
		u8 op0x14(); u8 op0x15();
		u8 op0x16(); u8 op0x17();
		u8 op0x18(); u8 op0x19();
		u8 op0x1A(); u8 op0x1B();
		u8 op0x1C(); u8 op0x1D();
		u8 op0x1E(); u8 op0x1F();

		u8 op0x20(); u8 op0x21();
		u8 op0x22(); u8 op0x23();
		u8 op0x24(); u8 op0x25();
		u8 op0x26(); u8 op0x27();
		u8 op0x28(); u8 op0x29();
		u8 op0x2A(); u8 op0x2B();
		u8 op0x2C(); u8 op0x2D();
		u8 op0x2E(); u8 op0x2F();

		u8 op0x30(); u8 op0x31();
		u8 op0x32(); u8 op0x33();
		u8 op0x34(); u8 op0x35();
		u8 op0x36(); u8 op0x37();
		u8 op0x38(); u8 op0x39();
		u8 op0x3A(); u8 op0x3B();
		u8 op0x3C(); u8 op0x3D();
		u8 op0x3E(); u8 op0x3F();

		u8 op0x40(); u8 op0x41();
		u8 op0x42(); u8 op0x43();
		u8 op0x44(); u8 op0x45();
		u8 op0x46(); u8 op0x47();
		u8 op0x48(); u8 op0x49();
		u8 op0x4A(); u8 op0x4B();
		u8 op0x4C(); u8 op0x4D();
		u8 op0x4E(); u8 op0x4F();

		u8 op0x50(); u8 op0x51();
		u8 op0x52(); u8 op0x53();
		u8 op0x54(); u8 op0x55();
		u8 op0x56(); u8 op0x57();
		u8 op0x58(); u8 op0x59();
		u8 op0x5A(); u8 op0x5B();
		u8 op0x5C(); u8 op0x5D();
		u8 op0x5E(); u8 op0x5F();

		u8 op0x60(); u8 op0x61();
		u8 op0x62(); u8 op0x63();
		u8 op0x64(); u8 op0x65();
		u8 op0x66(); u8 op0x67();
		u8 op0x68(); u8 op0x69();
		u8 op0x6A(); u8 op0x6B();
		u8 op0x6C(); u8 op0x6D();
		u8 op0x6E(); u8 op0x6F();

		u8 op0x70(); u8 op0x71();
		u8 op0x72(); u8 op0x73();
		u8 op0x74(); u8 op0x75();
		u8 op0x76(); u8 op0x77();
		u8 op0x78(); u8 op0x79();
		u8 op0x7A(); u8 op0x7B();
		u8 op0x7C(); u8 op0x7D();
		u8 op0x7E(); u8 op0x7F();

		u8 op0x80(); u8 op0x81();
		u8 op0x82(); u8 op0x83();
		u8 op0x84(); u8 op0x85();
		u8 op0x86(); u8 op0x87();
		u8 op0x88(); u8 op0x89();
		u8 op0x8A(); u8 op0x8B();
		u8 op0x8C(); u8 op0x8D();
		u8 op0x8E(); u8 op0x8F();

		u8 op0x90(); u8 op0x91();
		u8 op0x92(); u8 op0x93();
		u8 op0x94(); u8 op0x95();
		u8 op0x96(); u8 op0x97();
		u8 op0x98(); u8 op0x99();
		u8 op0x9A(); u8 op0x9B();
		u8 op0x9C(); u8 op0x9D();
		u8 op0x9E(); u8 op0x9F();

		u8 op0xA0(); u8 op0xA1();
		u8 op0xA2(); u8 op0xA3();
		u8 op0xA4(); u8 op0xA5();
		u8 op0xA6(); u8 op0xA7();
		u8 op0xA8(); u8 op0xA9();
		u8 op0xAA(); u8 op0xAB();
		u8 op0xAC(); u8 op0xAD();
		u8 op0xAE(); u8 op0xAF();

		u8 op0xB0(); u8 op0xB1();
		u8 op0xB2(); u8 op0xB3();
		u8 op0xB4(); u8 op0xB5();
		u8 op0xB6(); u8 op0xB7();
		u8 op0xB8(); u8 op0xB9();
		u8 op0xBA(); u8 op0xBB();
		u8 op0xBC(); u8 op0xBD();
		u8 op0xBE(); u8 op0xBF();

		u8 op0xC0(); u8 op0xC1();
		u8 op0xC2(); u8 op0xC3();
		u8 op0xC4(); u8 op0xC5();
		u8 op0xC6(); u8 op0xC7();
		u8 op0xC8(); u8 op0xC9();
		u8 op0xCA(); u8 op0xCB();
		u8 op0xCC(); u8 op0xCD();
		u8 op0xCE(); u8 op0xCF();

		u8 op0xD0(); u8 op0xD1();
		u8 op0xD2(); u8 op0xD3();
		u8 op0xD4(); u8 op0xD5();
		u8 op0xD6(); u8 op0xD7();
		u8 op0xD8(); u8 op0xD9();
		u8 op0xDA(); u8 op0xDB();
		u8 op0xDC(); u8 op0xDD();
		u8 op0xDE(); u8 op0xDF();

		u8 op0xE0(); u8 op0xE1();
		u8 op0xE2(); u8 op0xE3();
		u8 op0xE4(); u8 op0xE5();
		u8 op0xE6(); u8 op0xE7();
		u8 op0xE8(); u8 op0xE9();
		u8 op0xEA(); u8 op0xEB();
		u8 op0xEC(); u8 op0xED();
		u8 op0xEE(); u8 op0xEF();

		u8 op0xF0(); u8 op0xF1();
		u8 op0xF2(); u8 op0xF3();
		u8 op0xF4(); u8 op0xF5();
		u8 op0xF6(); u8 op0xF7();
		u8 op0xF8(); u8 op0xF9();
		u8 op0xFA(); u8 op0xFB();
		u8 op0xFC(); u8 op0xFD();
		u8 op0xFE(); u8 op0xFF();

		/* 
		   *0xCB Table Instruction Implementation 
		   *Returns extra cycles if needed 
		*/





		Register AF;
		Register BC;
		Register DE;
		Register HL;
		
		u16 PC;
		u16 SP;

		u8 cycles = 0;

		//1. Pneumonic
		//2. Function pointer to the opcode implementation
		//3. Cycles requ
		struct Instruction {
			std::string name;
			std::function<u8(void)> execute = nullptr;
			u8 cycles;
		};

		//Opcode associated with function pointer to implementation of instruction
		std::vector<Instruction> table;
		std::vector<Instruction> cbTable;

		MMU* mmu;
	};
}
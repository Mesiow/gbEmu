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
	
	struct MMU;

	union Register {
		struct {
			u8 lo;
			u8 hi;
		};
		u16 value;
	};

	/*
		Flag Positions
	*/
	enum class eFlag : u8{
		Z = 0x80,  //Zero flag
		N = 0x40,  //Subtract flag
		H = 0x20,  //Half Carry flag
		C = 0x10   //Carry flag
	};

	struct Cpu {
		Cpu(MMU *mmu);
		~Cpu();

		void reset();
		void clock();
		
		u8 read(u16 address);
		void write(u16 address, u8 value);

		void connectMMU(MMU* mmu);


		u16 immediateU16();
		u8 immediateU8();

		/*
			Returns the state of a status flag
		*/
		u8 getFlag(eFlag flag);

		/*
			Maps all opcodes to their instruction
		*/
		void mapOpcodes();





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
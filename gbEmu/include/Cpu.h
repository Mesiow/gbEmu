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
			byte lo;
			byte hi;
		};
		word value;
	};

	/*
		Flag Positions
	*/
	enum class eFlag : uint8_t{
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
		
		byte read(word address);
		void write(word address, byte value);

		void connectMMU(MMU* mmu);


		word immediateU16();
		byte immediateU8();

		/*
			Returns the state of a status flag
		*/
		byte getFlag(eFlag flag);

		/*
			Maps all opcodes to their instruction
		*/
		void mapOpcodes();





		/*
			*Instruction implementations.
			*Returns extra cycles if needed
		*/
		uint8_t op0x00(); uint8_t op0x01();        
		uint8_t op0x02(); uint8_t op0x03();
		uint8_t op0x04(); uint8_t op0x05();
		uint8_t op0x06(); uint8_t op0x07();
		uint8_t op0x08(); uint8_t op0x09();
		uint8_t op0x0A(); uint8_t op0x0B();
		uint8_t op0x0C(); uint8_t op0x0D();
		uint8_t op0x0E(); uint8_t op0x0F();

		uint8_t op0x10(); uint8_t op0x11();
		uint8_t op0x12(); uint8_t op0x13();
		uint8_t op0x14(); uint8_t op0x15();
		uint8_t op0x16(); uint8_t op0x17();
		uint8_t op0x18(); uint8_t op0x19();
		uint8_t op0x1A(); uint8_t op0x1B();
		uint8_t op0x1C(); uint8_t op0x1D();
		uint8_t op0x1E(); uint8_t op0x1F();

		uint8_t op0x20(); uint8_t op0x21();
		uint8_t op0x22(); uint8_t op0x23();
		uint8_t op0x24(); uint8_t op0x25();
		uint8_t op0x26(); uint8_t op0x27();
		uint8_t op0x28(); uint8_t op0x29();
		uint8_t op0x2A(); uint8_t op0x2B();
		uint8_t op0x2C(); uint8_t op0x2D();
		uint8_t op0x2E(); uint8_t op0x2F();

		uint8_t op0x30(); uint8_t op0x31();
		uint8_t op0x32(); uint8_t op0x33();
		uint8_t op0x34(); uint8_t op0x35();
		uint8_t op0x36(); uint8_t op0x37();
		uint8_t op0x38(); uint8_t op0x39();
		uint8_t op0x3A(); uint8_t op0x3B();
		uint8_t op0x3C(); uint8_t op0x3D();
		uint8_t op0x3E(); uint8_t op0x3F();

		uint8_t op0x40(); uint8_t op0x41();
		uint8_t op0x42(); uint8_t op0x43();
		uint8_t op0x44(); uint8_t op0x45();
		uint8_t op0x46(); uint8_t op0x47();
		uint8_t op0x48(); uint8_t op0x49();
		uint8_t op0x4A(); uint8_t op0x4B();
		uint8_t op0x4C(); uint8_t op0x4D();
		uint8_t op0x4E(); uint8_t op0x4F();

		uint8_t op0x50(); uint8_t op0x51();
		uint8_t op0x52(); uint8_t op0x53();
		uint8_t op0x54(); uint8_t op0x55();
		uint8_t op0x56(); uint8_t op0x57();
		uint8_t op0x58(); uint8_t op0x59();
		uint8_t op0x5A(); uint8_t op0x5B();
		uint8_t op0x5C(); uint8_t op0x5D();
		uint8_t op0x5E(); uint8_t op0x5F();

		uint8_t op0x60(); uint8_t op0x61();
		uint8_t op0x62(); uint8_t op0x63();
		uint8_t op0x64(); uint8_t op0x65();
		uint8_t op0x66(); uint8_t op0x67();
		uint8_t op0x68(); uint8_t op0x69();
		uint8_t op0x6A(); uint8_t op0x6B();
		uint8_t op0x6C(); uint8_t op0x6D();
		uint8_t op0x6E(); uint8_t op0x6F();

		uint8_t op0x70(); uint8_t op0x71();
		uint8_t op0x72(); uint8_t op0x73();
		uint8_t op0x74(); uint8_t op0x75();
		uint8_t op0x76(); uint8_t op0x77();
		uint8_t op0x78(); uint8_t op0x79();
		uint8_t op0x7A(); uint8_t op0x7B();
		uint8_t op0x7C(); uint8_t op0x7D();
		uint8_t op0x7E(); uint8_t op0x7F();








		Register AF;
		Register BC;
		Register DE;
		Register HL;
		
		uint16_t PC;
		uint16_t SP;

		byte cycles = 0;

		//1. Pneumonic
		//2. Function pointer to the opcode implementation
		//3. Cycles requ
		struct Instruction {
			std::string name;
			std::function<uint8_t(void)> execute = nullptr;
			uint8_t cycles;
		};

		//Opcode associated with function pointer to implementation of instruction
		std::vector<Instruction> table;
		std::vector<Instruction> cbTable;

		MMU* mmu;
	};
}
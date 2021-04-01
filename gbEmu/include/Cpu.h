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
		Cpu();
		~Cpu();

		void reset();
		void clock();
		
		byte read(word address);
		void write(byte value);

		void connectMMU(MMU* mmu);

		/*
			Returns the state of a status flag
		*/
		uint8_t getFlag(eFlag flag);

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

		Register AF;
		Register BC;
		Register DE;
		Register HL;
		
		uint16_t PC;
		uint16_t SP;

		uint8_t cycles = 0;

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
		std::unordered_map<byte, Instruction> cbTable;

		MMU* mmu;
	};
}
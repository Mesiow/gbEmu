#pragma once
#include <cstdint>
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
	using byte = uint8_t;
	using word = uint16_t;

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

		/*
			*Instruction implementations.
			*Returns extra cycles if needed
		*/
		uint8_t opcode0x01(); uint8_t opcode0x06();

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
		std::unordered_map<word, Instruction> table;
	};
}
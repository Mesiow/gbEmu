#include "../include/Cpu.h"
#include "../include/MMU.h"

namespace gbEmu {

	Cpu::Cpu()
	{
		mapOpcodes();
		reset();
	}

	Cpu::~Cpu()
	{
	
		
	}

	void Cpu::reset()
	{
		AF.value = 0x0000;
		BC.value = 0x0000;
		DE.value = 0x0000;
		HL.value = 0x0000;

		PC = 0x0000;
		SP = 0xFFFE;
	}

	void Cpu::clock()
	{
		if (cycles == 0) {
	
			//Read opcode
			byte opcode = read(PC);
			PC++;

			//Handle 0xCB table of opcodes
			if (opcode == 0xCB) {

				byte cb_table_opcode = read(PC);
				Instruction ins = cbTable[cb_table_opcode];

				cycles = ins.cycles;
				cycles += ins.execute();
			}
			else {
				//Handle Regular table of opcodes

				//Get instruction associated with that opcode
				Instruction ins = table[opcode];

				//Base number of cycles required
				cycles = ins.cycles;
				//May require more cycles based on certain conditions
				cycles += ins.execute();
			}
		}
		cycles--;
	}

	byte Cpu::read(word address)
	{
		return byte();
	}

	void Cpu::write(byte value)
	{

	}

	void Cpu::connectMMU(MMU* mmu)
	{
		this->mmu = mmu;
	}

	uint8_t Cpu::getFlag(eFlag flag)
	{
		uint8_t f = AF.lo;
		return ((f & (uint8_t)flag) > 0 ? 1 : 0);
	}

	uint8_t Cpu::op0x00()
	{
		return uint8_t();
	}

	uint8_t Cpu::op0x01()
	{
		return 0;
	}

	uint8_t Cpu::op0x02()
	{
		return uint8_t();
	}

	uint8_t Cpu::op0x03()
	{
		return uint8_t();
	}

	uint8_t Cpu::op0x04()
	{
		return uint8_t();
	}

	uint8_t Cpu::op0x05()
	{
		return uint8_t();
	}

	uint8_t Cpu::op0x06()
	{
		return 0;
	}
	uint8_t Cpu::op0x07()
	{
		return uint8_t();
	}
}

#include "../include/Cpu.h"
#include "../include/MMU.h"

namespace gbEmu {

	Cpu::Cpu(MMU* mmu)
	{
		connectMMU(mmu);

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

		//Test LD nn, n
		//mmu->write(0x00, 0x01); //opcode
		//mmu->write(0x01, 0x15); //16 bit values
		//mmu->write(0x02, 0x32);

		//clock();
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
		return mmu->read(address);
	}

	void Cpu::write(word address, byte value)
	{
		mmu->write(address, value);
	}

	void Cpu::connectMMU(MMU* mmu)
	{
		this->mmu = mmu;
	}

	word Cpu::immediateU16()
	{
		byte lo = read(PC++);
		byte hi = read(PC++);
		return ((hi << 8) | lo);
	}

	byte Cpu::immediateU8()
	{
		return byte();
	}

	byte Cpu::getFlag(eFlag flag)
	{
		byte f = AF.lo;
		return ((f & (byte)flag) > 0 ? 1 : 0);
	}


	/*
		Instruction Implementations
	*/

	uint8_t Cpu::op0x00()
	{
		return 0;
	}

	uint8_t Cpu::op0x01()
	{
		BC.value = immediateU16();
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

	uint8_t Cpu::op0x08()
	{
		return uint8_t();
	}

	uint8_t Cpu::op0x09()
	{
		return uint8_t();
	}

	uint8_t Cpu::op0x0A()
	{
		return uint8_t();
	}

	uint8_t Cpu::op0x0B()
	{
		return uint8_t();
	}

	uint8_t Cpu::op0x0C()
	{
		return uint8_t();
	}

	uint8_t Cpu::op0x0D()
	{
		return uint8_t();
	}

	uint8_t Cpu::op0x0E()
	{
		return uint8_t();
	}

	uint8_t Cpu::op0x0F()
	{
		return uint8_t();
	}

	uint8_t Cpu::op0x10()
	{
		return uint8_t();
	}

	uint8_t Cpu::op0x11()
	{
		return uint8_t();
	}
	uint8_t Cpu::op0x12()
	{
		return uint8_t();
	}

	uint8_t Cpu::op0x13()
	{
		return uint8_t();
	}

	uint8_t Cpu::op0x14()
	{
		return uint8_t();
	}

	uint8_t Cpu::op0x15()
	{
		return uint8_t();
	}
	uint8_t Cpu::op0x16()
	{
		return uint8_t();
	}

	uint8_t Cpu::op0x17()
	{
		return uint8_t();
	}

	uint8_t Cpu::op0x18()
	{
		return uint8_t();
	}

	uint8_t Cpu::op0x19()
	{
		return uint8_t();
	}

	uint8_t Cpu::op0x1A()
	{
		return uint8_t();
	}

	uint8_t Cpu::op0x1B()
	{
		return uint8_t();
	}

	uint8_t Cpu::op0x1C()
	{
		return uint8_t();
	}

	uint8_t Cpu::op0x1D()
	{
		return uint8_t();
	}

	uint8_t Cpu::op0x1E()
	{
		return uint8_t();
	}

	uint8_t Cpu::op0x1F()
	{
		return uint8_t();
	}
	uint8_t Cpu::op0x20()
	{
		return uint8_t();
	}
	uint8_t Cpu::op0x21()
	{
		return uint8_t();
	}
	uint8_t Cpu::op0x22()
	{
		return uint8_t();
	}
	uint8_t Cpu::op0x23()
	{
		return uint8_t();
	}
	uint8_t Cpu::op0x24()
	{
		return uint8_t();
	}
	uint8_t Cpu::op0x25()
	{
		return uint8_t();
	}
	uint8_t Cpu::op0x26()
	{
		return uint8_t();
	}
	uint8_t Cpu::op0x27()
	{
		return uint8_t();
	}
	uint8_t Cpu::op0x28()
	{
		return uint8_t();
	}
	uint8_t Cpu::op0x29()
	{
		return uint8_t();
	}
	uint8_t Cpu::op0x2A()
	{
		return uint8_t();
	}
	uint8_t Cpu::op0x2B()
	{
		return uint8_t();
	}
	uint8_t Cpu::op0x2C()
	{
		return uint8_t();
	}
	uint8_t Cpu::op0x2D()
	{
		return uint8_t();
	}
	uint8_t Cpu::op0x2E()
	{
		return uint8_t();
	}
	uint8_t Cpu::op0x2F()
	{
		return uint8_t();
	}
}

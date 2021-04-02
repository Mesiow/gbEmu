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
	}

	void Cpu::clock()
	{
		if (cycles == 0) {
	
			//Read opcode
			u8 opcode = read(PC);
			PC++;

			//Handle 0xCB table of opcodes
			if (opcode == 0xCB) {

				u8 cb_table_opcode = read(PC);
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

	u8 Cpu::read(u16 address)
	{
		return mmu->read(address);
	}

	void Cpu::write(u16 address, u8 value)
	{
		mmu->write(address, value);
	}

	void Cpu::connectMMU(MMU* mmu)
	{
		this->mmu = mmu;
	}

	u16 Cpu::immediateU16()
	{
		u8 lo = read(PC++);
		u8 hi = read(PC++);
		return ((hi << 8) | lo);
	}

	u8 Cpu::immediateU8()
	{
		return u8();
	}

	u8 Cpu::getFlag(eFlag flag)
	{
		u8 f = AF.lo;
		return ((f & (u8)flag) > 0 ? 1 : 0);
	}


	/*
		Instruction Implementations
	*/

	u8 Cpu::op0x00()
	{
		return 0;
	}

	u8 Cpu::op0x01()
	{
		BC.value = immediateU16();
		return 0;
	}

	u8 Cpu::op0x02()
	{
		return u8();
	}

	u8 Cpu::op0x03()
	{
		return u8();
	}

	u8 Cpu::op0x04()
	{
		return u8();
	}

	u8 Cpu::op0x05()
	{
		return u8();
	}

	u8 Cpu::op0x06()
	{
		return 0;
	}

	u8 Cpu::op0x07()
	{
		return u8();
	}

	u8 Cpu::op0x08()
	{
		return u8();
	}

	u8 Cpu::op0x09()
	{
		return u8();
	}

	u8 Cpu::op0x0A()
	{
		return u8();
	}

	u8 Cpu::op0x0B()
	{
		return u8();
	}

	u8 Cpu::op0x0C()
	{
		return u8();
	}

	u8 Cpu::op0x0D()
	{
		return u8();
	}

	u8 Cpu::op0x0E()
	{
		return u8();
	}

	u8 Cpu::op0x0F()
	{
		return u8();
	}

	u8 Cpu::op0x10()
	{
		return u8();
	}

	u8 Cpu::op0x11()
	{
		return u8();
	}
	u8 Cpu::op0x12()
	{
		return u8();
	}

	u8 Cpu::op0x13()
	{
		return u8();
	}

	u8 Cpu::op0x14()
	{
		return u8();
	}

	u8 Cpu::op0x15()
	{
		return u8();
	}
	u8 Cpu::op0x16()
	{
		return u8();
	}

	u8 Cpu::op0x17()
	{
		return u8();
	}

	u8 Cpu::op0x18()
	{
		return u8();
	}

	u8 Cpu::op0x19()
	{
		return u8();
	}

	u8 Cpu::op0x1A()
	{
		return u8();
	}

	u8 Cpu::op0x1B()
	{
		return u8();
	}

	u8 Cpu::op0x1C()
	{
		return u8();
	}

	u8 Cpu::op0x1D()
	{
		return u8();
	}

	u8 Cpu::op0x1E()
	{
		return u8();
	}

	u8 Cpu::op0x1F()
	{
		return u8();
	}
	u8 Cpu::op0x20()
	{
		return u8();
	}
	u8 Cpu::op0x21()
	{
		return u8();
	}
	u8 Cpu::op0x22()
	{
		return u8();
	}
	u8 Cpu::op0x23()
	{
		return u8();
	}
	u8 Cpu::op0x24()
	{
		return u8();
	}
	u8 Cpu::op0x25()
	{
		return u8();
	}
	u8 Cpu::op0x26()
	{
		return u8();
	}
	u8 Cpu::op0x27()
	{
		return u8();
	}
	u8 Cpu::op0x28()
	{
		return u8();
	}
	u8 Cpu::op0x29()
	{
		return u8();
	}
	u8 Cpu::op0x2A()
	{
		return u8();
	}
	u8 Cpu::op0x2B()
	{
		return u8();
	}
	u8 Cpu::op0x2C()
	{
		return u8();
	}
	u8 Cpu::op0x2D()
	{
		return u8();
	}
	u8 Cpu::op0x2E()
	{
		return u8();
	}
	u8 Cpu::op0x2F()
	{
		return u8();
	}
	u8 Cpu::op0x30()
	{
		return u8();
	}
	u8 Cpu::op0x31()
	{
		return u8();
	}
	u8 Cpu::op0x32()
	{
		return u8();
	}
	u8 Cpu::op0x33()
	{
		return u8();
	}
	u8 Cpu::op0x34()
	{
		return u8();
	}
	u8 Cpu::op0x35()
	{
		return u8();
	}
	u8 Cpu::op0x36()
	{
		return u8();
	}
	u8 Cpu::op0x37()
	{
		return u8();
	}
	u8 Cpu::op0x38()
	{
		return u8();
	}
	u8 Cpu::op0x39()
	{
		return u8();
	}
	u8 Cpu::op0x3A()
	{
		return u8();
	}
	u8 Cpu::op0x3B()
	{
		return u8();
	}
	u8 Cpu::op0x3C()
	{
		return u8();
	}
	u8 Cpu::op0x3D()
	{
		return u8();
	}
	u8 Cpu::op0x3E()
	{
		return u8();
	}
	u8 Cpu::op0x3F()
	{
		return u8();
	}
	u8 Cpu::op0x40()
	{
		return u8();
	}
	u8 Cpu::op0x41()
	{
		return u8();
	}
	u8 Cpu::op0x42()
	{
		return u8();
	}
	u8 Cpu::op0x43()
	{
		return u8();
	}
	u8 Cpu::op0x44()
	{
		return u8();
	}
	u8 Cpu::op0x45()
	{
		return u8();
	}
	u8 Cpu::op0x46()
	{
		return u8();
	}
	u8 Cpu::op0x47()
	{
		return u8();
	}
	u8 Cpu::op0x48()
	{
		return u8();
	}
	u8 Cpu::op0x49()
	{
		return u8();
	}
	u8 Cpu::op0x4A()
	{
		return u8();
	}
	u8 Cpu::op0x4B()
	{
		return u8();
	}
	u8 Cpu::op0x4C()
	{
		return u8();
	}
	u8 Cpu::op0x4D()
	{
		return u8();
	}
	u8 Cpu::op0x4E()
	{
		return u8();
	}
	u8 Cpu::op0x4F()
	{
		return u8();
	}
	u8 Cpu::op0x50()
	{
		return u8();
	}
	u8 Cpu::op0x51()
	{
		return u8();
	}
	u8 Cpu::op0x52()
	{
		return u8();
	}
	u8 Cpu::op0x53()
	{
		return u8();
	}
	u8 Cpu::op0x54()
	{
		return u8();
	}
	u8 Cpu::op0x55()
	{
		return u8();
	}
	u8 Cpu::op0x56()
	{
		return u8();
	}
	u8 Cpu::op0x57()
	{
		return u8();
	}
	u8 Cpu::op0x58()
	{
		return u8();
	}
	u8 Cpu::op0x59()
	{
		return u8();
	}
	u8 Cpu::op0x5A()
	{
		return u8();
	}
	u8 Cpu::op0x5B()
	{
		return u8();
	}
	u8 Cpu::op0x5C()
	{
		return u8();
	}
	u8 Cpu::op0x5D()
	{
		return u8();
	}
	u8 Cpu::op0x5E()
	{
		return u8();
	}
	u8 Cpu::op0x5F()
	{
		return u8();
	}
	u8 Cpu::op0x60()
	{
		return u8();
	}
	u8 Cpu::op0x61()
	{
		return u8();
	}
	u8 Cpu::op0x62()
	{
		return u8();
	}
	u8 Cpu::op0x63()
	{
		return u8();
	}
	u8 Cpu::op0x64()
	{
		return u8();
	}
	u8 Cpu::op0x65()
	{
		return u8();
	}
	u8 Cpu::op0x66()
	{
		return u8();
	}
	u8 Cpu::op0x67()
	{
		return u8();
	}
	u8 Cpu::op0x68()
	{
		return u8();
	}
	u8 Cpu::op0x69()
	{
		return u8();
	}
	u8 Cpu::op0x6A()
	{
		return u8();
	}
	u8 Cpu::op0x6B()
	{
		return u8();
	}
	u8 Cpu::op0x6C()
	{
		return u8();
	}
	u8 Cpu::op0x6D()
	{
		return u8();
	}
	u8 Cpu::op0x6E()
	{
		return u8();
	}
	u8 Cpu::op0x6F()
	{
		return u8();
	}
	u8 Cpu::op0x70()
	{
		return u8();
	}
	u8 Cpu::op0x71()
	{
		return u8();
	}
	u8 Cpu::op0x72()
	{
		return u8();
	}
	u8 Cpu::op0x73()
	{
		return u8();
	}
	u8 Cpu::op0x74()
	{
		return u8();
	}
	u8 Cpu::op0x75()
	{
		return u8();
	}
	u8 Cpu::op0x76()
	{
		return u8();
	}
	u8 Cpu::op0x77()
	{
		return u8();
	}
	u8 Cpu::op0x78()
	{
		return u8();
	}
	u8 Cpu::op0x79()
	{
		return u8();
	}
	u8 Cpu::op0x7A()
	{
		return u8();
	}
	u8 Cpu::op0x7B()
	{
		return u8();
	}
	u8 Cpu::op0x7C()
	{
		return u8();
	}
	u8 Cpu::op0x7D()
	{
		return u8();
	}
	u8 Cpu::op0x7E()
	{
		return u8();
	}
	u8 Cpu::op0x7F()
	{
		return u8();
	}
}

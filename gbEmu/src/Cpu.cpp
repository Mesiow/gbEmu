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

		testFunc();
	}

	void Cpu::testFunc()
	{
		//Test LD n, nn 
		//mmu->write(0x00, 0x01); //opcode
		//mmu->write(0x01, 0x15); //16 bit values
		//mmu->write(0x02, 0x32);

		////Test LD (BC), A
		//AF.hi = 0x39;
		//BC.value = 0x151;
		//mmu->write(0x00, 0x02); //opcode

		//Test INC NN
		//mmu->write(0x00, 0x03);

		//Test INC N
		//BC.hi = 0xF;
		//mmu->write(0x0, 0x04);

		//Test DEC N
		//BC.hi = 0x0;
		//mmu->write(0x0, 0x05);

		//Test LD N, u8
		//mmu->write(0x00, 0x06);
		//mmu->write(0x01, 0x48);

		//Test RLCA
		//AF.hi = 0x80;
		//mmu->write(0x00, 0x07);

		//Test LD (u16), SP
		//mmu->write(0x00, 0x08); //opcode
		//mmu->write(0x01, 0x10);
		//mmu->write(0x02, 0x01);

		//Test ADD HL, NN
		//HL.value = 0xFFFF;
		//BC.value = 0x0001;
		//mmu->write(0x0, 0x09);

		//Test LD N, (NN)
		//BC.value = 0x100;
		//mmu->write(0x0, 0x0A);
		//mmu->write(BC.value, 0x14);

		//Test DEC NN
		//BC.value = 0x0010;
		//mmu->write(0x0, 0x0B);

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

	
	u16 Cpu::fetchU16()
	{
		u8 lo = read(PC++);
		u8 hi = read(PC++);
		return ((hi << 8) | lo);
	}

	u8 Cpu::fetchU8()
	{
		u8 data = read(PC++);
		return data;
	}

	u16 Cpu::readU16()
	{
		u8 lo = read(PC);
		u8 hi = read(PC + 1);
		return ((hi << 8) | lo);
	}

	u8 Cpu::readU8()
	{
		u8 data = read(PC);
		return data;
	}

	u8 Cpu::getFlag(u8 flag)
	{
		u8 f = AF.lo;
		return ((f & flag) > 0 ? 1 : 0);
	}

	void Cpu::setFlag(u8 flags, bool condition)
	{
		if (condition) {
			setFlag(flags);
		}
		else {
			clearFlag(flags);
		}
	}

	void Cpu::setFlag(u8 flags)
	{
		u8 fl = flags & 0xF0;
		if (fl & FLAG_Z) {
			AF.lo = setBit(AF.lo, FLAG_BIT_Z);
		}
		if (fl & FLAG_N) {
			AF.lo = setBit(AF.lo, FLAG_BIT_N);
		}
		if (fl & FLAG_H) {
			AF.lo = setBit(AF.lo, FLAG_BIT_H);
		}
		if (fl & FLAG_C) {
			AF.lo = setBit(AF.lo, FLAG_BIT_C);
		}
	}

	void Cpu::clearFlag(u8 flags)
	{
		u8 fl = flags & 0xF0;
		if (fl & FLAG_Z) {
			AF.lo = resetBit(AF.lo, FLAG_BIT_Z);
		}
		if (fl & FLAG_N) {
			AF.lo = resetBit(AF.lo, FLAG_BIT_N);
		}
		if (fl & FLAG_H) {
			AF.lo = resetBit(AF.lo, FLAG_BIT_H);
		}
		if (fl & FLAG_C) {
			AF.lo = resetBit(AF.lo, FLAG_BIT_C);
		}
	}


	/*
		Instruction Implementations
	*/

	u8 Cpu::op0x00()
	{
		//NOP - do nothing
		return 0;
	}

	u8 Cpu::op0x01()
	{
		BC.value = fetchU16();
		return 0;
	}

	u8 Cpu::op0x02()
	{
		mmu->write(BC.value, AF.hi);
		return 0;
	}

	u8 Cpu::op0x03()
	{
		BC.value++;
		return 0;
	}

	u8 Cpu::op0x04()
	{
		u8 result = BC.hi + 1;

		setFlag(FLAG_Z, (result == 0));
		clearFlag(FLAG_N);
		setFlag(FLAG_H, ((BC.hi & 0xF) + 1) > 0xF);

		BC.hi++;
		return 0;
	}

	u8 Cpu::op0x05()
	{
		s8 result = BC.hi - 1;

		setFlag(FLAG_Z, (result == 0));
		setFlag(FLAG_N);
		setFlag(FLAG_H, ((BC.hi & 0xF) - 1) < 0);

		BC.hi--;
		return 0;
	}

	u8 Cpu::op0x06()
	{
		u8 data = fetchU8();
		BC.hi = data;
		return 0;
	}

	u8 Cpu::op0x07()
	{
		u8 msb = ((AF.hi & 0x80) >> 7);
		u8 result = (AF.hi << 1);

		clearFlag(FLAG_Z);
		clearFlag(FLAG_N);
		clearFlag(FLAG_H);

		//Carry will store old bit 7 data
		if (msb)
			setFlag(FLAG_C);
		else
			clearFlag(FLAG_C);

		AF.hi <<= 1;
		return 0;
	}

	u8 Cpu::op0x08()
	{
		u16 addr = fetchU16();
		
		u8 lo = (SP & 0xFF);
		u8 hi = ((SP >> 8) & 0xFF);

		write(addr, lo);
		write(addr + 1, hi);
		return 0;
	}

	u8 Cpu::op0x09()
	{
		u16 result = HL.value + BC.value;

		clearFlag(FLAG_N);
		setFlag(FLAG_H, (HL.value & 0xFFF) + (BC.value & 0xFFF) > 0xFFF); //set if overflow from bit 11
		setFlag(FLAG_C, (HL.value & 0xFFFF) + (BC.value & 0xFFFF) > 0xFFFF); //set if overflow from bit 15

		HL.value += BC.value;
		return 0;
	}

	u8 Cpu::op0x0A()
	{
		u8 data = read(BC.value);
		AF.hi = data;
		return 0;
	}

	u8 Cpu::op0x0B()
	{
		BC.value--;
		return 0;
	}

	u8 Cpu::op0x0C()
	{
		u8 result = BC.lo + 1;

		setFlag(FLAG_Z, (result == 0));
		clearFlag(FLAG_N);
		setFlag(FLAG_H, ((BC.lo & 0xF) + 1) > 0xF);

		BC.lo++;
		return 0;
	}

	u8 Cpu::op0x0D()
	{
		s8 result = BC.lo - 1;

		setFlag(FLAG_Z, (result == 0));
		setFlag(FLAG_N);
		setFlag(FLAG_H, ((BC.lo & 0xF) - 1) < 0);

		BC.lo--;
		return 0;
	}

	u8 Cpu::op0x0E()
	{
		u8 data = fetchU8();
		BC.lo = data;
		return 0;
	}

	u8 Cpu::op0x0F()
	{
		u8 lsb = (AF.hi & 0x1);
		u8 result = (AF.hi >> 1) & 0xFF;

		clearFlag(FLAG_Z);
		clearFlag(FLAG_N);
		clearFlag(FLAG_H);

		if (lsb)
			setFlag(FLAG_C);
		else
			clearFlag(FLAG_C);
		
		AF.hi >>= 1;
		return 0;
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
	u8 Cpu::op0x80()
	{
		return u8();
	}
	u8 Cpu::op0x81()
	{
		return u8();
	}
	u8 Cpu::op0x82()
	{
		return u8();
	}
	u8 Cpu::op0x83()
	{
		return u8();
	}
	u8 Cpu::op0x84()
	{
		return u8();
	}
	u8 Cpu::op0x85()
	{
		return u8();
	}
	u8 Cpu::op0x86()
	{
		return u8();
	}
	u8 Cpu::op0x87()
	{
		return u8();
	}
	u8 Cpu::op0x88()
	{
		return u8();
	}
	u8 Cpu::op0x89()
	{
		return u8();
	}
	u8 Cpu::op0x8A()
	{
		return u8();
	}
	u8 Cpu::op0x8B()
	{
		return u8();
	}
	u8 Cpu::op0x8C()
	{
		return u8();
	}
	u8 Cpu::op0x8D()
	{
		return u8();
	}
	u8 Cpu::op0x8E()
	{
		return u8();
	}
	u8 Cpu::op0x8F()
	{
		return u8();
	}
	u8 Cpu::op0x90()
	{
		return u8();
	}
	u8 Cpu::op0x91()
	{
		return u8();
	}
	u8 Cpu::op0x92()
	{
		return u8();
	}
	u8 Cpu::op0x93()
	{
		return u8();
	}
	u8 Cpu::op0x94()
	{
		return u8();
	}
	u8 Cpu::op0x95()
	{
		return u8();
	}
	u8 Cpu::op0x96()
	{
		return u8();
	}
	u8 Cpu::op0x97()
	{
		return u8();
	}
	u8 Cpu::op0x98()
	{
		return u8();
	}
	u8 Cpu::op0x99()
	{
		return u8();
	}
	u8 Cpu::op0x9A()
	{
		return u8();
	}
	u8 Cpu::op0x9B()
	{
		return u8();
	}
	u8 Cpu::op0x9C()
	{
		return u8();
	}
	u8 Cpu::op0x9D()
	{
		return u8();
	}
	u8 Cpu::op0x9E()
	{
		return u8();
	}
	u8 Cpu::op0x9F()
	{
		return u8();
	}
	u8 Cpu::op0xA0()
	{
		return u8();
	}
	u8 Cpu::op0xA1()
	{
		return u8();
	}
	u8 Cpu::op0xA2()
	{
		return u8();
	}
	u8 Cpu::op0xA3()
	{
		return u8();
	}
	u8 Cpu::op0xA4()
	{
		return u8();
	}
	u8 Cpu::op0xA5()
	{
		return u8();
	}
	u8 Cpu::op0xA6()
	{
		return u8();
	}
	u8 Cpu::op0xA7()
	{
		return u8();
	}
	u8 Cpu::op0xA8()
	{
		return u8();
	}
	u8 Cpu::op0xA9()
	{
		return u8();
	}
	u8 Cpu::op0xAA()
	{
		return u8();
	}
	u8 Cpu::op0xAB()
	{
		return u8();
	}
	u8 Cpu::op0xAC()
	{
		return u8();
	}
	u8 Cpu::op0xAD()
	{
		return u8();
	}
	u8 Cpu::op0xAE()
	{
		return u8();
	}
	u8 Cpu::op0xAF()
	{
		return u8();
	}
	u8 Cpu::op0xB0()
	{
		return u8();
	}
	u8 Cpu::op0xB1()
	{
		return u8();
	}
	u8 Cpu::op0xB2()
	{
		return u8();
	}
	u8 Cpu::op0xB3()
	{
		return u8();
	}
	u8 Cpu::op0xB4()
	{
		return u8();
	}
	u8 Cpu::op0xB5()
	{
		return u8();
	}
	u8 Cpu::op0xB6()
	{
		return u8();
	}
	u8 Cpu::op0xB7()
	{
		return u8();
	}
	u8 Cpu::op0xB8()
	{
		return u8();
	}
	u8 Cpu::op0xB9()
	{
		return u8();
	}
	u8 Cpu::op0xBA()
	{
		return u8();
	}
	u8 Cpu::op0xBB()
	{
		return u8();
	}
	u8 Cpu::op0xBC()
	{
		return u8();
	}
	u8 Cpu::op0xBD()
	{
		return u8();
	}
	u8 Cpu::op0xBE()
	{
		return u8();
	}
	u8 Cpu::op0xBF()
	{
		return u8();
	}
	u8 Cpu::op0xC0()
	{
		return u8();
	}
	u8 Cpu::op0xC1()
	{
		return u8();
	}
	u8 Cpu::op0xC2()
	{
		return u8();
	}
	u8 Cpu::op0xC3()
	{
		return u8();
	}
	u8 Cpu::op0xC4()
	{
		return u8();
	}
	u8 Cpu::op0xC5()
	{
		return u8();
	}
	u8 Cpu::op0xC6()
	{
		return u8();
	}
	u8 Cpu::op0xC7()
	{
		return u8();
	}
	u8 Cpu::op0xC8()
	{
		return u8();
	}
	u8 Cpu::op0xC9()
	{
		return u8();
	}
	u8 Cpu::op0xCA()
	{
		return u8();
	}
	u8 Cpu::op0xCB()
	{
		return u8();
	}
	u8 Cpu::op0xCC()
	{
		return u8();
	}
	u8 Cpu::op0xCD()
	{
		return u8();
	}
	u8 Cpu::op0xCE()
	{
		return u8();
	}
	u8 Cpu::op0xCF()
	{
		return u8();
	}
	u8 Cpu::op0xD0()
	{
		return u8();
	}
	u8 Cpu::op0xD1()
	{
		return u8();
	}
	u8 Cpu::op0xD2()
	{
		return u8();
	}
	u8 Cpu::op0xD3()
	{
		return u8();
	}
	u8 Cpu::op0xD4()
	{
		return u8();
	}
	u8 Cpu::op0xD5()
	{
		return u8();
	}
	u8 Cpu::op0xD6()
	{
		return u8();
	}
	u8 Cpu::op0xD7()
	{
		return u8();
	}
	u8 Cpu::op0xD8()
	{
		return u8();
	}
	u8 Cpu::op0xD9()
	{
		return u8();
	}
	u8 Cpu::op0xDA()
	{
		return u8();
	}
	u8 Cpu::op0xDB()
	{
		return u8();
	}
	u8 Cpu::op0xDC()
	{
		return u8();
	}
	u8 Cpu::op0xDD()
	{
		return u8();
	}
	u8 Cpu::op0xDE()
	{
		return u8();
	}
	u8 Cpu::op0xDF()
	{
		return u8();
	}
	u8 Cpu::op0xE0()
	{
		return u8();
	}
	u8 Cpu::op0xE1()
	{
		return u8();
	}
	u8 Cpu::op0xE2()
	{
		return u8();
	}
	u8 Cpu::op0xE3()
	{
		return u8();
	}
	u8 Cpu::op0xE4()
	{
		return u8();
	}
	u8 Cpu::op0xE5()
	{
		return u8();
	}
	u8 Cpu::op0xE6()
	{
		return u8();
	}
	u8 Cpu::op0xE7()
	{
		return u8();
	}
	u8 Cpu::op0xE8()
	{
		return u8();
	}
	u8 Cpu::op0xE9()
	{
		return u8();
	}
	u8 Cpu::op0xEA()
	{
		return u8();
	}
	u8 Cpu::op0xEB()
	{
		return u8();
	}
	u8 Cpu::op0xEC()
	{
		return u8();
	}
	u8 Cpu::op0xED()
	{
		return u8();
	}
	u8 Cpu::op0xEE()
	{
		return u8();
	}
	u8 Cpu::op0xEF()
	{
		return u8();
	}
	u8 Cpu::op0xF0()
	{
		return u8();
	}
	u8 Cpu::op0xF1()
	{
		return u8();
	}
	u8 Cpu::op0xF2()
	{
		return u8();
	}
	u8 Cpu::op0xF3()
	{
		return u8();
	}
	u8 Cpu::op0xF4()
	{
		return u8();
	}
	u8 Cpu::op0xF5()
	{
		return u8();
	}
	u8 Cpu::op0xF6()
	{
		return u8();
	}
	u8 Cpu::op0xF7()
	{
		return u8();
	}
	u8 Cpu::op0xF8()
	{
		return u8();
	}
	u8 Cpu::op0xF9()
	{
		return u8();
	}
	u8 Cpu::op0xFA()
	{
		return u8();
	}
	u8 Cpu::op0xFB()
	{
		return u8();
	}
	u8 Cpu::op0xFC()
	{
		return u8();
	}
	u8 Cpu::op0xFD()
	{
		return u8();
	}
	u8 Cpu::op0xFE()
	{
		return u8();
	}
	u8 Cpu::op0xFF()
	{
		return u8();
	}
}

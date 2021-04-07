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

		//Test RRCA
		//AF.hi = 0x19;
		//mmu->write(0x0, 0x0F);

		//Test RLA
		//AF.hi = 0x25;
		//mmu->write(0x0, 0x17);

		//Test JR i8
		//mmu->write(0x0, 0x18);
		//mmu->write(0x1, 0x15);

		//Test RRA
		//AF.hi = 0x24;
		//AF.lo = 0x10;
		//mmu->write(0x0, 0x1F);

		//Test JR NZ, i8
		//AF.lo = 0x00;
		//mmu->write(0x0, 0x20);
		//mmu->write(0x1, 0x4);

		//Test LD (HL+), A
		//AF.hi = 0x6;
		//mmu->write(0x0, 0x22);

		//Test INC, (HL)
		//HL.value = 0x05;
		//mmu->write(0x0, 0x34);
		//mmu->write(0x05, 0x40);

		//Test LD (HL), u8
		//HL.value = 0x05;
		//mmu->write(0x0, 0x36);
		//mmu->write(0x1, 0x21);

		//Test CPL
		//AF.hi = 0x4;
		//mmu->write(0x0, 0x2F);

		//Test SCF
		//mmu->write(0x0, 0x37);

		//Test CCF
		//AF.lo = 0x00;
		//mmu->write(0x0, 0x3F);

		//Test ADD_A_N
		//AF.hi = 0x2;
		//BC.hi = 0xFE;
		//mmu->write(0x0, 0x80);

		//Test ADC_A_N
		//AF.hi = 0xFE;
		//AF.lo = 0x10;
		//BC.hi = 0x2;
		//mmu->write(0x0, 0x88);

		//Test SUB_A_N
		//AF.hi = 0x10;
		//BC.hi = 0x5;
		//mmu->write(0x0, 0x90);

        //Test PUSH_NN
		//DE.value = 0x1425;
		//mmu->write(0x0, 0xC5);

		//Test POP_NN
		//mmu->write(0x0, 0xC1);
		
		//Test CALL 
		//mmu->write(0x0, 0xCD);
		//mmu->write(0x1, 0x40);
		//mmu->write(0x2, 0x1);
		//Test RET
		//mmu->write(0x140, 0xC9);		
	}

	void Cpu::clock()
	{
		if (cycles == 0) {
			//Read opcode
			u8 opcode = read(PC);
			PC++;

			//Get instruction associated with that opcode
			Instruction ins = table[opcode];

			//Base number of cycles required
			cycles = ins.cycles;
			//May require more cycles based on certain conditions
			cycles += ins.execute();
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
		Cpu helper functions
	*/
	u8 Cpu::INC_N(u8& reg) {
		u8 result = reg + 1;

		setFlag(FLAG_Z, (result == 0));
		clearFlag(FLAG_N);
		setFlag(FLAG_H, ((reg & 0xF) + 1) > 0xF);

		reg++;
		return 0;
	}

	u8 Cpu::DEC_N(u8& reg) {
		u8 result = reg - 1;

		setFlag(FLAG_Z, (result == 0));
		setFlag(FLAG_N);
		setFlag(FLAG_H, ((reg & 0xF) - 1) < 0);

		reg--;
		return 0;
	}

	u8 Cpu::ADD_HL_NN(u16 reg)
	{
		u16 result = HL.value + reg;

		clearFlag(FLAG_N);
		setFlag(FLAG_H, (HL.value & 0xFFF) + (reg & 0xFFF) > 0xFFF); //set if overflow from bit 11
		setFlag(FLAG_C, (HL.value & 0xFFFF) + (reg & 0xFFFF) > 0xFFFF); //set if overflow from bit 15
	
		HL.value += reg;
		return 0;
	}

	u8 Cpu::ADD_A_N(u8 reg)
	{
		u8 result = AF.hi + reg;

		setFlag(FLAG_Z, (result == 0));
		clearFlag(FLAG_N);
		setFlag(FLAG_H, (AF.hi & 0xF) + (reg & 0xF) > 0xF);
		setFlag(FLAG_C, (AF.hi & 0xFF) + (reg & 0xFF) > 0xFF);
		
		AF.hi += reg;
		return 0;
	}

	u8 Cpu::ADC_A_N(u8 reg)
	{
		u8 carry = getFlag(FLAG_C);
		u8 result = AF.hi + reg + carry;

		setFlag(FLAG_Z, (result == 0));
		clearFlag(FLAG_N);
		setFlag(FLAG_H, (AF.hi & 0xF) + (reg & 0xF) + (carry) > 0xF);
		setFlag(FLAG_C, (AF.hi & 0xFF) + (reg & 0xFF) + (carry) > 0xFF);

		AF.hi = result;
		return 0;
	}

	u8 Cpu::SUB_A_N(u8 reg)
	{
		u8 result = AF.hi - reg;

		setFlag(FLAG_Z, (result == 0));
		setFlag(FLAG_N);
		setFlag(FLAG_H, ((AF.hi & 0xF) - (reg & 0xF)) < 0);
		setFlag(FLAG_C, (reg > AF.hi));

		AF.hi -= reg;
		return 0;
	}

	u8 Cpu::SBC_A_N(u8 reg)
	{
		u8 carry = getFlag(FLAG_C);
		u8 result = AF.hi - (reg + carry);

		setFlag(FLAG_Z, (result == 0));
		setFlag(FLAG_N);
		setFlag(FLAG_H, ((AF.hi & 0xF) - (reg & 0xF) - carry) < 0);
		setFlag(FLAG_C, ((reg + carry) > AF.hi));

		AF.hi = AF.hi - (reg + carry);
		return 0;
	}

	u8 Cpu::AND_A_N(u8 reg)
	{
		u8 result = AF.hi & reg;

		setFlag(FLAG_Z, (result == 0));
		clearFlag(FLAG_N);
		setFlag(FLAG_H);
		clearFlag(FLAG_C);

		AF.hi &= reg;
		return 0;
	}

	u8 Cpu::XOR_A_N(u8 reg)
	{
		u8 result = AF.hi ^ reg;

		setFlag(FLAG_Z, (result == 0));
		clearFlag((FLAG_N | FLAG_H | FLAG_C));

		AF.hi ^= reg;
		return 0;
	}

	u8 Cpu::OR_A_N(u8 reg)
	{
		u8 result = AF.hi | reg;

		setFlag(FLAG_Z, (result == 0));
		clearFlag((FLAG_N | FLAG_H | FLAG_C));

		AF.hi |= reg;
		return 0;
	}

	u8 Cpu::CP_A_N(u8 reg)
	{
		u8 result = AF.hi - reg;

		setFlag(FLAG_Z, (result == 0));
		setFlag(FLAG_N);
		setFlag(FLAG_H, ((AF.hi & 0xF) - (reg & 0xF)) < 0);
		setFlag(FLAG_C, (reg > AF.hi));
		return 0;
	}

	void Cpu::PUSH_NN(Register reg)
	{
		SP--;
		write(SP, reg.hi);
		SP--;
		write(SP, reg.lo);
	}

	void Cpu::POP_NN(Register& reg)
	{
		reg.lo = read(SP);
		SP++;
		reg.hi = read(SP);
		SP++;
	}

	void Cpu::JP()
	{
		u16 addr = fetchU16();
		PC = addr;
	}

	u8 Cpu::JP_COND(bool condition)
	{
		u16 addr = fetchU16();
		if (condition) {
			PC = addr;
			return 4;
		}
		return 0;
	}

	void Cpu::CALL()
	{
		u16 addr = fetchU16();

		u8 lo = (PC & 0xFF);
		u8 hi = (PC & 0xFF00) >> 8;

		SP--;
		write(SP, hi);
		SP--;
		write(SP, lo);

		PC = addr;
	}

	u8 Cpu::CALL_COND(bool condition)
	{
		u16 addr = fetchU16();
		if (condition) {
			CALL();
			return 8;
		}
		return 0;
	}

	void Cpu::RET()
	{
		u8 lo;
		u8 hi;

		lo = read(SP);
		SP++;
	    hi = read(SP);
		SP++;

		u16 returnAddr = ((hi << 8) | lo);
		PC = returnAddr;
	}

	u8 Cpu::RET_COND(bool condition)
	{
		return 0;
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
		write(BC.value, AF.hi);
		return 0;
	}

	u8 Cpu::op0x03()
	{
		BC.value++;
		return 0;
	}

	u8 Cpu::op0x04()
	{
		INC_N(BC.hi);
		return 0;
	}

	u8 Cpu::op0x05()
	{
		DEC_N(BC.hi);
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

		clearFlag((FLAG_Z | FLAG_N | FLAG_H));

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
		ADD_HL_NN(BC.value);
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
		INC_N(BC.lo);
		return 0;
	}

	u8 Cpu::op0x0D()
	{
		DEC_N(BC.lo);
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

		clearFlag((FLAG_Z | FLAG_N | FLAG_H));

		if (lsb)
			setFlag(FLAG_C);
		else
			clearFlag(FLAG_C);
		
		AF.hi >>= 1;
		return 0;
	}

	u8 Cpu::op0x10()
	{
		PC++;
		return 0;
	}

	u8 Cpu::op0x11()
	{
		DE.value = fetchU16();
		return 0;
	}
	u8 Cpu::op0x12()
	{
		write(BC.value, AF.hi);
		return 0;
	}

	u8 Cpu::op0x13()
	{
		DE.value++;
		return 0;
	}

	u8 Cpu::op0x14()
	{
		INC_N(DE.hi);
		return 0;
	}

	u8 Cpu::op0x15()
	{
		DEC_N(DE.hi);
		return 0;
	}
	u8 Cpu::op0x16()
	{
		u8 data = fetchU8();
		DE.hi = data;
		return 0;
	}

	u8 Cpu::op0x17()
	{
		u8 carry = ((AF.lo & FLAG_C) >> 4);
		u8 msb = ((AF.hi & 0x80) >> 7);
		u8 result = (AF.hi << 1);

		clearFlag((FLAG_Z | FLAG_N | FLAG_H));

		AF.hi <<= 1;

		//Carry put into bit 0
		(AF.hi |= carry);
		//Msb put into carry
		if (msb) {
			setFlag(FLAG_C);
		}
		else {
			clearFlag(FLAG_C);
		}

		return 0;
	}

	u8 Cpu::op0x18()
	{
		s8 i8 = fetchU8();
		PC = PC + i8;
		return 0;
	}

	u8 Cpu::op0x19()
	{
		ADD_HL_NN(DE.value);
		return 0;
	}

	u8 Cpu::op0x1A()
	{
		u8 data = read(DE.value);
		AF.hi = data;
		return 0;
	}

	u8 Cpu::op0x1B()
	{
		DE.value--;
		return 0;
	}

	u8 Cpu::op0x1C()
	{
		INC_N(DE.lo);
		return 0;
	}

	u8 Cpu::op0x1D()
	{
		DEC_N(DE.lo);
		return 0;
	}

	u8 Cpu::op0x1E()
	{
		u8 data = fetchU8();
		DE.lo = data;
		return 0;
	}

	u8 Cpu::op0x1F()
	{
		u8 carry = ((AF.lo & FLAG_C));
		u8 lsb = (AF.hi & 0x1);
		u8 result = (AF.hi >> 1);

		clearFlag((FLAG_Z | FLAG_N | FLAG_H));

		AF.hi >>= 1;

		//Carry put into bit 0
		(AF.hi |= (carry << 3));

		//Msb put into carry
		if (lsb) {
			setFlag(FLAG_C);
		}
		else {
			clearFlag(FLAG_C);
		}
		return 0;
	}
	u8 Cpu::op0x20()
	{
		s8 i8 = fetchU8();
		//Jump if Zero is not set
		if (getFlag(FLAG_Z) == 0) {
			PC = PC + i8;
			return 4;
		}
		return 0;
	}
	u8 Cpu::op0x21()
	{
		HL.value = fetchU16();
		return 0;
	}
	u8 Cpu::op0x22()
	{
		write(HL.value++, AF.hi);
		return 0;
	}
	u8 Cpu::op0x23()
	{
		HL.value++;
		return 0;
	}
	u8 Cpu::op0x24()
	{
		INC_N(HL.hi);
		return 0;
	}
	u8 Cpu::op0x25()
	{
		DEC_N(HL.hi);
		return 0;
	}
	u8 Cpu::op0x26()
	{
		u8 data = fetchU8();
		HL.hi = data;
		return 0;
	}
	u8 Cpu::op0x27()
	{
		/*
				
			The DA instruction adjusts the 8-bit value in the accumulator 
			to correspond to binary-coded decimal (BCD) format. 
			This instruction begins by testing the low-order nibble of the 
			accumulator. If the AC flag is set or if the low 4 bits of the 
			accumulator exceed a value of 9, the accumulator is incremented 
			by 6. The high-order nibble is then tested. 
			If the carry flag is set or if the high 4 bits of the accumulator 
			exceed a value of 9, the value 60h is added to the accumulator. 
			This instruction performs a decimal conversion by adding 00h, 
			06h, or 66h to the accumulator depending on the initial contents 
			of the PSW and accumulator.
		
		*/
		u8 a = AF.hi;

		if (!getFlag(FLAG_N)) {
			if ((getFlag(FLAG_H)) || (a & 0x0F) > 9)
				a += 6;

			if ((getFlag(FLAG_C)) || a > 0x9F)
				a += 0x60;
		}
		else {
			if (getFlag(FLAG_H)) {
				a -= 6;
				if (!(getFlag(FLAG_C)))
					a &= 0xFF;
			}
			if (getFlag(FLAG_C))
				a -= 0x60;
		}
		clearFlag((FLAG_H | FLAG_Z));
		if (a & 0x100)
			setFlag(FLAG_C);

		AF.hi = a & 0xFF;

		if (!AF.hi)
			setFlag(FLAG_Z);


		return 0;
	}
	u8 Cpu::op0x28()
	{
		s8 i8 = fetchU8();
		//Jump if Zero is set
		if (getFlag(FLAG_Z)) {
			PC = PC + i8;
			return 4;
		}

		return 0;
	}
	u8 Cpu::op0x29()
	{
		ADD_HL_NN(HL.value);
		return 0;
	}
	u8 Cpu::op0x2A()
	{
		u8 data = read(HL.value++);
		AF.hi = data;
		return 0;
	}
	u8 Cpu::op0x2B()
	{
		HL.value--;
		return 0;
	}
	u8 Cpu::op0x2C()
	{
		INC_N(HL.lo);
		return 0;
	}
	u8 Cpu::op0x2D()
	{
		DEC_N(HL.lo);
		return 0;
	}
	u8 Cpu::op0x2E()
	{
		u8 data = fetchU8();
		HL.lo = data;
		return 0;
	}
	u8 Cpu::op0x2F()
	{
		AF.hi = ~AF.hi;
		setFlag((FLAG_N | FLAG_H));
		return 0;
	}
	u8 Cpu::op0x30()
	{
		s8 i8 = fetchU8();
		//Jump if Carry is not set
		if (getFlag(FLAG_C) == 0) {
			PC = PC + i8;
			return 4;
		}
		return 0;
	}
	u8 Cpu::op0x31()
	{
		SP = fetchU16();
		return 0;
	}
	u8 Cpu::op0x32()
	{
		write(HL.value--, AF.hi);
		return 0;
	}
	u8 Cpu::op0x33()
	{
		SP++;
		return 0;
	}
	u8 Cpu::op0x34()
	{
		u8 data = read(HL.value);
		write(HL.value, ++data);
		return 0;
	}
	u8 Cpu::op0x35()
	{
		u8 data = read(HL.value);
		write(HL.value, --data);
		return 0;
	}
	u8 Cpu::op0x36()
	{
		u8 data = fetchU8();
		write(HL.value, data);
		return 0;
	}
	u8 Cpu::op0x37()
	{
		clearFlag((FLAG_N | FLAG_H));
		setFlag(FLAG_C);
		return 0;
	}
	u8 Cpu::op0x38()
	{
		s8 i8 = fetchU8();
		if (getFlag(FLAG_C)) {
			PC = PC + i8;
			return 4;
		}
		return 0;
	}
	u8 Cpu::op0x39()
	{
		ADD_HL_NN(SP);
		return 0;
	}
	u8 Cpu::op0x3A()
	{
		u8 data = read(HL.value--);
		AF.hi = data;
		return 0;
	}
	u8 Cpu::op0x3B()
	{
		SP--;
		return 0;
	}
	u8 Cpu::op0x3C()
	{
		INC_N(AF.hi);
		return 0;
	}
	u8 Cpu::op0x3D()
	{
		DEC_N(AF.hi);
		return 0;
	}
	u8 Cpu::op0x3E()
	{
		u8 data = fetchU8();
		AF.hi = data;
		return 0;
	}
	u8 Cpu::op0x3F()
	{
		clearFlag((FLAG_N | FLAG_H));
		setFlag(FLAG_C, (getFlag(FLAG_C) ^ 1));
		return 0;
	}
	u8 Cpu::op0x40()
	{
		BC.hi = BC.hi;
		return 0;
	}
	u8 Cpu::op0x41()
	{
		BC.hi = BC.lo;
		return 0;
	}
	u8 Cpu::op0x42()
	{
		BC.hi = DE.hi;
		return 0;
	}
	u8 Cpu::op0x43()
	{
		BC.hi = DE.lo;
		return 0;
	}
	u8 Cpu::op0x44()
	{
		BC.hi = HL.hi;
		return 0;
	}
	u8 Cpu::op0x45()
	{
		BC.hi = HL.lo;
		return 0;
	}
	u8 Cpu::op0x46()
	{
		BC.hi = read(HL.value);
		return 0;
	}
	u8 Cpu::op0x47()
	{
		BC.hi = AF.hi;
		return 0;
	}
	u8 Cpu::op0x48()
	{
		BC.lo = BC.hi;
		return 0;
	}
	u8 Cpu::op0x49()
	{
		BC.lo = BC.lo;
		return 0;
	}
	u8 Cpu::op0x4A()
	{
		BC.lo = DE.hi;
		return 0;
	}
	u8 Cpu::op0x4B()
	{
		BC.lo = DE.lo;
		return 0;
	}
	u8 Cpu::op0x4C()
	{
		BC.lo = HL.hi;
		return 0;
	}
	u8 Cpu::op0x4D()
	{
		BC.lo = HL.lo;
		return 0;
	}
	u8 Cpu::op0x4E()
	{
		BC.lo = read(HL.value);
		return 0;
	}
	u8 Cpu::op0x4F()
	{
		BC.lo = AF.hi;
		return 0;
	}
	u8 Cpu::op0x50()
	{
		DE.hi = BC.hi;
		return 0;
	}
	u8 Cpu::op0x51()
	{
		DE.hi = BC.lo;
		return 0;
	}
	u8 Cpu::op0x52()
	{
		DE.hi = DE.hi;
		return 0;
	}
	u8 Cpu::op0x53()
	{
		DE.hi = DE.lo;
		return 0;
	}
	u8 Cpu::op0x54()
	{
		DE.hi = HL.hi;
		return 0;
	}
	u8 Cpu::op0x55()
	{
		DE.hi = HL.lo;
		return 0;
	}
	u8 Cpu::op0x56()
	{
		DE.hi = read(HL.value);
		return 0;
	}
	u8 Cpu::op0x57()
	{
		DE.hi = AF.hi;
		return 0;
	}
	u8 Cpu::op0x58()
	{
		DE.lo = BC.hi;
		return 0;
	}
	u8 Cpu::op0x59()
	{
		DE.lo = BC.lo;
		return 0;
	}
	u8 Cpu::op0x5A()
	{
		DE.lo = DE.hi;
		return 0;
	}
	u8 Cpu::op0x5B()
	{
		DE.lo = DE.lo;
		return 0;
	}
	u8 Cpu::op0x5C()
	{
		DE.lo = HL.hi;
		return 0;
	}
	u8 Cpu::op0x5D()
	{
		DE.lo = HL.lo;
		return 0;
	}
	u8 Cpu::op0x5E()
	{
		DE.lo = read(HL.value);
		return 0;
	}
	u8 Cpu::op0x5F()
	{
		DE.lo = AF.hi;
		return 0;
	}
	u8 Cpu::op0x60()
	{
		HL.hi = BC.hi;
		return 0;
	}
	u8 Cpu::op0x61()
	{
		HL.hi = BC.lo;
		return 0;
	}
	u8 Cpu::op0x62()
	{
		HL.hi = DE.hi;
		return 0;
	}
	u8 Cpu::op0x63()
	{
		HL.hi = DE.lo;
		return 0;
	}
	u8 Cpu::op0x64()
	{
		HL.hi = HL.hi;
		return 0;
	}
	u8 Cpu::op0x65()
	{
		HL.hi = HL.lo;
		return 0;
	}
	u8 Cpu::op0x66()
	{
		HL.hi = read(HL.value);
		return 0;
	}
	u8 Cpu::op0x67()
	{
		HL.hi = AF.hi;
		return 0;
	}
	u8 Cpu::op0x68()
	{
		HL.lo = BC.hi;
		return 0;
	}
	u8 Cpu::op0x69()
	{
		HL.lo = BC.lo;
		return 0;
	}
	u8 Cpu::op0x6A()
	{
		HL.lo = DE.hi;
		return 0;
	}
	u8 Cpu::op0x6B()
	{
		HL.lo = DE.lo;
		return 0;
	}
	u8 Cpu::op0x6C()
	{
		HL.lo = HL.hi;
		return 0;
	}
	u8 Cpu::op0x6D()
	{
		HL.lo = HL.lo;
		return 0;
	}
	u8 Cpu::op0x6E()
	{
		HL.lo = read(HL.value);
		return 0;
	}
	u8 Cpu::op0x6F()
	{
		HL.lo = AF.hi;
		return 0;
	}
	u8 Cpu::op0x70()
	{
		write(HL.value, BC.hi);
		return 0;
	}
	u8 Cpu::op0x71()
	{
		write(HL.value, BC.lo);
		return 0;
	}
	u8 Cpu::op0x72()
	{
		write(HL.value, DE.hi);
		return 0;
	}
	u8 Cpu::op0x73()
	{
		write(HL.value, DE.lo);
		return 0;
	}
	u8 Cpu::op0x74()
	{
		write(HL.value, HL.hi);
		return 0;
	}
	u8 Cpu::op0x75()
	{
		write(HL.value, HL.lo);
		return 0;
	}
	u8 Cpu::op0x76()
	{
		//Halt
		return 0;
	}
	u8 Cpu::op0x77()
	{
		write(HL.value, AF.hi);
		return 0;
	}
	u8 Cpu::op0x78()
	{
		AF.hi = BC.hi;
		return 0;
	}
	u8 Cpu::op0x79()
	{
		AF.hi = BC.lo;
		return 0;
	}
	u8 Cpu::op0x7A()
	{
		AF.hi = DE.hi;
		return 0;
	}
	u8 Cpu::op0x7B()
	{
		AF.hi = DE.lo;
		return 0;
	}
	u8 Cpu::op0x7C()
	{
		AF.hi = HL.hi;
		return 0;
	}
	u8 Cpu::op0x7D()
	{
		AF.hi = HL.lo;
		return 0;
	}
	u8 Cpu::op0x7E()
	{
		AF.hi = read(HL.value);
		return 0;
	}
	u8 Cpu::op0x7F()
	{
		AF.hi = AF.hi;
		return 0;
	}
	u8 Cpu::op0x80()
	{
		ADD_A_N(BC.hi);
		return 0;
	}
	u8 Cpu::op0x81()
	{
		ADD_A_N(BC.lo);
		return 0;
	}
	u8 Cpu::op0x82()
	{
		ADD_A_N(DE.hi);
		return 0;
	}
	u8 Cpu::op0x83()
	{
		ADD_A_N(DE.lo);
		return 0;
	}
	u8 Cpu::op0x84()
	{
		ADD_A_N(HL.hi);
		return 0;
	}
	u8 Cpu::op0x85()
	{
		ADD_A_N(HL.lo);
		return 0;
	}
	u8 Cpu::op0x86()
	{
		ADD_A_N(read(HL.value));
		return 0;
	}
	u8 Cpu::op0x87()
	{
		ADD_A_N(AF.hi);
		return 0;
	}
	u8 Cpu::op0x88()
	{
		ADC_A_N(BC.hi);
		return 0;
	}
	u8 Cpu::op0x89()
	{
		ADC_A_N(BC.lo);
		return 0;
	}
	u8 Cpu::op0x8A()
	{
		ADC_A_N(DE.hi);
		return 0;
	}
	u8 Cpu::op0x8B()
	{
		ADC_A_N(DE.lo);
		return 0;
	}
	u8 Cpu::op0x8C()
	{
		ADC_A_N(HL.hi);
		return 0;
	}
	u8 Cpu::op0x8D()
	{
		ADC_A_N(HL.lo);
		return 0;
	}
	u8 Cpu::op0x8E()
	{
		ADC_A_N(read(HL.value));
		return 0;
	}
	u8 Cpu::op0x8F()
	{
		ADC_A_N(AF.hi);
		return 0;
	}
	u8 Cpu::op0x90()
	{
		SUB_A_N(BC.hi);
		return 0;
	}
	u8 Cpu::op0x91()
	{
		SUB_A_N(BC.lo);
		return 0;
	}
	u8 Cpu::op0x92()
	{
		SUB_A_N(DE.hi);
		return 0;
	}
	u8 Cpu::op0x93()
	{
		SUB_A_N(DE.lo);
		return 0;
	}
	u8 Cpu::op0x94()
	{
		SUB_A_N(HL.hi);
		return 0;
	}
	u8 Cpu::op0x95()
	{
		SUB_A_N(HL.lo);
		return 0;
	}
	u8 Cpu::op0x96()
	{
		SUB_A_N(read(HL.value));
		return 0;
	}
	u8 Cpu::op0x97()
	{
		SUB_A_N(AF.hi);
		return 0;
	}
	u8 Cpu::op0x98()
	{
		SBC_A_N(BC.hi);
		return 0;
	}
	u8 Cpu::op0x99()
	{
		SBC_A_N(BC.lo);
		return 0;
	}
	u8 Cpu::op0x9A()
	{
		SBC_A_N(DE.hi);
		return 0;
	}
	u8 Cpu::op0x9B()
	{
		SBC_A_N(DE.lo);
		return 0;
	}
	u8 Cpu::op0x9C()
	{
		SBC_A_N(HL.hi);
		return 0;
	}
	u8 Cpu::op0x9D()
	{
		SBC_A_N(HL.lo);
		return 0;
	}
	u8 Cpu::op0x9E()
	{
		SBC_A_N(read(HL.value));
		return 0;
	}
	u8 Cpu::op0x9F()
	{
		SBC_A_N(AF.hi);
		return 0;
	}
	u8 Cpu::op0xA0()
	{
		AND_A_N(BC.hi);
		return 0;
	}
	u8 Cpu::op0xA1()
	{
		AND_A_N(BC.lo);
		return 0;
	}
	u8 Cpu::op0xA2()
	{
		AND_A_N(DE.hi);
		return 0;
	}
	u8 Cpu::op0xA3()
	{
		AND_A_N(DE.lo);
		return 0;
	}
	u8 Cpu::op0xA4()
	{
		AND_A_N(HL.hi);
		return 0;
	}
	u8 Cpu::op0xA5()
	{
		AND_A_N(HL.lo);
		return 0;
	}
	u8 Cpu::op0xA6()
	{
		AND_A_N(read(HL.value));
		return 0;
	}
	u8 Cpu::op0xA7()
	{
		AND_A_N(AF.hi);
		return 0;
	}
	u8 Cpu::op0xA8()
	{
		XOR_A_N(BC.hi);
		return 0;
	}
	u8 Cpu::op0xA9()
	{
		XOR_A_N(BC.lo);
		return 0;
	}
	u8 Cpu::op0xAA()
	{
		XOR_A_N(DE.hi);
		return 0;
	}
	u8 Cpu::op0xAB()
	{
		XOR_A_N(DE.lo);
		return 0;
	}
	u8 Cpu::op0xAC()
	{
		XOR_A_N(HL.hi);
		return 0;
	}
	u8 Cpu::op0xAD()
	{
		XOR_A_N(HL.lo);
		return 0;;
	}
	u8 Cpu::op0xAE()
	{
		XOR_A_N(read(HL.value));
		return 0;
	}
	u8 Cpu::op0xAF()
	{
		XOR_A_N(AF.hi);
		return 0;
	}
	u8 Cpu::op0xB0()
	{
		OR_A_N(BC.hi);
		return 0;
	}
	u8 Cpu::op0xB1()
	{
		OR_A_N(BC.lo);
		return 0;
	}
	u8 Cpu::op0xB2()
	{
		OR_A_N(DE.hi);
		return 0;
	}
	u8 Cpu::op0xB3()
	{
		OR_A_N(DE.lo);
		return 0;
	}
	u8 Cpu::op0xB4()
	{
		OR_A_N(HL.hi);
		return 0;
	}
	u8 Cpu::op0xB5()
	{
		OR_A_N(HL.lo);
		return 0;
	}
	u8 Cpu::op0xB6()
	{
		OR_A_N(read(HL.value));
		return 0;
	}
	u8 Cpu::op0xB7()
	{
		OR_A_N(AF.hi);
		return 0;
	}
	u8 Cpu::op0xB8()
	{
		CP_A_N(BC.hi);
		return 0;
	}
	u8 Cpu::op0xB9()
	{
		CP_A_N(BC.lo);
		return 0;
	}
	u8 Cpu::op0xBA()
	{
		CP_A_N(DE.hi);
		return 0;
	}
	u8 Cpu::op0xBB()
	{
		CP_A_N(DE.lo);
		return 0;
	}
	u8 Cpu::op0xBC()
	{
		CP_A_N(HL.hi);
		return 0;
	}
	u8 Cpu::op0xBD()
	{
		CP_A_N(HL.lo);
		return 0;
	}
	u8 Cpu::op0xBE()
	{
		CP_A_N(read(HL.value));
		return 0;
	}
	u8 Cpu::op0xBF()
	{
		CP_A_N(AF.hi);
		return 0;
	}
	u8 Cpu::op0xC0()
	{

		return 0;
	}
	u8 Cpu::op0xC1()
	{
		POP_NN(BC);
		return 0;
	}
	u8 Cpu::op0xC2()
	{
		u8 extra_cycles = JP_COND((getFlag(FLAG_Z) == 0));
		return extra_cycles;
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
		PUSH_NN(BC);
		return 0;
	}
	u8 Cpu::op0xC6()
	{
		ADD_A_N(fetchU8());
		return 0;
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
		RET();
		return 0;
	}
	u8 Cpu::op0xCA()
	{
		return u8();
	}
	u8 Cpu::op0xCB()
	{
		//Execute CB table instruction
		u8 cb_table_opcode = read(PC);
		Instruction ins = cbTable[cb_table_opcode];

		cycles += ins.cycles;
		cycles += ins.execute();

		return 0;
	}
	u8 Cpu::op0xCC()
	{
		return u8();
	}
	u8 Cpu::op0xCD()
	{
		CALL();
		return 0;
	}
	u8 Cpu::op0xCE()
	{
		ADC_A_N(fetchU8());
		return 0;
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

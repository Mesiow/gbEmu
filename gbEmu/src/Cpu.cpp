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
		SP = 0x0000;

		//stub FF44 in memory to pass 0068 in bootrom
		write(0xFF44, 0x90);

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

		//Test 0xCB Swap
		//BC.hi = 0x23;
		//mmu->write(0x0, 0xCB);
		//mmu->write(0x1, 0x30);

	}


	//TODO: TESTING Blargs test 07
	u8 Cpu::clock()
	{
		//if (cycles == 0) {
			//Read opcode
		

			if (!halt) {
				u8 opcode = read(PC);
				PC++;
				if (opcode == 0xCB) {
					//Get instruction associated with that opcode
					u8 cbOpcode = read(PC++);
					Instruction ins = cbTable[cbOpcode];

					//Base number of cycles required
					cycles = ins.cycles;
					//May require more cycles based on certain conditions
					cycles += ins.execute();
				}
				else {

					Instruction ins = table[opcode];
					cycles = ins.cycles;
					//May require more cycles based on certain conditions
					cycles += ins.execute();
				}
				handleTimer(cycles);
				handleInterrupts();
			}
			
		//}
		//cycles--;
		return cycles;
	}

	void Cpu::handleTimer(u32 cycles)
	{

	}

	void Cpu::handleInterrupts()
	{
		//IF - interrupt flag request register at 0xFF0F (that shows that the condition for a interrupt was met (user pressed a button, for example)
		//IE - interrupt enable register at 0xFFFF (only interrupts that are enabled in this register
		//will be handled, once they are flagged in IF

		//Interrupts should be handled
		if (interruptsEnabled) {
			u8 IF = read(0xFF0F);
			u8 IE = read(0xFFFF);

			//Interrupt service routine addresses(ISR)
			//VBlank - 0x40
			//LCD Status triggers - 0x48
			//Timer overflow - 0x50
			//Serial link - 0x58
			//Joypad press - 0x60


			//If an interrupt is enabled and allowed
			if (IE & IF) {
				//Handle interrupts starting from 
				//Bit 0 (vblank)
				if ((IE & 0x1) & (IF & 0x1)) {
					CALL(0x40);
					//Clear IF after jumping to ISR address
					IF &= ~0x1;
					write(0xFF0F, IF);
				}

				//When 0, off, when 1, on
				//Bit 1 (LCD stat) 
				if ((IE & 0x2) & (IF & 0x2)) {
					CALL(0x48);
					//Clear IF after jumping to ISR address
					IF &= ~0x2;
					write(0xFF0F, IF);
				}

				//Bit 2 (Timer)
				if ((IE & 0x4) & (IF & 0x4)) {
					CALL(0x50);
					IF &= ~0x4;
					write(0xFF0F, IF);
				}

				//Bit 3 (Serial)
				if ((IE & 0x8) & (IF & 0x8)) {
					CALL(0x58);
					IF &= ~0x8;
					write(0xFF0F, IF);
				}

				//Bit 4 (Joypad)
				if ((IE & 0x10) & (IF & 0x10)) {
					CALL(0x60);
					IF &= ~0x10;
					write(0xFF0F, IF);
				}
			}
		}
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
		u8 result = AF.hi - reg - carry;

		setFlag(FLAG_Z, (result == 0));
		setFlag(FLAG_N);
		setFlag(FLAG_H, ((AF.hi & 0xF) - (reg & 0xF) - carry) < 0);
		setFlag(FLAG_C, ((reg + carry) > AF.hi));

		AF.hi = result;
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

	void Cpu::PUSH_PC()
	{

	}

	void Cpu::POP_NN(Register& reg)
	{
		reg.lo = read(SP);
		SP++;
		reg.hi = read(SP);
		SP++;
	}

	void Cpu::JP(u16 address)
	{
		PC = address;
	}

	u8 Cpu::JP_COND(u16 address, bool condition)
	{
		if (condition) {
			JP(address);
			return 4;
		}
		PC += 2;
		return 0;
	}

	void Cpu::CALL(u16 address)
	{
		u8 lo = (PC & 0xFF);
		u8 hi = (PC & 0xFF00) >> 8;

		SP--;
		write(SP, hi);
		SP--;
		write(SP, lo);

		PC = address;
	}

	u8 Cpu::CALL_COND(u16 address, bool condition)
	{
		if (condition) {
			CALL(address);
			return 8;
		}
		PC += 2;
		return 0;
	}

	void Cpu::RET()
	{
		u8 lo = read(SP);
		SP++;
	    u8 hi = read(SP);
		SP++;

		u16 returnAddr = ((hi << 8) | lo);
		PC = returnAddr;
	}

	u8 Cpu::RET_COND(bool condition)
	{
		if (condition) {
			RET();
			return 12;
		}
		return 0;
	}

	void Cpu::RST(u16 address)
	{
		u8 lo = (PC & 0xFF);
		u8 hi = (PC & 0xFF00) >> 8;

		SP--;
		write(SP, hi);
		SP--;
		write(SP, lo);

		PC = address;
	}

	void Cpu::EI()
	{
		interruptsEnabled = true;
	}

	void Cpu::DI()
	{
		interruptsEnabled = false;
	}


	/*
		Instruction Implementations
	*/

	void Cpu::RLC_N(u8& reg)
	{
		u8 msb = ((reg & 0x80) >> 7);
		clearFlag((FLAG_N | FLAG_H));

		reg <<= 1;

		//Carry will store old bit 7 data,
		//bit 7 will also be moved to bit 0
		if (msb) {
			setFlag(FLAG_C);
			reg = setBit(reg, 0);
		}
		else {
			clearFlag(FLAG_C);
			reg = resetBit(reg, 0);
		}

		setFlag(FLAG_Z, (reg == 0));
	}

	void Cpu::RRC_N(u8& reg)
	{
		u8 lsb = (reg & 0x1);
		u8 result = (reg >> 1) & 0xFF;
		clearFlag((FLAG_N | FLAG_H));

		reg >>= 1;


		if (lsb) {
			setFlag(FLAG_C);
			reg = setBit(reg, 7);
		}
		else {
			clearFlag(FLAG_C);
			reg = resetBit(reg, 7);
		}

		setFlag(FLAG_Z, (reg == 0));
	}

	void Cpu::RL_N(u8& reg)
	{
		u8 carry = ((AF.lo & FLAG_C) >> 4);
		u8 msb = ((reg & 0x80) >> 7);	
		clearFlag((FLAG_N | FLAG_H));

		reg <<= 1;

		//Carry put into bit 0
		(reg |= carry);
		//Msb put into carry
		if (msb) {
			setFlag(FLAG_C);
		}
		else {
			clearFlag(FLAG_C);
		}

		setFlag(FLAG_Z, (reg == 0));
	}

	void Cpu::RR_N(u8& reg)
	{
		u8 carry = ((AF.lo & FLAG_C));
		u8 lsb = (reg & 0x1);
		clearFlag((FLAG_N | FLAG_H));

		reg >>= 1;

		//Carry put into bit 7
		(reg |= (carry << 3));

		//Msb put into carry
		if (lsb) {
			setFlag(FLAG_C);
		}
		else {
			clearFlag(FLAG_C);
		}

		setFlag(FLAG_Z, (reg == 0));
	}

	void Cpu::SLA_N(u8& reg)
	{
		u8 msb = ((reg & 0x80) >> 7);
		u8 result = (reg << 1);

		setFlag(FLAG_Z, (result == 0));
		clearFlag((FLAG_N | FLAG_H));

		if (msb) {
			setFlag(FLAG_C);
		}
		else {
			clearFlag(FLAG_C);
		}
		reg <<= 1;
		reg = resetBit(reg, 0);
	}

	void Cpu::SRA_N(u8& reg)
	{
		u8 lsb = (reg & 0x1);	
		u8 msb = ((reg & 0x80));
		clearFlag((FLAG_N | FLAG_H));

		reg >>= 1;
		reg |= (msb);

		if (lsb) {
			setFlag(FLAG_C);
		}
		else {
			clearFlag(FLAG_C);
		}
		
		setFlag(FLAG_Z, (reg == 0));
	}

	void Cpu::SWAP_N(u8& reg)
	{
		u8 upper4 = (reg & 0xF0) >> 4;
		u8 lower4 = (reg & 0x0F);

		u8 result = ((lower4 & 0xF) << 4) | (upper4 & 0xF);

		setFlag(FLAG_Z, (result == 0));
		clearFlag((FLAG_N | FLAG_H | FLAG_C));

		reg = result;
	}

	void Cpu::SRL_N(u8& reg)
	{
		u8 lsb = (reg & 0x1);
		u8 result = (reg >> 1);

		setFlag(FLAG_Z, (result == 0));
		clearFlag((FLAG_N | FLAG_H));

		if (lsb) {
			setFlag(FLAG_C);
		}
		else {
			clearFlag(FLAG_C);
		}
		reg >>= 1;
		reg = resetBit(reg, 7);
	}

	void Cpu::BIT_B_N(u8 bit, u8 reg)
	{
		setFlag(FLAG_H);
		clearFlag(FLAG_N);
		if (testBit(reg, bit) == 0) {
			setFlag(FLAG_Z);
		}
		else {
			clearFlag(FLAG_Z);
		}
	}

	void Cpu::RES_B_N(u8 bit, u8& reg)
	{
		reg = resetBit(reg, bit);
	}

	void Cpu::SET_B_N(u8 bit, u8& reg)
	{
		reg = setBit(reg, bit);
	}

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
		clearFlag((FLAG_Z | FLAG_N | FLAG_H));

		AF.hi <<= 1;

		//Carry will store old bit 7 data, bit 7 will be
		//moved to bit 0
		if (msb) {
			setFlag(FLAG_C);
			AF.hi = setBit(AF.hi, 0);
		}
		else {
			clearFlag(FLAG_C);
			AF.hi = resetBit(AF.hi, 0);
		}

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
		clearFlag((FLAG_Z | FLAG_N | FLAG_H));

		AF.hi >>= 1;

		if (lsb) {
			setFlag(FLAG_C);
			AF.hi = setBit(AF.hi, 7);
		}
		else {
			clearFlag(FLAG_C);
			AF.hi = resetBit(AF.hi, 7);
		}
		
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
		write(DE.value, AF.hi);
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

		//Carry put into bit 7
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
		//Jump if Zero is not set
		if (getFlag(FLAG_Z) == 0) {
			s8 i8 = fetchU8();
			PC = PC + i8;
			return 4;
		}
		PC++;
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
		s32 a = AF.hi;

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
		//Jump if Zero is set
		if (getFlag(FLAG_Z)) {
			s8 i8 = fetchU8();
			PC = PC + i8;
			return 4;
		}
		PC++;
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
		//Jump if Carry is not set
		if (getFlag(FLAG_C) == 0) {
			s8 i8 = fetchU8();
			PC = PC + i8;
			return 4;
		}
		PC++;
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
		INC_N(data);
		write(HL.value, data);

		return 0;
	}
	u8 Cpu::op0x35()
	{
		u8 data = read(HL.value);
		DEC_N(data);
		write(HL.value, data);

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
		//Jump if carry is set
		if (getFlag(FLAG_C)) {
			s8 i8 = fetchU8();
			PC = PC + i8;
			return 4;
		}
		PC++;
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
		u8 extra_cycles = RET_COND((getFlag(FLAG_Z) == 0));
		return extra_cycles;
	}
	u8 Cpu::op0xC1()
	{
		POP_NN(BC);
		return 0;
	}
	u8 Cpu::op0xC2()
	{
		u8 extra_cycles = JP_COND(fetchU16(), (getFlag(FLAG_Z) == 0));
		return extra_cycles;
	}
	u8 Cpu::op0xC3()
	{
		JP(fetchU16());
		return 0;
	}
	u8 Cpu::op0xC4()
	{
		u8 extra_cycles = CALL_COND(fetchU16(), (getFlag(FLAG_Z) == 0));
		return extra_cycles;
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
		RST(0x0000);
		return 0;
	}
	u8 Cpu::op0xC8()
	{
		u8 extra_cycles = RET_COND(getFlag(FLAG_Z));
		return extra_cycles;
	}
	u8 Cpu::op0xC9()
	{
		RET();
		return 0;
	}
	u8 Cpu::op0xCA()
	{
		u8 extra_cycles = JP_COND(fetchU16(), getFlag(FLAG_Z));
		return extra_cycles;
	}
	u8 Cpu::op0xCB()
	{
		//CB prefix
		return 0;
	}
	u8 Cpu::op0xCC()
	{
		u8 extra_cycles = CALL_COND(fetchU16(), getFlag(FLAG_Z));
		return extra_cycles;
	}
	u8 Cpu::op0xCD()
	{
		CALL(fetchU16());
		return 0;
	}
	u8 Cpu::op0xCE()
	{
		ADC_A_N(fetchU8());
		return 0;
	}
	u8 Cpu::op0xCF()
	{
		RST(0x0008);
		return 0;
	}
	u8 Cpu::op0xD0()
	{
		u8 extra_cycles = RET_COND((getFlag(FLAG_C) == 0));
		return extra_cycles;
	}
	u8 Cpu::op0xD1()
	{
		POP_NN(DE);
		return 0;
	}
	u8 Cpu::op0xD2()
	{
		u8 extra_cycles = JP_COND(fetchU16(), (getFlag(FLAG_C) == 0));
		return extra_cycles;
	}
	u8 Cpu::op0xD3()
	{
		//Illegal opcode
		return 0;
	}
	u8 Cpu::op0xD4()
	{
		u8 extra_cycles = CALL_COND(fetchU16(), (getFlag(FLAG_C) == 0));
		return extra_cycles;
	}
	u8 Cpu::op0xD5()
	{
		PUSH_NN(DE);
		return 0;
	}
	u8 Cpu::op0xD6()
	{
		SUB_A_N(fetchU8());
		return 0;
	}
	u8 Cpu::op0xD7()
	{
		RST(0x0010);
		return 0;
	}
	u8 Cpu::op0xD8()
	{
		u8 extra_cycles = RET_COND(getFlag(FLAG_C));
		return extra_cycles;
	}
	u8 Cpu::op0xD9()
	{
		EI();
		RET();
		return 0;
	}
	u8 Cpu::op0xDA()
	{
		u8 extra_cycles = JP_COND(fetchU16(), getFlag(FLAG_C));
		return extra_cycles;
	}
	u8 Cpu::op0xDB()
	{
		//Illegal opcode
		return 0;
	}
	u8 Cpu::op0xDC()
	{
		u8 extra_cycles = CALL_COND(fetchU16(), getFlag(FLAG_C));
		return extra_cycles;
	}
	u8 Cpu::op0xDD()
	{
		//Illegal opcode
		return 0;
	}
	u8 Cpu::op0xDE()
	{
		SBC_A_N(fetchU8());
		return 0;
	}
	u8 Cpu::op0xDF()
	{
		RST(0x0018);
		return 0;
	}
	u8 Cpu::op0xE0()
	{
		u8 data = fetchU8();
		write((0xFF00 + data), AF.hi);
		return 0;
	}
	u8 Cpu::op0xE1()
	{
		POP_NN(HL);
		return 0;
	}
	u8 Cpu::op0xE2()
	{
		write((0xFF00 + BC.lo), AF.hi);
		return 0;
	}
	u8 Cpu::op0xE3()
	{
		//Illegal opcode
		return 0;
	}
	u8 Cpu::op0xE4()
	{
		//Illegal opcode
		return 0;
	}
	u8 Cpu::op0xE5()
	{
		PUSH_NN(HL);
		return 0;
	}
	u8 Cpu::op0xE6()
	{
		AND_A_N(fetchU8());
		return 0;
	}
	u8 Cpu::op0xE7()
	{
		RST(0x0020);
		return 0;
	}
	u8 Cpu::op0xE8()
	{
		s8 i8 = fetchU8();
		u16 result = SP + i8;

		clearFlag((FLAG_Z | FLAG_N));
		setFlag(FLAG_H, (SP & 0xF) + (i8 & 0xF) > 0xF);
		setFlag(FLAG_C, (SP & 0xFF) + (i8 & 0xFF) > 0xFF);

		SP += i8;
		return 0;
	}
	u8 Cpu::op0xE9()
	{
		PC = HL.value;
		return 0;
	}
	u8 Cpu::op0xEA()
	{
		u16 addr = fetchU16();
		write(addr, AF.hi);
		return 0;
	}
	u8 Cpu::op0xEB()
	{
		//Illegal opcode
		return 0;
	}
	u8 Cpu::op0xEC()
	{
		//Illegal opcode
		return 0;
	}
	u8 Cpu::op0xED()
	{
		//Illegal opcode
		return 0;
	}
	u8 Cpu::op0xEE()
	{
		XOR_A_N(fetchU8());
		return 0;
	}
	u8 Cpu::op0xEF()
	{
		RST(0x0028);
		return 0;
	}
	u8 Cpu::op0xF0()
	{
		u8 data = fetchU8();
		AF.hi = read((0xFF00 + data));
		return 0;
	}
	u8 Cpu::op0xF1()
	{
		POP_NN(AF);

		//Make sure lower nibble is cleared to get correct flag value
		AF.lo &= 0xF0; 

		return 0;
	}
	u8 Cpu::op0xF2()
	{
		AF.hi = read((0xFF00 + BC.lo));
		return 0;
	}
	u8 Cpu::op0xF3()
	{
		DI();
		return 0;
	}
	u8 Cpu::op0xF4()
	{
		//Illegal opcode
		return 0;
	}
	u8 Cpu::op0xF5()
	{
		PUSH_NN(AF);
		return 0;
	}
	u8 Cpu::op0xF6()
	{
		OR_A_N(fetchU8());
		return 0;
	}
	u8 Cpu::op0xF7()
	{
		RST(0x0030);
		return 0;
	}
	u8 Cpu::op0xF8()
	{
		s8 i8 = fetchU8();
		u16 result = SP + i8;

		clearFlag((FLAG_Z | FLAG_N));
		setFlag(FLAG_H, (SP & 0xF) + (i8 & 0xF) > 0xF);
		setFlag(FLAG_C, (SP & 0xFF) + (i8 & 0xFF) > 0xFF);

		HL.value = result;
		return 0;
	}
	u8 Cpu::op0xF9()
	{
		SP = HL.value;
		return 0;
	}
	u8 Cpu::op0xFA()
	{
		u16 addr = fetchU16();
		AF.hi = read(addr);
		return 0;
	}
	u8 Cpu::op0xFB()
	{
		EI();
		return 0;
	}
	u8 Cpu::op0xFC()
	{
		//Illegal opcode
		return 0;
	}
	u8 Cpu::op0xFD()
	{
		//Illegal opcode
		return 0;
	}
	u8 Cpu::op0xFE()
	{
		CP_A_N(fetchU8());
		return 0;
	}
	u8 Cpu::op0xFF()
	{
		RST(0x0038);
		return 0;
	}



	/*
		0xCB opcodes
	*/

	u8 Cpu::opCB0x00()
	{
		RLC_N(BC.hi);
		return 0;
	}
	u8 Cpu::opCB0x01()
	{
		RLC_N(BC.lo);
		return 0;
	}
	u8 Cpu::opCB0x02()
	{
		RLC_N(DE.hi);
		return 0;
	}
	u8 Cpu::opCB0x03()
	{
		RLC_N(DE.lo);
		return 0;
	}
	u8 Cpu::opCB0x04()
	{
		RLC_N(HL.hi);
		return 0;
	}
	u8 Cpu::opCB0x05()
	{
		RLC_N(HL.lo);
		return 0;
	}
	u8 Cpu::opCB0x06()
	{
		u8 data = read(HL.value);
		RLC_N(data);
		write(HL.value, data);
	
		return 0;
	}
	u8 Cpu::opCB0x07()
	{
		RLC_N(AF.hi);
		return 0;
	}
	u8 Cpu::opCB0x08()
	{
		RRC_N(BC.hi);
		return 0;
	}
	u8 Cpu::opCB0x09()
	{
		RRC_N(BC.lo);
		return 0;
	}
	u8 Cpu::opCB0x0A()
	{
		RRC_N(DE.hi);
		return 0;
	}
	u8 Cpu::opCB0x0B()
	{
		RRC_N(DE.lo);
		return 0;
	}
	u8 Cpu::opCB0x0C()
	{
		RRC_N(HL.hi);
		return 0;
	}
	u8 Cpu::opCB0x0D()
	{
		RRC_N(HL.lo);
		return 0;
	}
	u8 Cpu::opCB0x0E()
	{
		u8 data = read(HL.value);
		RRC_N(data);
		write(HL.value, data);

		return 0;
	}
	u8 Cpu::opCB0x0F()
	{
		RRC_N(AF.hi);
		return 0;
	}
	u8 Cpu::opCB0x10()
	{
		RL_N(BC.hi);
		return 0;
	}
	u8 Cpu::opCB0x11()
	{
		RL_N(BC.lo);
		return 0;
	}
	u8 Cpu::opCB0x12()
	{
		RL_N(DE.hi);
		return 0;
	}
	u8 Cpu::opCB0x13()
	{
		RL_N(DE.lo);
		return 0;
	}
	u8 Cpu::opCB0x14()
	{
		RL_N(HL.hi);
		return 0;
	}
	u8 Cpu::opCB0x15()
	{
		RL_N(HL.lo);
		return 0;
	}
	u8 Cpu::opCB0x16()
	{
		u8 data = read(HL.value);
		RL_N(data);
		write(HL.value, data);

		return 0;
	}
	u8 Cpu::opCB0x17()
	{
		RL_N(AF.hi);
		return 0;
	}
	u8 Cpu::opCB0x18()
	{
		RR_N(BC.hi);
		return 0;
	}
	u8 Cpu::opCB0x19()
	{
		RR_N(BC.lo);
		return 0;
	}
	u8 Cpu::opCB0x1A()
	{
		RR_N(DE.hi);
		return 0;
	}
	u8 Cpu::opCB0x1B()
	{
		RR_N(DE.lo);
		return 0;
	}
	u8 Cpu::opCB0x1C()
	{
		RR_N(HL.hi);
		return 0;
	}
	u8 Cpu::opCB0x1D()
	{
		RR_N(HL.lo);
		return 0;
	}
	u8 Cpu::opCB0x1E()
	{
		u8 data = read(HL.value);
		RR_N(data);
		write(HL.value, data);

		return 0;
	}
	u8 Cpu::opCB0x1F()
	{
		RR_N(AF.hi);
		return 0;
	}
	u8 Cpu::opCB0x20()
	{
		SLA_N(BC.hi);
		return 0;
	}
	u8 Cpu::opCB0x21()
	{
		SLA_N(BC.lo);
		return 0;
	}
	u8 Cpu::opCB0x22()
	{
		SLA_N(DE.hi);
		return 0;
	}
	u8 Cpu::opCB0x23()
	{
		SLA_N(DE.lo);
		return 0;
	}
	u8 Cpu::opCB0x24()
	{
		SLA_N(HL.hi);
		return 0;
	}
	u8 Cpu::opCB0x25()
	{
		SLA_N(HL.lo);
		return 0;
	}
	u8 Cpu::opCB0x26()
	{
		u8 data = read(HL.value);

		u8 msb = ((data & 0x80) >> 7);
		u8 result = (data << 1);

		setFlag(FLAG_Z, (result == 0));
		clearFlag((FLAG_N | FLAG_H));

		if (msb) {
			setFlag(FLAG_C);
		}
		else {
			clearFlag(FLAG_C);
		}
		data <<= 1;
		data = resetBit(data, 0);

		write(HL.value, data);
		return 0;
	}
	u8 Cpu::opCB0x27()
	{
		SLA_N(AF.hi);
		return 0;
	}
	u8 Cpu::opCB0x28()
	{
		SRA_N(BC.hi);
		return 0;
	}
	u8 Cpu::opCB0x29()
	{
		SRA_N(BC.lo);
		return 0;
	}
	u8 Cpu::opCB0x2A()
	{
		SRA_N(DE.hi);
		return 0;
	}
	u8 Cpu::opCB0x2B()
	{
		SRA_N(DE.lo);
		return 0;
	}
	u8 Cpu::opCB0x2C()
	{
		SRA_N(HL.hi);
		return 0;
	}
	u8 Cpu::opCB0x2D()
	{
		SRA_N(HL.lo);
		return 0;
	}
	u8 Cpu::opCB0x2E()
	{
		u8 data = read(HL.value);
		SRA_N(data);
		write(HL.value, data);

		return 0;
	}
	u8 Cpu::opCB0x2F()
	{
		SRA_N(AF.hi);
		return 0;
	}
	u8 Cpu::opCB0x30()
	{
		SWAP_N(BC.hi);
		return 0;
	}
	u8 Cpu::opCB0x31()
	{
		SWAP_N(BC.lo);
		return 0;
	}
	u8 Cpu::opCB0x32()
	{
		SWAP_N(DE.hi);
		return 0;
	}
	u8 Cpu::opCB0x33()
	{
		SWAP_N(DE.lo);
		return 0;
	}
	u8 Cpu::opCB0x34()
	{
		SWAP_N(HL.hi);
		return 0;
	}
	u8 Cpu::opCB0x35()
	{
		SWAP_N(HL.lo);
		return 0;
	}
	u8 Cpu::opCB0x36()
	{
		u8 data = read(HL.value);

		u8 upper4 = (data & 0xF0) >> 4;
		u8 lower4 = (data & 0x0F);

		u8 result = ((lower4 & 0xF) << 4) | (upper4 & 0xF);

		setFlag(FLAG_Z, (result == 0));
		clearFlag((FLAG_N | FLAG_H | FLAG_C));

		data = result;
		write(HL.value, data);
		return 0;
	}
	u8 Cpu::opCB0x37()
	{
		SWAP_N(AF.hi);
		return 0;
	}
	u8 Cpu::opCB0x38()
	{
		SRL_N(BC.hi);
		return 0;
	}
	u8 Cpu::opCB0x39()
	{
		SRL_N(BC.lo);
		return 0;
	}
	u8 Cpu::opCB0x3A()
	{
		SRL_N(DE.hi);
		return 0;
	}
	u8 Cpu::opCB0x3B()
	{
		SRL_N(DE.lo);
		return 0;
	}
	u8 Cpu::opCB0x3C()
	{
		SRL_N(HL.hi);
		return 0;
	}
	u8 Cpu::opCB0x3D()
	{
		SRL_N(HL.lo);
		return 0;
	}
	u8 Cpu::opCB0x3E()
	{
		u8 data = read(HL.value);

		u8 lsb = (data & 0x1);
		u8 result = (data >> 1);

		setFlag(FLAG_Z, (result == 0));
		clearFlag((FLAG_N | FLAG_H));

		if (lsb) {
			setFlag(FLAG_C);
		}
		else {
			clearFlag(FLAG_C);
		}

		data >>= 1;
		data = resetBit(data, 7);
		write(HL.value, data);

		return 0;
	}
	u8 Cpu::opCB0x3F()
	{
		SRL_N(AF.hi);
		return 0;
	}
	u8 Cpu::opCB0x40()
	{
		BIT_B_N(0, BC.hi);
		return 0;
	}
	u8 Cpu::opCB0x41()
	{
		BIT_B_N(0, BC.lo);
		return 0;
	}
	u8 Cpu::opCB0x42()
	{
		BIT_B_N(0, DE.hi);
		return 0;
	}
	u8 Cpu::opCB0x43()
	{
		BIT_B_N(0, DE.lo);
		return 0;
	}
	u8 Cpu::opCB0x44()
	{
		BIT_B_N(0, HL.hi);
		return 0;
	}
	u8 Cpu::opCB0x45()
	{
		BIT_B_N(0, HL.lo);
		return 0;
	}
	u8 Cpu::opCB0x46()
	{
		BIT_B_N(0, read(HL.value));
		return 0;
	}
	u8 Cpu::opCB0x47()
	{
		BIT_B_N(0, AF.hi);
		return 0;
	}
	u8 Cpu::opCB0x48()
	{
		BIT_B_N(1, BC.hi);
		return 0;
	}
	u8 Cpu::opCB0x49()
	{
		BIT_B_N(1, BC.lo);
		return 0;
	}
	u8 Cpu::opCB0x4A()
	{
		BIT_B_N(1, DE.hi);
		return 0;
	}
	u8 Cpu::opCB0x4B()
	{
		BIT_B_N(1, DE.lo);
		return 0;
	}
	u8 Cpu::opCB0x4C()
	{
		BIT_B_N(1, HL.hi);
		return 0;
	}
	u8 Cpu::opCB0x4D()
	{
		BIT_B_N(1, HL.lo);
		return 0;
	}
	u8 Cpu::opCB0x4E()
	{
		BIT_B_N(1, read(HL.value));
		return 0;
	}
	u8 Cpu::opCB0x4F()
	{
		BIT_B_N(1, AF.hi);
		return 0;
	}
	u8 Cpu::opCB0x50()
	{
		BIT_B_N(2, BC.hi);
		return 0;
	}
	u8 Cpu::opCB0x51()
	{
		BIT_B_N(2, BC.lo);
		return 0;
	}
	u8 Cpu::opCB0x52()
	{
		BIT_B_N(2, DE.hi);
		return 0;
	}
	u8 Cpu::opCB0x53()
	{
		BIT_B_N(2, DE.lo);
		return 0;
	}
	u8 Cpu::opCB0x54()
	{
		BIT_B_N(2, HL.hi);
		return 0;
	}
	u8 Cpu::opCB0x55()
	{
		BIT_B_N(2, HL.lo);
		return 0;
	}
	u8 Cpu::opCB0x56()
	{
		BIT_B_N(2, read(HL.value));
		return 0;
	}
	u8 Cpu::opCB0x57()
	{
		BIT_B_N(2, AF.hi);
		return 0;
	}
	u8 Cpu::opCB0x58()
	{
		BIT_B_N(3, BC.hi);
		return 0;
	}
	u8 Cpu::opCB0x59()
	{
		BIT_B_N(3, BC.lo);
		return 0;
	}
	u8 Cpu::opCB0x5A()
	{
		BIT_B_N(3, DE.hi);
		return 0;
	}
	u8 Cpu::opCB0x5B()
	{
		BIT_B_N(3, DE.lo);
		return 0;
	}
	u8 Cpu::opCB0x5C()
	{
		BIT_B_N(3, HL.hi);
		return 0;
	}
	u8 Cpu::opCB0x5D()
	{
		BIT_B_N(3, HL.lo);
		return 0;
	}
	u8 Cpu::opCB0x5E()
	{
		BIT_B_N(3, read(HL.value));
		return 0;
	}
	u8 Cpu::opCB0x5F()
	{
		BIT_B_N(3, AF.hi);
		return 0;
	}
	u8 Cpu::opCB0x60()
	{
		BIT_B_N(4, BC.hi);
		return 0;
	}
	u8 Cpu::opCB0x61()
	{
		BIT_B_N(4, BC.lo);
		return 0;
	}
	u8 Cpu::opCB0x62()
	{
		BIT_B_N(4, DE.hi);
		return 0;
	}
	u8 Cpu::opCB0x63()
	{
		BIT_B_N(4, DE.lo);
		return 0;
	}
	u8 Cpu::opCB0x64()
	{
		BIT_B_N(4, HL.hi);
		return 0;
	}
	u8 Cpu::opCB0x65()
	{
		BIT_B_N(4, HL.lo);
		return 0;
	}
	u8 Cpu::opCB0x66()
	{
		BIT_B_N(4, read(HL.value));
		return 0;
	}
	u8 Cpu::opCB0x67()
	{
		BIT_B_N(4, AF.hi);
		return 0;
	}
	u8 Cpu::opCB0x68()
	{
		BIT_B_N(5, BC.hi);
		return 0;
	}
	u8 Cpu::opCB0x69()
	{
		BIT_B_N(5, BC.lo);
		return 0;
	}
	u8 Cpu::opCB0x6A()
	{
		BIT_B_N(5, DE.hi);
		return 0;
	}
	u8 Cpu::opCB0x6B()
	{
		BIT_B_N(5, DE.lo);
		return 0;
	}
	u8 Cpu::opCB0x6C()
	{
		BIT_B_N(5, HL.hi);
		return 0;
	}
	u8 Cpu::opCB0x6D()
	{
		BIT_B_N(5, HL.lo);
		return 0;
	}
	u8 Cpu::opCB0x6E()
	{
		BIT_B_N(5, read(HL.value));
		return 0;
	}
	u8 Cpu::opCB0x6F()
	{
		BIT_B_N(5, AF.hi);
		return 0;
	}
	u8 Cpu::opCB0x70()
	{
		BIT_B_N(6, BC.hi);
		return 0;
	}
	u8 Cpu::opCB0x71()
	{
		BIT_B_N(6, BC.lo);
		return 0;
	}
	u8 Cpu::opCB0x72()
	{
		BIT_B_N(6, DE.hi);
		return 0;
	}
	u8 Cpu::opCB0x73()
	{
		BIT_B_N(6, DE.lo);
		return 0;
	}
	u8 Cpu::opCB0x74()
	{
		BIT_B_N(6, HL.hi);
		return 0;
	}
	u8 Cpu::opCB0x75()
	{
		BIT_B_N(6, HL.lo);
		return 0;
	}
	u8 Cpu::opCB0x76()
	{
		BIT_B_N(6, read(HL.value));
		return 0;
	}
	u8 Cpu::opCB0x77()
	{
		BIT_B_N(6, AF.hi);
		return 0;
	}
	u8 Cpu::opCB0x78()
	{
		BIT_B_N(7, BC.hi);
		return 0;
	}
	u8 Cpu::opCB0x79()
	{
		BIT_B_N(7, BC.lo);
		return 0;
	}
	u8 Cpu::opCB0x7A()
	{
		BIT_B_N(7, DE.hi);
		return 0;
	}
	u8 Cpu::opCB0x7B()
	{
		BIT_B_N(7, DE.lo);
		return 0;
	}
	u8 Cpu::opCB0x7C()
	{
		BIT_B_N(7, HL.hi);
		return 0;
	}
	u8 Cpu::opCB0x7D()
	{
		BIT_B_N(7, HL.lo);
		return 0;
	}
	u8 Cpu::opCB0x7E()
	{
		BIT_B_N(7, read(HL.value));
		return 0;
	}
	u8 Cpu::opCB0x7F()
	{
		BIT_B_N(7, AF.hi);
		return 0;
	}
	u8 Cpu::opCB0x80()
	{
		RES_B_N(0, BC.hi);
		return 0;
	}
	u8 Cpu::opCB0x81()
	{
		RES_B_N(0, BC.lo);
		return 0;
	}
	u8 Cpu::opCB0x82()
	{
		RES_B_N(0, DE.hi);
		return 0;
	}
	u8 Cpu::opCB0x83()
	{
		RES_B_N(0, DE.lo);
		return 0;
	}
	u8 Cpu::opCB0x84()
	{
		RES_B_N(0, HL.hi);
		return 0;
	}
	u8 Cpu::opCB0x85()
	{
		RES_B_N(0, HL.lo);
		return 0;
	}
	u8 Cpu::opCB0x86()
	{
		u8 data = read(HL.value);
		data = resetBit(data, 0);
		write(HL.value, data);

		return 0;
	}
	u8 Cpu::opCB0x87()
	{
		RES_B_N(0, AF.hi);
		return 0;
	}
	u8 Cpu::opCB0x88()
	{
		RES_B_N(1, BC.hi);
		return 0;
	}
	u8 Cpu::opCB0x89()
	{
		RES_B_N(1, BC.lo);
		return 0;
	}
	u8 Cpu::opCB0x8A()
	{
		RES_B_N(1, DE.hi);
		return 0;
	}
	u8 Cpu::opCB0x8B()
	{
		RES_B_N(1, DE.lo);
		return 0;
	}
	u8 Cpu::opCB0x8C()
	{
		RES_B_N(1, HL.hi);
		return 0;
	}
	u8 Cpu::opCB0x8D()
	{
		RES_B_N(1, HL.lo);
		return 0;
	}
	u8 Cpu::opCB0x8E()
	{
		u8 data = read(HL.value);
		data = resetBit(data, 1);
		write(HL.value, data);

		return 0;
	}
	u8 Cpu::opCB0x8F()
	{
		RES_B_N(1, AF.hi);
		return 0;
	}
	u8 Cpu::opCB0x90()
	{
		RES_B_N(2, BC.hi);
		return 0;
	}
	u8 Cpu::opCB0x91()
	{
		RES_B_N(2, BC.lo);
		return 0;
	}
	u8 Cpu::opCB0x92()
	{
		RES_B_N(2, DE.hi);
		return 0;
	}
	u8 Cpu::opCB0x93()
	{
		RES_B_N(2, DE.lo);
		return 0;
	}
	u8 Cpu::opCB0x94()
	{
		RES_B_N(2, HL.hi);
		return 0;
	}
	u8 Cpu::opCB0x95()
	{
		RES_B_N(2, HL.lo);
		return 0;
	}
	u8 Cpu::opCB0x96()
	{
		u8 data = read(HL.value);
		data = resetBit(data, 2);
		write(HL.value, data);

		return 0;
	}
	u8 Cpu::opCB0x97()
	{
		RES_B_N(2, AF.hi);
		return 0;
	}
	u8 Cpu::opCB0x98()
	{
		RES_B_N(3, BC.hi);
		return 0;
	}
	u8 Cpu::opCB0x99()
	{
		RES_B_N(3, BC.lo);
		return 0;
	}
	u8 Cpu::opCB0x9A()
	{
		RES_B_N(3, DE.hi);
		return 0;
	}
	u8 Cpu::opCB0x9B()
	{
		RES_B_N(3, DE.lo);
		return 0;
	}
	u8 Cpu::opCB0x9C()
	{
		RES_B_N(3, HL.hi);
		return 0;
	}
	u8 Cpu::opCB0x9D()
	{
		RES_B_N(3, HL.lo);
		return 0;
	}
	u8 Cpu::opCB0x9E()
	{
		u8 data = read(HL.value);
		data = resetBit(data, 3);
		write(HL.value, data);

		return 0;
	}
	u8 Cpu::opCB0x9F()
	{
		RES_B_N(3, AF.hi);
		return 0;
	}
	u8 Cpu::opCB0xA0()
	{
		RES_B_N(4, BC.hi);
		return 0;
	}
	u8 Cpu::opCB0xA1()
	{
		RES_B_N(4, BC.lo);
		return 0;
	}
	u8 Cpu::opCB0xA2()
	{
		RES_B_N(4, DE.hi);
		return 0;
	}
	u8 Cpu::opCB0xA3()
	{
		RES_B_N(4, DE.lo);
		return 0;
	}
	u8 Cpu::opCB0xA4()
	{
		RES_B_N(4, HL.hi);
		return 0;
	}
	u8 Cpu::opCB0xA5()
	{
		RES_B_N(4, HL.lo);
		return 0;
	}
	u8 Cpu::opCB0xA6()
	{
		u8 data = read(HL.value);
		data = resetBit(data, 4);
		write(HL.value, data);

		return 0;
	}
	u8 Cpu::opCB0xA7()
	{
		RES_B_N(4, AF.hi);
		return 0;
	}
	u8 Cpu::opCB0xA8()
	{
		RES_B_N(5, BC.hi);
		return 0;
	}
	u8 Cpu::opCB0xA9()
	{
		RES_B_N(5, BC.lo);
		return 0;
	}
	u8 Cpu::opCB0xAA()
	{
		RES_B_N(5, DE.hi);
		return 0;
	}
	u8 Cpu::opCB0xAB()
	{
		RES_B_N(5, DE.lo);
		return 0;
	}
	u8 Cpu::opCB0xAC()
	{
		RES_B_N(5, HL.hi);
		return 0;
	}
	u8 Cpu::opCB0xAD()
	{
		RES_B_N(5, HL.lo);
		return 0;
	}
	u8 Cpu::opCB0xAE()
	{
		u8 data = read(HL.value);
		data = resetBit(data, 5);
		write(HL.value, data);

		return 0;
	}
	u8 Cpu::opCB0xAF()
	{
		RES_B_N(5, AF.hi);
		return 0;
	}
	u8 Cpu::opCB0xB0()
	{
		RES_B_N(6, BC.hi);
		return 0;
	}
	u8 Cpu::opCB0xB1()
	{
		RES_B_N(6, BC.lo);
		return 0;
	}
	u8 Cpu::opCB0xB2()
	{
		RES_B_N(6, DE.hi);
		return 0;
	}
	u8 Cpu::opCB0xB3()
	{
		RES_B_N(6, DE.lo);
		return 0;
	}
	u8 Cpu::opCB0xB4()
	{
		RES_B_N(6, HL.hi);
		return 0;
	}
	u8 Cpu::opCB0xB5()
	{
		RES_B_N(6, HL.lo);
		return 0;
	}
	u8 Cpu::opCB0xB6()
	{
		u8 data = read(HL.value);
		data = resetBit(data, 6);
		write(HL.value, data);

		return 0;
	}
	u8 Cpu::opCB0xB7()
	{
		RES_B_N(6, AF.hi);
		return 0;
	}
	u8 Cpu::opCB0xB8()
	{
		RES_B_N(7, BC.hi);
		return 0;
	}
	u8 Cpu::opCB0xB9()
	{
		RES_B_N(7, BC.lo);
		return 0;
	}
	u8 Cpu::opCB0xBA()
	{
		RES_B_N(7, DE.hi);
		return 0;
	}
	u8 Cpu::opCB0xBB()
	{
		RES_B_N(7, DE.lo);
		return 0;
	}
	u8 Cpu::opCB0xBC()
	{
		RES_B_N(7, HL.hi);
		return 0;
	}
	u8 Cpu::opCB0xBD()
	{
		RES_B_N(7, HL.lo);
		return 0;
	}
	u8 Cpu::opCB0xBE()
	{
		u8 data = read(HL.value);
		data = resetBit(data, 7);
		write(HL.value, data);

		return 0;
	}
	u8 Cpu::opCB0xBF()
	{
		RES_B_N(7, AF.hi);
		return 0;
	}
	u8 Cpu::opCB0xC0()
	{
		SET_B_N(0, BC.hi);
		return 0;
	}
	u8 Cpu::opCB0xC1()
	{
		SET_B_N(0, BC.lo);
		return 0;
	}
	u8 Cpu::opCB0xC2()
	{
		SET_B_N(0, DE.hi);
		return 0;
	}
	u8 Cpu::opCB0xC3()
	{
		SET_B_N(0, DE.lo);
		return 0;
	}
	u8 Cpu::opCB0xC4()
	{
		SET_B_N(0, HL.hi);
		return 0;
	}
	u8 Cpu::opCB0xC5()
	{
		SET_B_N(0, HL.lo);
		return 0; 
	}
	u8 Cpu::opCB0xC6()
	{
		u8 data = read(HL.value);
		data = setBit(data, 0);
		write(HL.value, data);

		return 0;
	}
	u8 Cpu::opCB0xC7()
	{
		SET_B_N(0, AF.hi);
		return 0;
	}
	u8 Cpu::opCB0xC8()
	{
		SET_B_N(1, BC.hi);
		return 0;
	}
	u8 Cpu::opCB0xC9()
	{
		SET_B_N(1, BC.lo);
		return 0;
	}
	u8 Cpu::opCB0xCA()
	{
		SET_B_N(1, DE.hi);
		return 0;
	}
	u8 Cpu::opCB0xCB()
	{
		SET_B_N(1, DE.lo);
		return 0;
	}
	u8 Cpu::opCB0xCC()
	{
		SET_B_N(1, HL.hi);
		return 0;
	}
	u8 Cpu::opCB0xCD()
	{
		SET_B_N(1, HL.lo);
		return 0;
	}
	u8 Cpu::opCB0xCE()
	{
		u8 data = read(HL.value);
		data = setBit(data, 1);
		write(HL.value, data);

		return 0;
	}
	u8 Cpu::opCB0xCF()
	{
		SET_B_N(1, AF.hi);
		return 0;
	}
	u8 Cpu::opCB0xD0()
	{
		SET_B_N(2, BC.hi);
		return 0;
	}
	u8 Cpu::opCB0xD1()
	{
		SET_B_N(2, BC.lo);
		return 0;
	}
	u8 Cpu::opCB0xD2()
	{
		SET_B_N(2, DE.hi);
		return 0;
	}
	u8 Cpu::opCB0xD3()
	{
		SET_B_N(2, DE.lo);
		return 0;
	}
	u8 Cpu::opCB0xD4()
	{
		SET_B_N(2, HL.hi);
		return 0;
	}
	u8 Cpu::opCB0xD5()
	{
		SET_B_N(2, HL.lo);
		return 0;
	}
	u8 Cpu::opCB0xD6()
	{
		u8 data = read(HL.value);
		data = setBit(data, 2);
		write(HL.value, data);

		return 0;
	}
	u8 Cpu::opCB0xD7()
	{
		SET_B_N(2, AF.hi);
		return 0;
	}
	u8 Cpu::opCB0xD8()
	{
		SET_B_N(3, BC.hi);
		return 0;
	}
	u8 Cpu::opCB0xD9()
	{
		SET_B_N(3, BC.lo);
		return 0;
	}
	u8 Cpu::opCB0xDA()
	{
		SET_B_N(3, DE.hi);
		return 0;
	}
	u8 Cpu::opCB0xDB()
	{
		SET_B_N(3, DE.lo);
		return 0;
	}
	u8 Cpu::opCB0xDC()
	{
		SET_B_N(3, HL.hi);
		return 0;
	}
	u8 Cpu::opCB0xDD()
	{
		SET_B_N(3, HL.lo);
		return 0;
	}
	u8 Cpu::opCB0xDE()
	{
		u8 data = read(HL.value);
		data = setBit(data, 3);
		write(HL.value, data);

		return 0;
	}
	u8 Cpu::opCB0xDF()
	{
		SET_B_N(3, AF.hi);
		return 0;
	}
	u8 Cpu::opCB0xE0()
	{
		SET_B_N(4, BC.hi);
		return 0;
	}
	u8 Cpu::opCB0xE1()
	{
		SET_B_N(4, BC.lo);
		return 0;
	}
	u8 Cpu::opCB0xE2()
	{
		SET_B_N(4, DE.hi);
		return 0;
	}
	u8 Cpu::opCB0xE3()
	{
		SET_B_N(4, DE.lo);
		return 0;
	}
	u8 Cpu::opCB0xE4()
	{
		SET_B_N(4, HL.hi);
		return 0;
	}
	u8 Cpu::opCB0xE5()
	{
		SET_B_N(4, HL.lo);
		return 0;
	}
	u8 Cpu::opCB0xE6()
	{
		u8 data = read(HL.value);
		data = setBit(data, 4);
		write(HL.value, data);

		return 0;
	}
	u8 Cpu::opCB0xE7()
	{
		SET_B_N(4, AF.hi);
		return 0;
	}
	u8 Cpu::opCB0xE8()
	{
		SET_B_N(5, BC.hi);
		return 0;
	}
	u8 Cpu::opCB0xE9()
	{
		SET_B_N(5, BC.lo);
		return 0;
	}
	u8 Cpu::opCB0xEA()
	{
		SET_B_N(5, DE.hi);
		return 0;
	}
	u8 Cpu::opCB0xEB()
	{
		SET_B_N(5, DE.lo);
		return 0;
	}
	u8 Cpu::opCB0xEC()
	{
		SET_B_N(5, HL.hi);
		return 0;
	}
	u8 Cpu::opCB0xED()
	{
		SET_B_N(5, HL.lo);
		return 0;
	}
	u8 Cpu::opCB0xEE()
	{
		u8 data = read(HL.value);
		data = setBit(data, 5);
		write(HL.value, data);

		return 0;
	}
	u8 Cpu::opCB0xEF()
	{
		SET_B_N(5, AF.hi);
		return 0;
	}
	u8 Cpu::opCB0xF0()
	{
		SET_B_N(6, BC.hi);
		return 0;
	}
	u8 Cpu::opCB0xF1()
	{
		SET_B_N(6, BC.lo);
		return 0;
	}
	u8 Cpu::opCB0xF2()
	{
		SET_B_N(6, DE.hi);
		return 0;
	}
	u8 Cpu::opCB0xF3()
	{
		SET_B_N(6, DE.lo);
		return 0;
	}
	u8 Cpu::opCB0xF4()
	{
		SET_B_N(6, HL.hi);
		return 0;
	}
	u8 Cpu::opCB0xF5()
	{
		SET_B_N(6, HL.lo);
		return 0;
	}
	u8 Cpu::opCB0xF6()
	{
		u8 data = read(HL.value);
		data = setBit(data, 6);
		write(HL.value, data);

		return 0;
	}
	u8 Cpu::opCB0xF7()
	{
		SET_B_N(6, AF.hi);
		return 0;
	}
	u8 Cpu::opCB0xF8()
	{
		SET_B_N(7, BC.hi);
		return 0;
	}
	u8 Cpu::opCB0xF9()
	{
		SET_B_N(7, BC.lo);
		return 0;
	}
	u8 Cpu::opCB0xFA()
	{
		SET_B_N(7, DE.hi);
		return 0;
	}
	u8 Cpu::opCB0xFB()
	{
		SET_B_N(7, DE.lo);
		return 0;
	}
	u8 Cpu::opCB0xFC()
	{
		SET_B_N(7, HL.hi);
		return 0;
	}
	u8 Cpu::opCB0xFD()
	{
		SET_B_N(7, HL.lo);
		return 0;
	}
	u8 Cpu::opCB0xFE()
	{
		u8 data = read(HL.value);
		data = setBit(data, 7);
		write(HL.value, data);

		return 0;
	}
	u8 Cpu::opCB0xFF()
	{
		SET_B_N(7, AF.hi);
		return 0;
	}
}

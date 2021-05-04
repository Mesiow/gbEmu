#pragma once
#include "../include/Joypad.h"
#include "../include/MMU.h"

namespace gbEmu {
	Joypad::Joypad(MMU* mmu)
	{
		this->mmu = mmu;
		write(JOYPAD, 0xFF); // 1 = not pressed, 0 = pressed
	}

	u8 Joypad::read(u16 address)
	{
		return mmu->read(address);
	}

	void Joypad::write(u16 address, u8 value)
	{
		mmu->write(address, value);
	}

	void Joypad::keyPressed(Key key)
	{

	}

	void Joypad::keyReleased(Key key)
	{
	}

	u8 Joypad::joypadState()
	{

		return 0;
	}

	void Joypad::requestInterrupt()
	{
		u8 IF = read(0xFF0F);
		IF = setBit(IF, JOYPAD_INT);
		write(0xFF0F, IF);
	}
}

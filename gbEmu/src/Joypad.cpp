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
		bool directional = false;
		if (key == Key::ArrowDown || key == Key::ArrowLeft ||
			key == Key::ArrowRight || key == Key::ArrowUp) {
			directional = true;
		}

		u8 keyBit = getKeyBit(key);

		u8 joy = directional ? directionals : actions;
		bool unset = testBit(joy, keyBit);

		if (unset) {
			//Press button
			joy = resetBit(joy, keyBit);

			//Update according input bit values based on what was pressed
			if (directional) {
				directionals = joy;
			}
			else {
				actions = joy;
			}
			requestInterrupt();
		}
	}

	void Joypad::keyReleased(Key key)
	{
		bool directional = false;
		if (key == Key::ArrowDown || key == Key::ArrowLeft ||
			key == Key::ArrowRight || key == Key::ArrowUp) {
			directional = true;
		}

		u8 keyBit = getKeyBit(key);

		u8 joy = directional ? directionals : actions;
		bool unset = testBit(joy, keyBit);

		if (!unset) {
			joy = setBit(joy, keyBit);

			//Update according input bit values based on what was released
			if (directional) {
				directionals = joy;
			}
			else {
				actions = joy;
			}
		}
	}

	u8 Joypad::getKeyBit(Key key)
	{
		if (key == Key::ArrowRight || key == Key::A) return 0;
		if (key == Key::ArrowLeft || key == Key::S) return 1;
		if (key == Key::ArrowUp || key == Key::LShift) return 2;
		if (key == Key::ArrowDown || key == Key::Enter) return 3;
	}

	u8 Joypad::joypadState()
	{
		u8 joy = mmu->memory[JOYPAD];

		//Determine what input the game is looking for
		if (joy == 0x10) {
			return actions;
		}
		else if (joy == 0x20) {
			return directionals;
		}
		else {
			return 0xFF;
		}
	}

	void Joypad::requestInterrupt()
	{
		u8 IF = read(0xFF0F);
		IF = setBit(IF, JOYPAD_INT);
		write(0xFF0F, IF);
	}
}

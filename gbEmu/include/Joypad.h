#pragma once
#include "Utils.h"

namespace gbEmu {
	#define JOYPAD 0xFF00
	#define JOYPAD_INT 4

	enum class Button {
		Action = 0,
		Directional
	};

	enum class Key : u8{
		ArrowRight,
		ArrowLeft,
		ArrowUp,
		ArrowDown,
		
		A,
		S,
		LShift,
		Enter
	};

	struct MMU;
	struct Joypad {
		Joypad(MMU* mmu);

		u8 read(u16 address);
		void write(u16 address, u8 value);

		void keyPressed(Key key);
		void keyReleased(Key key);

		u8 getKeyBit(Key key);

		u8 joypadState();

		void requestInterrupt();

		u8 directionals = 0xF; //Directional button bits
		u8 actions = 0xF; //Action button bits

		MMU* mmu;
		Button buttons;
	};
}
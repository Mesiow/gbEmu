#pragma once
#include "Utils.h"

namespace gbEmu {
	#define JOYPAD 0xFF00
	#define JOYPAD_INT 4

	enum class Button {
		Select = 0,
		Directional
	};

	enum class Key {

	};

	struct MMU;
	struct Joypad {
		Joypad(MMU* mmu);

		u8 read(u16 address);
		void write(u16 address, u8 value);

		void keyPressed(Key key);
		void keyReleased(Key key);

		u8 joypadState();

		void requestInterrupt();

		MMU* mmu;
	};
}
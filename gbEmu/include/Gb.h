#pragma once
#include <SFML\Graphics.hpp>
#include "Cpu.h"
#include "MMU.h"
#include "Ppu.h"
#include "Joypad.h"
#include "Cartridge.h"

namespace gbEmu {
	struct Gb {
		Gb();
		void update();
		void render(sf::RenderTarget& target);

		MMU mmu;
		Cpu cpu;
		Ppu ppu;
		Joypad joypad;

		Cartridge cart;
	};
}
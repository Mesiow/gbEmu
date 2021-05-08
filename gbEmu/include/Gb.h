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

		void handleEvents(sf::Event& ev);
		void handleKeyReleased(sf::Event &ev);
		void handleKeyPressed(sf::Event &ev);

		MMU mmu;
		Cpu cpu;
		Ppu ppu;
		Joypad joypad;

		Cartridge cart;
	};
}
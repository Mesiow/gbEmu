#pragma once
#include <SFML\Graphics.hpp>
#include "Cpu.h"
#include "MMU.h"
#include "Ppu.h"
#include "Joypad.h"
#include "Cartridge.h"
#include <chrono>
#include <thread>

namespace gbEmu {
	struct Gb {
		Gb();
		void update(float dt);
		void render(sf::RenderTarget& target);

		void handleEvents(sf::Event& ev);
		void handleKeyReleased(sf::Event &ev);
		void handleKeyPressed(sf::Event &ev);

		MMU mmu;
		Cpu cpu;
		Ppu ppu;
		Joypad joypad;

		Cartridge cart;

		const int maxCycles = 70224;
		const float fps = 59.72f;
		const float delayTime = 1000.0f / fps;
	};
}
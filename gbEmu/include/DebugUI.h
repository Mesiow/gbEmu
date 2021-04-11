#pragma once
#include "imgui.h"
#include "imgui_memory_editor.h"
#include <SFML/Graphics.hpp>
#include <string>

namespace gbEmu {
	struct MMU;
	struct Cpu;
	struct DebugUI {
		DebugUI(MMU* mmu, Cpu *cpu);
		void render();
		void update();
		void handleButtonPresses();
		void handleEvents(sf::Event& ev);

		static MemoryEditor mainMemory;
		static MemoryEditor bootRomMemory;
		MMU* mmu;
		Cpu* cpu;

		bool stepPressed = false;
		bool runPressed = false;
		bool haltPressed = false;
		bool running = false;
		std::string msg = "";

		bool haltAtPos = false;
	};
}

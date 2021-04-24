#pragma once
#include "imgui.h"
#include "imgui_memory_editor.h"
#include <SFML/Graphics.hpp>
#include <string>

namespace gbEmu {
	struct Gb;
	struct DebugUI {
		DebugUI(Gb *gb);
		void render();
		void renderGPU();
		void update();
		void handleButtonPresses();
		void handleEvents(sf::Event& ev);

		static MemoryEditor mainMemory;
		static MemoryEditor bootRomMemory;
		Gb* gb;

		bool stepPressed = false;
		bool runPressed = false;
		bool haltPressed = false;
		bool running = false;
		std::string msg = "";

		bool haltAtPos = false;
	};
}

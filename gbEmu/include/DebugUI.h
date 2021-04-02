#pragma once
#include "imgui.h"
#include "imgui_memory_editor.h"

namespace gbEmu {
	struct MMU;
	struct Cpu;
	struct DebugUI {
		DebugUI(MMU* mmu, Cpu *cpu);
		void render();
		void update();
		void handleButtonPresses();

		static MemoryEditor ed;
		MMU* mmu;
		Cpu* cpu;

		bool stepPressed = false;
		bool runPressed = false;
		bool haltPressed = false;
	};
}

#pragma once
#include "imgui.h"
#include "imgui_memory_editor.h"

namespace gbEmu {
	struct MMU;
	struct Cpu;
	struct DebugUI {
		DebugUI(MMU* mmu, Cpu *cpu);
		void render();

		static MemoryEditor ed;
		MMU* mmu;
		Cpu* cpu;
	};
}
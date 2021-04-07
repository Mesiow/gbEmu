#include "../include/DebugUI.h"
#include "../include/MMU.h"
#include "../include/Cpu.h"

namespace gbEmu {

    MemoryEditor DebugUI::ed;

    DebugUI::DebugUI(MMU* mmu, Cpu *cpu) {
        this->mmu = mmu;
        this->cpu = cpu;
    }

    void DebugUI::render() {

        ImGui::Begin("Status/Registers");

        ImGui::Text("Status: ");
        ImGui::SameLine();

        if (cpu->getFlag(FLAG_Z)) {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(133, 245, 29, 255)));
            ImGui::Text("%c", 'Z');
            ImGui::SameLine();
            ImGui::PopStyleColor(1);
        }
        else {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(245, 50, 29, 255)));
            ImGui::Text("%c", 'Z');
            ImGui::SameLine();
            ImGui::PopStyleColor(1);
        }

        if (cpu->getFlag(FLAG_N)) {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(133, 245, 29, 255)));
            ImGui::Text("%c", 'N');
            ImGui::SameLine();
            ImGui::PopStyleColor(1);
        }
        else {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(245, 50, 29, 255)));
            ImGui::Text("%c", 'N');
            ImGui::SameLine();
            ImGui::PopStyleColor(1);
        }

        if (cpu->getFlag(FLAG_H)) {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(133, 245, 29, 255)));
            ImGui::Text("%c", 'H');
            ImGui::SameLine();
            ImGui::PopStyleColor(1);
        }
        else {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(245, 50, 29, 255)));
            ImGui::Text("%c", 'H');
            ImGui::SameLine();
            ImGui::PopStyleColor(1);
        }

        if (cpu->getFlag(FLAG_C)) {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(133, 245, 29, 255)));
            ImGui::Text("%c", 'C');
            ImGui::SameLine();
            ImGui::PopStyleColor(1);
        }
        else {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(245, 50, 29, 255)));
            ImGui::Text("%c", 'C');
            ImGui::SameLine();
            ImGui::PopStyleColor(1);
        }
        


        ImGui::NewLine();
        ImGui::Text("PC: 0x%04X", cpu->PC);
        ImGui::Text("AF: 0x%04X", cpu->AF.value);
        ImGui::Text("BC: 0x%04X", cpu->BC.value);
        ImGui::Text("DE: 0x%04X", cpu->DE.value);
        ImGui::Text("HL: 0x%04X", cpu->HL.value);
        ImGui::Text("SP: 0x%04X", cpu->SP);
        ImGui::NewLine();

        const char* i = "LD BC,u16";
        ImGui::Text("Instruction: %s", i);

       
        stepPressed = ImGui::Button("Step");
        ImGui::SameLine();
        runPressed = ImGui::Button("Run");
        ImGui::SameLine();
        haltPressed = ImGui::Button("Halt");
        ImGui::SameLine();

        ed.DrawWindow("Memory Editor", mmu->memory, MAX_MEM);

        ImGui::End();
    }

    void DebugUI::update()
    {
        handleButtonPresses();
    }

    void DebugUI::handleButtonPresses()
    {
        if (stepPressed) {
              cpu->clock();
              while (cpu->cycles > 0)
                  cpu->clock();
        }
        if (runPressed) {
            printf("Running cpu emulation\n");
        }
        if (haltPressed) {
            printf("Halting cpu emulation");
        }
    }

}
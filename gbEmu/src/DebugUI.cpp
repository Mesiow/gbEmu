#include "../include/DebugUI.h"
#include "../include/Gb.h"

namespace gbEmu {

    MemoryEditor DebugUI::mainMemory;
    MemoryEditor DebugUI::bootRomMemory;

    DebugUI::DebugUI(Gb *gb) {
        this->gb = gb;
    }

    void DebugUI::render() {

        if (show) {

            ImGui::Begin("Status/Registers");

            ImGui::Text("Status: ");
            ImGui::SameLine();

            Cpu* cpu = &gb->cpu;
            MMU* mmu = &gb->mmu;

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

            mainMemory.DrawWindow("Memory Editor", mmu->memory, MAX_MEM);
            //bootRomMemory.DrawWindow("Boot Rom", mmu->bootrom, 0x100);

            ImGui::End();

            renderGPU();
        }
    }

    void DebugUI::renderGPU()
    {
        ImGui::SetNextWindowSizeConstraints(ImVec2(100.f, 100.f), ImVec2(800.f, 800.f));
        ImGui::Begin("GPU");
        {

            //ImVec2 pos = ImGui::GetCursorScreenPos();
            //uint32_t tex = gb->ppu.bgLayer.getTexture()->getNativeHandle();

            //ImVec2 size = ImGui::GetWindowSize();
            //size.y -= 46;

            //ImGui::Image(tex, size /*ImVec2(0, 0), ImVec2(0.625, 0.5625)*/);
            //ImGui::End();
            //Tabs


           /* ImGui::BeginTabBar("Options");
            if (ImGui::TabItemButton("Display")) {

            }
            if (ImGui::TabItemButton("Background")) {

            }
            if (ImGui::TabItemButton("Tiles")) {

            }
            if (ImGui::TabItemButton("Sprites")) {

            }
            ImGui::EndTabBar();*/



            /*  ImVec2 imgPos = ImGui::GetCursorScreenPos();
                if (selectedTab == 0)
                {
                    ImGui::Image((ImTextureID)screenTexture, ImVec2(SCREEN_WIDTH * 2.f, SCREEN_HEIGHT * 2.f));
                }
                else if (selectedTab == 1)
                {
                    set_texture(screenBgTexture, SCREEN_WIDTH, SCREEN_HEIGHT, GB_bgscreen());
                    ImGui::Image((ImTextureID)screenBgTexture, ImVec2(SCREEN_WIDTH * 2.f, SCREEN_HEIGHT * 2.f));
                    imgui_drawgrid(imgPos, 20, 18, SCREEN_WIDTH * 2.f, SCREEN_HEIGHT * 2.f);
                }
                else if (selectedTab == 2)
                {
                    set_texture(tileTexture, TILES_WIDTH, TILES_HEIGHT, GB_tiledata());
                    ImGui::Image((ImTextureID)tileTexture, ImVec2(TILES_WIDTH * 2.f, TILES_HEIGHT * 2.f));
                    imgui_drawgrid(imgPos, 16, 24, TILES_WIDTH * 2.f, TILES_HEIGHT * 2.f);
                }
                else if (selectedTab == 3)
                {
                    set_texture(tileTexture, TILES_WIDTH, TILES_HEIGHT, GB_tiledata());
                    const Sprite* sprites = reinterpret_cast<Sprite*>(&gb.memory[0xFE00]);
                    ImGui::Columns(8);
                    for (int i = 0; i < 8 * 5; i++)
                    {
                        const Sprite& sprite = sprites[i];
                        ImVec2 pos = ImVec2((sprite.TileNum % 16) / 16.f, (sprite.TileNum / 16) / 24.f);
                        ImGui::Image((ImTextureID)tileTexture, ImVec2(16.f * 2.f, 16.f * 2.f), pos, ImVec2(pos.x + 1.f / 16.f, pos.y + 1.f / 24.f));
                        ImGui::SameLine();
                        ImGui::Text("%02X\n%02X\n%02X\n%02X", sprite.YPos, sprite.XPos, sprite.TileNum, sprite.Attributes);
                        ImGui::NextColumn();
                    }
                    ImGui::Columns(1);
                }
            }
            ImGui::End();*/
        }
    }

    void DebugUI::update()
    {
        handleButtonPresses();
        if (running) {
          //gb->update();
           // gb->cpu.clock();
         //   if (haltAtPos) { //Stop PC at this position for debugging
         //          if (gb->cpu.PC == 0x0100) {
         //              gb->cpu.paused = true;
         //          }
         //      }
         //      else {
         //          gb->cpu.paused = false;
         //      }
         }
    }

    void DebugUI::handleButtonPresses()
    {
        if (stepPressed) {
            gb->cpu.paused = false;
            gb->cpu.clock();
            
        }
        if (runPressed) {
            printf("Running cpu emulation\n");
            running = true;
        }
        if (haltPressed) {
            running = false;
            printf("Halting cpu emulation");
        }
    }

    void DebugUI::handleEvents(sf::Event& ev)
    {
        if (ev.type == sf::Event::KeyReleased) {
            if (ev.key.code == sf::Keyboard::LShift) {
                show = !show;
            }
        }

        if (ev.type == sf::Event::KeyReleased) {
            if (ev.key.code == sf::Keyboard::Space) {
                running = !running;
            }
        }
        if (ev.type == sf::Event::KeyReleased) {
            if (ev.key.code == sf::Keyboard::S) {
                gb->cpu.paused = false;
            }
        }

        if (ev.type == sf::Event::KeyReleased) {
            if (ev.key.code == sf::Keyboard::F) {
                haltAtPos = !haltAtPos;
                std::cout << "halt\n";
            }
        }
    }
}
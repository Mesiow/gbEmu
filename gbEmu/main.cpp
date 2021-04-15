#pragma once
#include "imgui-SFML.h"
#include "include/DebugUI.h"
#include "include/MMU.h"
#include "include/Cpu.h"
#include "include\Ppu.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

int main(int arc, char* argv[]) {

    sf::RenderWindow window(sf::VideoMode(768, 560), "gbEmu");
    //window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

   
   
    gbEmu::MMU mmu;
   
   // mmu.loadRom("test_roms/02-interrupts.GB");
    mmu.loadRom("roms/DMG_ROM.GB", true);

    gbEmu::Cpu cpu(&mmu);
    gbEmu::DebugUI ui(&mmu, &cpu);
    gbEmu::Ppu ppu(&mmu);
    ppu.init();
   


    sf::Clock deltaClock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            ui.handleEvents(event);

            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());
       
       // ui.update();
       // ui.render();

        window.clear(sf::Color(100, 149, 237, 255));

        ImGui::SFML::Render(window);
        ppu.draw(window);

        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}
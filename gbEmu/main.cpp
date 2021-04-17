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
#include "include\Gb.h"

int main(int arc, char* argv[]) {

    sf::RenderWindow window(sf::VideoMode(540, 580), "gbEmu");
    ImGui::SFML::Init(window);

   
    gbEmu::Gb gb;
    gbEmu::DebugUI ui(&gb.mmu, &gb.cpu);
  
   


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
       
        ui.update();
        ui.render();
        
        gb.update();

       //window.clear(sf::Color(100, 149, 237, 255);
        window.clear(sf::Color::Black);

      
        gb.render(window);
       // ImGui::SFML::Render(window);
      

        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}
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
    sf::RenderWindow window(sf::VideoMode(160 * 3, 144 * 3), "gbEmu");

    sf::Image icon;
    icon.loadFromFile("icon.jpg");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    window.setVerticalSyncEnabled(true);

    ImGui::SFML::Init(window);

   
    gbEmu::Gb gb;
   //gbEmu::DebugUI ui(&gb);
  
    std::chrono::time_point<std::chrono::high_resolution_clock> current, prev;
    prev = std::chrono::high_resolution_clock::now();

    sf::Clock deltaClock;
    while (window.isOpen()) {
        current = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(current - prev);
        prev = current;

        sf::Event event;
        while (window.pollEvent(event)) {
            //ImGui::SFML::ProcessEvent(event);
            //ui.handleEvents(event);

            gb.handleEvents(event);

            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        //ImGui::SFML::Update(window, deltaClock.restart());
       
        //ui.update();
        //ui.render();
        gb.update(elapsed.count());

        window.clear();
        gb.render(window);
        //ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}
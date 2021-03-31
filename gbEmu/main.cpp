#pragma once
#include "imgui.h"
#include "imgui-SFML.h"
#include "imgui_memory_editor.h"


#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

int main(int arc, char* argv[]) {

    sf::RenderWindow window(sf::VideoMode(768, 560), "gbEmu");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    static MemoryEditor ed;
    static char data[0x10000];
    size_t data_size = 0x10000;
   


    sf::Clock deltaClock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::Begin("Status/Registers");

       
       
        ImGui::Text("Status: ");
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(133, 245, 29, 255)));
        ImGui::Text("%c", 'Z');
        ImGui::SameLine();
        ImGui::PopStyleColor(1);
        ImGui::Text("%c", 'N');
        ImGui::SameLine();
        ImGui::Text("%c", 'H');
        ImGui::SameLine();
        ImGui::Text("%c", 'C');
        ImGui::SameLine();
      

        ImGui::NewLine();
        ImGui::Text("PC: 0x%x", 0x1234);
        ImGui::Text("AF: 0x%x", 0x1234);
        ImGui::Text("BC: 0x%x", 0x1234);
        ImGui::Text("DE: 0x%x", 0x1234);
        ImGui::Text("HL: 0x%x", 0x1234);
        ImGui::Text("SP: 0x%x", 0x1234);
        ed.DrawWindow("Memory Editor", data, data_size);
       
        ImGui::End();

        window.clear(sf::Color(100, 149, 237, 255));
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}
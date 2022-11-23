// первая строка кода - 15 ноября 2022 года, великое событие, отсюда берет свое начало легендарный проект, 
// зародившийся в умах двух школьников школы "Интеллектуал", существовавшей в 2022 году

#include "SFML/Graphics.hpp"
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <string>
#include <iostream>
#include <cmath>
#include <gsl/gsl_linalg.h>
#include <imgui.h>
#include <imgui-SFML.h>



#include "msg.h"
#include "utils.h"
#include "world.h"

#define FONT_SIZE 22.f
#define ROUNDING 5.f
#define BORDER 1.5f
#define WINDOWS_W 800
#define WINDOWS_H 600

int deltaThrust = 1;
int rotationFrame = 1;


void setupImgui()
{
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF(resPath("pixls.ttf").c_str(), FONT_SIZE);
    ImGui::SFML::UpdateFontTexture();
}


void showThrust(float thrust_len, sf::Text &playerTextThrust){

    int f = thrust_len/4;
    sf::String text = "Thrust force: ";
    sf::String out = text + std::to_string(f);
    playerTextThrust.setString(out);

}

void traektory(float Vy, float Vx, float g, sf::VertexBuffer& v_buffer){
    static sf::Vertex verticies[WINDOWS_W + 1];
    for(int x = 0; x <= WINDOWS_W; x++){
        float y = ((Vy/Vx) * x) - (g/(2 * Vx*Vx) * x * x);
        verticies[x].position = sf::Vector2f(-x, y);
        verticies[x].color = sf::Color::White;
        std::cout << x << " " << y << "\n";
    }
    v_buffer.update(verticies);
}

int main()
{
    sf::ContextSettings settings;
    //settings.antialiasingLevel = 2;
    sf::RenderWindow window(sf::VideoMode(WINDOWS_W, WINDOWS_H), "Simulation", sf::Style::Default, settings);
    ImGui::SFML::Init(window, false);
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);

    sf::Font font;
    font.loadFromFile(resPath("pixls.ttf"));

    setupImgui();

    World world(&window);

    sf::View view = window.getView();
    sf::Clock deltaClock;
    sf::Clock deltaClockImgui;
    float dt;

    sf::SoundBuffer spacemus;
    spacemus.loadFromFile(resPath("song1.wav"));
    sf::Sound sound;
    sound.setBuffer(spacemus);
    sound.play();

    Message msg("Let's start!", sf::Vector2f(WINDOWS_W / 2, WINDOWS_H / 2), sf::Color(255, 255, 255, 255), font);
    sf::VertexBuffer vertices(sf::PrimitiveType::LineStrip);
    vertices.create(WINDOWS_W + 1);
    while (window.isOpen())
    {   
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::Resized) {
                // resize view
                view.setSize(
                    static_cast<float>(event.size.width),
                    static_cast<float>(event.size.height)
                );
                window.setView(view);
            }

            // Зум
            if (event.type == sf::Event::MouseWheelScrolled && event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
            {
                if (event.mouseWheelScroll.delta > 0) view.zoom(0.95);
                else view.zoom(1.05);
                window.setView(view);
            }
        }

        
        ImGui::SFML::Update(window, deltaClockImgui.restart());
        
        world.processKeyboard();
        dt = deltaClock.restart().asSeconds();
        world.step(dt);

        // traektory(velocity.y, velocity.x, gravity.y, vertices);
        
        window.clear(sf::Color(14, 32, 46));
        window.draw(vertices);
        world.render();
        msg.draw(window);
        ImGui::SFML::Render(window);
        window.display();
    }
    ImGui::SFML::Shutdown();
    return 0;
}


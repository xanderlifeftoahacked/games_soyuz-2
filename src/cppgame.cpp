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


#define FONT_SIZE 22.f
#define ROUNDING 5.f
#define BORDER 1.5f

#define DEBUG true

#define WINDOWS_W 800
#define WINDOWS_H 600
#define RECTX 50
#define RECTY 126
#define MAX_THRUST 400

int deltaThrust = 1;
int rotationFrame = 1;

std::string resPath(std::string p) { return std::string(RESOURSE_PATH + p); }


void setupImgui()
{
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF(resPath("pixls.ttf").c_str(), FONT_SIZE);
    ImGui::SFML::UpdateFontTexture();
}


int selectThrustFrame(float thrust)
{
    if(thrust == 0) return 0;
    else if(thrust > 0 && thrust <= MAX_THRUST / 4) return 1;
    else if(thrust > MAX_THRUST / 4 && thrust <= MAX_THRUST / 2) return 2;
    else if(thrust <= MAX_THRUST) return 3;
}

void selectRectFrame(int thrustFrame, int rotationFrame, sf::Sprite &sprite)
{
    static int burnTypeCounter = 0;
    int burnType = 0;
    if(burnTypeCounter % 10 >= 5) burnType = 1;
    else if(burnTypeCounter % 10 < 5) burnType = 0;
    else if(burnTypeCounter == 10) burnTypeCounter = 0;
    int oleg = RECTX * thrustFrame * 3 + RECTX * (rotationFrame - 1);
    sprite.setTextureRect(sf::IntRect(oleg, burnType == 1 ? RECTY : 0, RECTX, RECTY));
    burnTypeCounter++;
}

void showThrust(float thrust_len, sf::Text &playerTextThrust){

    int f = thrust_len/4;
    sf::String text = "Thrust force: ";
    sf::String out = text + std::to_string(f);
    playerTextThrust.setString(out);

}

int main()
{
    sf::ContextSettings settings;
    //settings.antialiasingLevel = 2;
    sf::RenderWindow window(sf::VideoMode(WINDOWS_W, WINDOWS_H), "Simulation", sf::Style::Default, settings);
    ImGui::SFML::Init(window, false);
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);
    

    setupImgui();

    // //Ставим шрифт
    // sf::Font font;
    // font.loadFromFile(resPath("pixls.ttf"));

    // //Создаем текст для игрока
    // sf::Text playerTextThrust;
    // playerTextThrust.setFont(font);
    // playerTextThrust.setCharacterSize(15);
    // playerTextThrust.setPosition(sf::Vector2f(100.f,100.f));

    // Спрайт ракет
    sf::Texture texture;
    texture.loadFromFile(resPath("sprites/RocketV01.png"));
    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0, 0, RECTX, RECTY));
    sprite.setOrigin(RECTX/2, RECTY/2);
    //sprite.scale(sf::Vector2f(0.5, 0.5));
    sprite.setPosition(window.getSize().x/2, window.getSize().y/2);

  


    sf::Vector2f thrust(0, 0);
    sf::Vector2f gravity(0, -1);
    sf::Vector2f total_force;
    float mass = 10;
    float thrust_len = sqrt(pow(thrust.x, 2) + pow(thrust.y, 2));
    sf::Vector2f accelretion(0, 0);
    sf::Vector2f velocity(0, 0);
    float angle_velocity = 0;
    float angle_acceleration = 10;
    float angle = 0;

    sf::View view = window.getDefaultView();
    sf::Clock deltaClock;
    sf::Clock deltaClockImgui;
    float dt;

    sf::SoundBuffer spacemus;
    spacemus.loadFromFile(resPath("song1.wav"));
    sf::Sound sound;
    sound.setBuffer(spacemus);
    sound.play();

    while (window.isOpen())
    {   
        //setzoom
        // Declare and load a texture
        // Draw it
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

        dt = deltaClock.restart().asSeconds();
        ImGui::SFML::Update(window, deltaClockImgui.restart());
        

        // Обработка нажатий клавиатуры
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) if(thrust_len < MAX_THRUST) thrust_len += deltaThrust;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) if(thrust_len - 1 >= 0) thrust_len -= 1;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) 
        { 
            angle_velocity += angle_acceleration * dt; angle += angle_velocity * dt + angle_acceleration * pow(dt, 2) / 2;  rotationFrame = 3;
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) 
        { 
            angle_velocity -= angle_acceleration * dt; angle += angle_velocity * dt - angle_acceleration * pow(dt, 2) / 2; rotationFrame = 2;
        }
        else
        {   
            rotationFrame = 1;
            // Имитация гиродина
            if(abs(angle_velocity) < 1) angle_velocity = 0;
            else 
            {
                if(angle_velocity > 0)
                {
                    angle_velocity -= angle_acceleration * dt;
                    angle += angle_velocity * dt - angle_acceleration * pow(dt, 2) / 2;
                }
                else
                {                    angle_velocity += angle_acceleration * dt;
                    angle += angle_velocity * dt + angle_acceleration * pow(dt, 2) / 2;
                }
            }
        }

        //изменение спрайта ракеты
        selectRectFrame(selectThrustFrame(thrust_len), rotationFrame, sprite);
        
        thrust = sf::Vector2f(thrust_len * sin(angle * 3.14 / 180), thrust_len * cos(angle * 3.14 / 180));
        total_force = thrust + gravity;
        accelretion = sf::Vector2f(total_force.x / mass, total_force.y / mass);
        velocity += accelretion * dt;

        sprite.setRotation(angle);
        sprite.move(velocity.x * dt + accelretion.x * pow(dt, 2) / 2, -(velocity.y * dt + accelretion.y * pow(dt, 2) / 2));
        //view.move(velocity.x * dt + accelretion.x * pow(dt, 2) / 2, -(velocity.y * dt + accelretion.y * pow(dt, 2) / 2));
        //playerTextThrust.move(velocity.x * dt + accelretion.x * pow(dt, 2) / 2, -(velocity.y * dt + accelretion.y * pow(dt, 2) / 2));
        window.setView(view);


        std::cout << "thrust " << total_force.x << " " << total_force.y << std::endl;
        std::cout << "accel " << accelretion.x << " " << accelretion.y << std::endl;
        std::cout << "vel " << velocity.x << " " << velocity.y << std::endl;
        std::cout << angle << " " << angle_velocity << " " <<angle_acceleration << std::endl  <<std::endl;

        //showThrust(thrust_len, playerTextThrust);
        
        ImGui::Begin("Info");
        ImGui::Text("asdfasdf");
        ImGui::InputFloat("thrust", &thrust_len);
        ImGui::End();

        window.clear(sf::Color::Black);
        //window.draw(playerTextThrust);
        window.draw(sprite);
        ImGui::SFML::Render(window);
        window.display();
    }
    ImGui::SFML::Shutdown();
    return 0;
}

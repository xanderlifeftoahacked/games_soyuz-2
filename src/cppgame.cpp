// первая строка кода - 15 ноября 2022 года, великое событие, отсюда берет свое начало легендарный проект, 
// зародившийся в умах двух школьников школы "Интеллектуал", существовавшей в 2022 году

#include "SFML/Graphics.hpp"
#include <string>
#include <iostream>
#include <cmath>
#include <gsl/gsl_linalg.h>

#define DEBUG true

#define WINDOWS_W 800
#define WINDOWS_H 600
#define RECTX 50
#define RECTY 126
#define MAX_THRUST 400

int deltaThrust = 1;

std::string resPath(std::string p) { return std::string(RESOURSE_PATH + p); }

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

int main()
{
    sf::ContextSettings settings;
    //settings.antialiasingLevel = 2;
    sf::RenderWindow window(sf::VideoMode(WINDOWS_W, WINDOWS_H), "Simulation", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);


    // Спрайт ракет
    sf::Texture texture;
    texture.loadFromFile(resPath("sprites/RocketV01.png"));
    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0, 0, RECTX, RECTY));
    sprite.setOrigin(RECTX/2, RECTY/2);
    sprite.scale(sf::Vector2f(0.5, 0.5));
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
    sf::Event event;
    
    unsigned int rotationFrame = 0;

    while (window.isOpen())
    {
        // Declare and load a texture

        // Draw it
        while (window.pollEvent(event))
        {
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

        float dt = deltaClock.restart().asSeconds();

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
        view.move(velocity.x * dt + accelretion.x * pow(dt, 2) / 2, -(velocity.y * dt + accelretion.y * pow(dt, 2) / 2));
        window.setView(view);


        std::cout << "thrust " << total_force.x << " " << total_force.y << std::endl;
        std::cout << "accel " << accelretion.x << " " << accelretion.y << std::endl;
        std::cout << "vel " << velocity.x << " " << velocity.y << std::endl;
        std::cout << angle << " " << angle_velocity << " " <<angle_acceleration << std::endl  <<std::endl;



        window.clear(sf::Color::Black);
        window.draw(sprite);
        window.display();
    }
    return 0;
}


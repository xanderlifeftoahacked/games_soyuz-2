#pragma once
#include "SFML/Graphics.hpp"
#include "planet.h"
#include "rocket.h"

const float RECTX =  50.0f;
const float RECTY = 126.0f;
const real MAX_THRUST = 4000000.0f;
const real DELTA_THRUST = 10000.0f;
const real THRUST_DEVIATION = PI * 0.1f;

// Центр координат - центр Земли

class World
{
    Rocket rocket;
    Planet earth;
    sf::RenderWindow *window;
    sf::View view;
public:
    World(sf::RenderWindow *_window)
        : rocket(100.f, 30.f, 1e5, MAX_THRUST, THRUST_DEVIATION), earth(6400000.f, 10e24), window{_window}
    {
        rocket.setTexture("sprites/RocketV01.png", RECTX, RECTY);
        rocket.setPosition(0, earth.radius + rocket.height * 0.5f);
    
        earth.setTexture("sprites/earth.png");
        earth.setPosition(0, 0);

        // Устанавливаем камеру на ракету
        view.reset(sf::FloatRect(-(float)(window->getSize().x) * 0.5f, -earth.radius - (float)(window->getSize().y) * 0.5f, window->getSize().x, window->getSize().y));
        window->setView(view);

    }

    // Обработка нажатий клавиатуры
    void processKeyboard()
    {
        
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            std::cout << rocket.thrust_magnitude << "\n";
            rocket.setThrustMagnitude(rocket.thrust_magnitude + DELTA_THRUST);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            rocket.setThrustMagnitude(rocket.thrust_magnitude - DELTA_THRUST);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            rocket.thrustRotateLeft(); rocket.setRotationFrame(2);
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            rocket.thrustRotateRight(); rocket.setRotationFrame(3);
        }
        else
        { 
            rocket.thrustSetCenter();
            rocket.setRotationFrame(1);
            // // Имитация гиродина
            // if(abs(angle_velocity) < 1) angle_velocity = 0;
            // else 
            // {
            //     if(angle_velocity > 0)
            //     {
            //         angle_velocity -= angle_acceleration * dt;
            //         angle += angle_velocity * dt - angle_acceleration * pow(dt, 2) / 2;
            //     }
            //     else
            //     {                    angle_velocity += angle_acceleration * dt;
            //         angle += angle_velocity * dt + angle_acceleration * pow(dt, 2) / 2;
            //     }
            // }
        }
    }

    void integrateVelocities(real dt)
    {
        rocket.position += rocket.velocity * dt;
        rocket.rotation += rocket.angular_velocity * dt;
        rocket.sprite.setRotation(rocket.rotation);
        rocket.sprite.setPosition(rocket.position.x, -rocket.position.y);
    }

    void integrateForces(real dt)
    {
        std::cout << "pos "; rocket.position.print();
        std::cout << "vel "; rocket.velocity.print(); 
        std::cout << "ang_vel " << rocket.angular_velocity << "\n"; 
        std::cout << "grav_accel "; earth.getGravityForce(rocket.position, G_EARTH_SURFACE, rocket.inv_mass).print();
        std::cout << "thrust "; rocket.thrust.print();
        std::cout << "thrust_mag " << rocket.thrust_magnitude << "\n";
        std::cout << std::endl;
        rocket.velocity += (earth.getGravityForce(rocket.position, G_EARTH_SURFACE, rocket.mass) + rocket.thrust + rocket.applied_force) * rocket.inv_mass * dt;
        rocket.angular_velocity += rocket.thrust.x * rocket.height * 0.5f * rocket.inv_intertia_moment * dt; // поворот вектором тяги

    }

    void step(real dt)
    {
        //Manifold m(&earth, &rocket);
        //if(OOBBvsCircle(&m)) m.applyImpulse();
        integrateForces(dt);
        integrateVelocities(dt);
        //m.positionalCorrection();
        view.setCenter(rocket.position.x, -rocket.position.y);
        window->setView(view);
    }

    void render()
    {
        rocket.animate();
        rocket.draw(window);
        earth.draw(window);
    }
};
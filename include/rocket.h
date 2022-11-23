#pragma once
#include "physics.h"
#include <string>
#include "SFML/Graphics.hpp"
#include "utils.h"

class Rocket : public OOBB
{
    float rectx, recty; // размеры спрайта ракеты в пикселах

    uint32 rotationFrame = 1;
    uint32 burnTypeCounter = 0;

public:
    sf::Texture texture;
    sf::Sprite sprite;

    const real max_thrust_magnitude;    // максимальная длина вектора тяги
    Vec2d thrust;                       // ветор тяги, в системе координат ракеты
    real thrust_magnitude = 0;          // длина вектора тяги
    real thrust_angle = 0;              // угол, под которым наклонен вектор тяги [радианы]
    real thrust_angle_deviation;        // половина угла между крайними положениями вектора тяги [радианы]
    Mat22 thrust_rotate_right;
    Mat22 thrust_rotate_left;

    real gyrodine_tangent_thrust; // тяга, создаваемая гиродином (полностью идет во вращение)


    /// \brief задает основные параметры ракеты
    /// \param _thrust_angle_deviation половина угла между крайними положениями вектора тяги [радианы]
    /// \param _gyrodine_tangent_thrust ветор тяги, создаваемая гиродином
    /// \param _gyrodine_dist_to_mc вектор от ЦМ до гиродина (гиродины будут находиться симметрично относитльно оси OY ракеты)
    Rocket(real _height, real _width, real _mass,  real _max_thrust, real _thrust_angle_deviation, Vec2d _gyrodine_thrust, Vec2d _gyrodine_to_mc)
        : OOBB(_width, _height, _mass), max_thrust_magnitude{_max_thrust}, thrust_angle_deviation{_thrust_angle_deviation},
            thrust_rotate_right(-2 * PI - _thrust_angle_deviation), thrust_rotate_left(_thrust_angle_deviation), 
            gyrodine_tangent_thrust{std::abs(cross_product(_gyrodine_to_mc, _gyrodine_thrust))}
    {
    }


    // Установка новой длины вектора тяги (меньше max_thrust_magnitude)
    void setThrustMagnitude(real new_thrust_magnitude) 
    { 
        if((new_thrust_magnitude < max_thrust_magnitude) && (new_thrust_magnitude >= 0))
        {
            thrust_magnitude = new_thrust_magnitude;
            Vec2d new_thrust(0, new_thrust_magnitude);
            if (thrust_angle < 0.f) thrust = thrust_rotate_right.rotate_vector(new_thrust);
            if (thrust_angle > 0.f) thrust = thrust_rotate_left.rotate_vector(new_thrust);
            else thrust = new_thrust;
        }
    }

    void thrustRotateRight() 
        { if(thrust_angle >= 0.f) { thrust = thrust_rotate_right.rotate_vector(thrust); thrust_angle = -thrust_angle_deviation; } }
    void thrustRotateLeft() 
        { if(thrust_angle <= 0.f) { thrust = thrust_rotate_left.rotate_vector(thrust); thrust_angle = thrust_angle_deviation; } }

    void thrustSetCenter() { thrust = Vec2d(0, thrust_magnitude); thrust_angle = 0; }

    void setPosition(real x, real y)
    {
        sprite.setPosition(x, -y);
        position.set(x, y);
    }

    int selectThrustFrame()
    {
        if(thrust_magnitude == 0) return 0;
        else if(thrust_magnitude > 0 && thrust_magnitude <= max_thrust_magnitude / 4) return 1;
        else if(thrust_magnitude > max_thrust_magnitude / 4 && thrust_magnitude <= max_thrust_magnitude / 2) return 2;
        else if(thrust_magnitude <= max_thrust_magnitude) return 3;
    }
    

    void animate()
    {
        int burnType = 0;
        if(burnTypeCounter % 10 >= 5) burnType = 1;
        else if(burnTypeCounter % 10 < 5) burnType = 0;
        else if(burnTypeCounter == 10) burnTypeCounter = 0;
        int oleg = rectx * selectThrustFrame() * 3 + rectx * (rotationFrame - 1);
        sprite.setTextureRect(sf::IntRect(oleg, burnType == 1 ? recty : 0, rectx, recty));
        //sprite.setScale(width / rectx, height / recty);
        burnTypeCounter++;
    }

    void setRotationFrame(int rf) { rotationFrame = rf; }

    void setTexture(std::string path, float _rectx, float _recty)
    {
        rectx = _rectx; recty = _recty;
        texture.loadFromFile(resPath(path));
        sprite.setTexture(texture);
        sprite.setTextureRect(sf::IntRect(0, 0, rectx, recty));
        //sprite.setScale(width / rectx, height / recty);
        sprite.setOrigin(width * 0.5f, height * 0.5f);
    }


    void draw(sf::RenderWindow* window) { window->draw(sprite); }
};
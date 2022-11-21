#pragma once
#include "rocket_math.h"
#include <vector>

#define INERTIA_COUNT_PRECISION 100

class Body
{
public:
    Vec2d velocity;
    Vec2d position;
    
    Vec2d applied_force;

    real mass;
    real inv_mass;

    real restitution; // коэффициент "восстановления"
    real staticFriction;
    real dynamicFriction;
    
    real inertia_moment;
    real angular_velocity;

    Body(real _mass)
        : mass{_mass}, inv_mass{1.0f / _mass}
    {
    }
    // virtual void draw();
};


/*
OOBB - object oriented bounding box
Задается векторами (половина ширины и половина высоты)
*/
class OOBB : public Body
{
public:
    real rotation; //радианы
    Vec2d h_width;
    Vec2d h_height;
    const uint32 inertia_precision;


    OOBB(real _width, real _height, real _mass, uint32 _inertia_precision = 100)
        : Body(_mass), inertia_precision { _inertia_precision }
    {
        position.set(0, 0);
        h_height.set(0, _height * 0.5f);
        h_width.set(_width * 0.5f, 0);
        count_intertia();
    }

    void count_intertia()
    {
        real d_mass = mass / (inertia_precision * inertia_precision);
        Vec2d n1 = h_width * (1.0f / inertia_precision), n2 = h_height * (1.0f / inertia_precision);

        for(uint32 i = 1; i <= inertia_precision; i++)
            for(uint32 j = 1; j <= inertia_precision; j++)
                inertia_moment += (n1 * i + n2 * j).lenSquare() * d_mass;
        inertia_moment *= 4;
    }

};


class Circle : public Body
{
public:
    real radius;
    Circle(real _radius, real _mass)
        : Body{_mass}
    {
        radius = _radius;
        inertia_moment = radius * radius * mass;
    }
};
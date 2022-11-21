#pragma once
#include "rocket_math.h"
#include <vector>

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
    real inv_intertia_moment;
    real angular_velocity;

    void applyImpulse(Vec2d& impulse, Vec2d& contactVector)
    {
        velocity += impulse * inv_mass;
        angular_velocity += inv_intertia_moment * cross_product(contactVector, impulse );
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
    std::vector<Vec2d> verticies; // набор точек, принадлежащих OOBB, вершины + по три точки на ребро


    OOBB(real _width, real _height, real _mass, uint32 _inertia_precision = 100)
        : inertia_precision { _inertia_precision }
    {
        mass = _mass; inv_mass = 1.0f / _mass;
        
        h_height.set(0, _height * 0.5f);
        h_width.set(_width * 0.5f, 0);
        position.set(0, 0);
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
        inv_intertia_moment = 1.0f / inertia_moment;
    }

};


class Circle : public Body
{
public:
    real radius;
    Circle(real _radius, real _mass)
    {
        mass = _mass; inv_mass = 1.0f / _mass;
        radius = _radius;
        inertia_moment = mass * radius * radius; inv_intertia_moment = 1.0f / inertia_moment; 
    }
};
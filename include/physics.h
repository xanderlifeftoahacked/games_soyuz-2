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
        angular_velocity += inv_intertia_moment * cross_product(contactVector, impulse);
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
    Vec2d h_width, h_height;
    real height, width;
    const uint32 inertia_precision;

    real applied_force_moment; 


    OOBB(real _width, real _height, real _mass, uint32 _inertia_precision = 100)
        : inertia_precision { _inertia_precision }, height{_height}, width{_width}
    {
        mass = _mass; inv_mass = 1.0f / _mass;
        
        h_height.set(0, _height * 0.5f);
        h_width.set(_width * 0.5f, 0);
        position.set(0, 0);
        count_intertia();
    }

    void count_intertia();

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


struct Manifold
{
  Circle *circle;
  OOBB *oobb;
  float penetration;
  Vec2d normal;
  Vec2d contact_point;

  real restitution;       // Смешанный коэффициент восстановления
  real dynamic_friction;  // Смешанный коэффициент динамического трения
  real static_friction;   // Смешанный коэффициент статического трения

  Manifold(Circle *_circle, OOBB *_oobb)
    : circle{_circle}, oobb{_oobb}
  {
    // Calculate average restitution
    restitution = std::min( circle->restitution, oobb->restitution );

    // Calculate static and dynamic friction
    static_friction = std::sqrt( circle->staticFriction * oobb->staticFriction );
    dynamic_friction = std::sqrt( circle->dynamicFriction * oobb->dynamicFriction );
  }

  void applyImpulse();
  void positionalCorrection();
};

bool OOBBvsCircle(Manifold *m);
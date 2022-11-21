#pragma once
#include "rocket_math.h"
#include "body.h"

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

// Разрешить коллизию для oobb относительно circle
void Manifold::applyImpulse()
{
  // Calculate radii from COM to contact
  Vec2d ra = contact_point - circle->position;
  Vec2d rb = contact_point - oobb->position;

  // Relative velocity
  Vec2d rv = oobb->velocity + cross_product( oobb->angular_velocity, rb) - circle->velocity - cross_product( circle->angular_velocity, ra );

  // Relative velocity along the normal
  real contactVel = dot_product( rv, normal );

  // Do not resolve if velocities are separating
  if(contactVel > 0)
    return;

  real raCrossN = cross_product( ra, normal );
  real rbCrossN = cross_product( rb, normal );
  real invMassSum = circle->inv_mass + oobb->inv_mass + std::sqrt( raCrossN ) * circle->inv_intertia_moment + std::sqrt( rbCrossN ) * oobb->inv_intertia_moment;

  // Calculate impulse scalar
  real j = -(1.0f + restitution) * contactVel;
  j /= invMassSum;

  // Apply impulse
  Vec2d impulse = normal * j;
  oobb->applyImpulse(impulse, rb );

  // Friction impulse
  rv = oobb->velocity + cross_product( oobb->angular_velocity, rb ) - circle->velocity - cross_product( circle->angular_velocity, ra );

  Vec2d t = rv - (normal * dot_product(rv, normal));
  t = t.normalized();

  // j tangent magnitude
  real jt = -dot_product( rv, t );
  jt /= invMassSum;

  // Don't apply tiny friction impulses
  if(is_equal_with_precision_up_to_epsilon( jt, 0.0f ))
    return;

  // Coulumb's law
  Vec2d tangentImpulse;
  if(std::abs( jt ) < j * static_friction)
    tangentImpulse = t * jt;
  else
    tangentImpulse = t * -j * dynamic_friction;

  // Apply friction impulse
  oobb->applyImpulse(tangentImpulse, rb);
}


// Коррекция позиции oobb относительно circle
void Manifold::positionalCorrection()
{
  const float percent = 0.2; // обычно от 20% до 80%
  const float k_slop = 0.01; // обычно от 0.01 до 0.1
  Vec2d correction = normal * ((std::max( penetration - k_slop, 0.0f ) / (circle->inv_mass + oobb->inv_mass)) * percent);
  oobb->position += correction * circle->inv_mass;
}
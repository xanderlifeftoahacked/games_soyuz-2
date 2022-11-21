#pragma once
#include "rocket_math.h"
#include "body.h"

struct Manifold
{
  Body *A;
  Body *B;
  float penetration;
  Vec2d normal;

  void PositionalCorrection();
};

// Коррекция позиции тела A относительно тела B
void Manifold::PositionalCorrection()
{
  const float percent = 0.2; // обычно от 20% до 80%
  const float k_slop = 0.01; // обычно от 0.01 до 0.1
  Vec2d correction = normal * ((std::max( penetration - k_slop, 0.0f ) / (A->inv_mass + B->inv_mass)) * percent);
  A->position -= correction * A->inv_mass;
}
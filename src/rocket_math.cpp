#include "rocket_math.h"

real distance(Vec2d &v1, Vec2d &v2) { return std::sqrt(std::pow(v1.x + v2.x, 2) + std::pow(v1.y + v2.y, 2)); }

real dot_product(Vec2d v1, Vec2d v2) { return v1.x * v2.x + v1.y * v2.y; }

real cross_product(const Vec2d &v1, const Vec2d &v2) { return v1.x * v2.y - v1.y * v2.x; }

Vec2d cross_product( const Vec2d& a, float s ) { return Vec2d( s * a.y, -s * a.x ); }

Vec2d cross_product( float s, const Vec2d& a ) { return Vec2d( -s * a.y, s * a.x ); }

Vec2d max(Vec2d &v1, Vec2d &v2) { return Vec2d(std::max(v1.x, v2.x), std::max(v1.y, v2.y)); }

real clamp(real min, real max, real a)
{
  if (a < min) return min;
  if (a > max) return max;
  return a;
}

real degree_to_radians(real deg) { return deg * PI / 180.f; }

bool is_equal_with_precision_up_to_epsilon(real a, real b) { return std::abs( a - b ) <= EPSILON; }

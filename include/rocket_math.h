#include <cmath>

typedef double real;

class Vec2d
{
public:
    real x;
    real y;

    Vec2d(real _x, real _y) { x = _x; y = _y; }

    Vec2d() { x = y = 0; }

    Vec2d operator+(Vec2d v2)
    {
        Vec2d v_temp;
        v_temp.x = x + v2.x;
        v_temp.y = y + v2.y;
        return v_temp;
    }
    Vec2d operator-(Vec2d v2)
    {
        Vec2d v_temp;
        v_temp.x = x - v2.x;
        v_temp.y = y - v2.y;
        return v_temp;
    }

    Vec2d operator=(Vec2d v2) { x = v2.x; y = v2.y; return *this; }
    Vec2d operator-() { x = -x; y = -y; return *this; }

    real len() const { return std::sqrt( x * x + y * y ); }
    real lenSquare() const { return x * x + y * y; }
    
};

real dotProduct(Vec2d &v1, Vec2d &v2) { v1.x * v2.x + v1.y * v2.y; }
real distance(Vec2d &v1, Vec2d &v2) { return std::sqrt(std::pow(v1.x + v2.x, 2) + std::pow(v1.y + v2.y, 2)); }


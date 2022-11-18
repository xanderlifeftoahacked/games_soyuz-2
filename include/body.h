#include "rocket_math.h"

enum Type
{
    Circle,
    OBB     //oriented bounding box
};

struct Body
{
    Vec2d velocity;
    Vec2d position;
    
    real angularVelocity;   // угловая скорость
    real torque;    // момент силы
    real angle; // в радианах
    
    real intertiaMoment; // момент инерции
    real mass; // масса
};
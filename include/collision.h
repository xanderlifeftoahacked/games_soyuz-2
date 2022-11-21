#pragma once
#include "body.h"
#include "rocket_math.h"

bool OOBBvsCircle(OOBB &oobb, Circle &circle)
{
    // Вектор из центра oobb в центр окружности
    Vec2d rp = circle.position - oobb.position;

    // Переводим rp из глобальной системы координат в систему координат oobb
    Mat22 mat_rotaion(oobb.rotation);
    rp = mat_rotaion.rotate_vector(rp);

    real l = rp.len() - circle.radius;
    if((std::abs(dot_product(max(oobb.h_height, oobb.h_width), rp.normalized())) >= l)) return true;
    return false;
}
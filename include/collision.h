#pragma once
#include "body.h"
#include "rocket_math.h"
#include "manifold.h"

bool OOBBvsCircle(Manifold *m)
{
    OOBB oobb = *m->oobb;
    Circle circle = *m->circle;
    // Вектор из центра oobb в центр окружности
    Vec2d rp = circle.position - oobb.position;

    // Переводим rp из глобальной системы координат в систему координат oobb
    Mat22 mat_rotaion(oobb.rotation);
    rp = mat_rotaion.rotate_vector(rp);

    // Ближайшая к центру B точка A
    Vec2d closest = rp;

    real h_width = oobb.h_width.len();
    real h_height = oobb.h_height.len();

    // Ограничиваем точку ребром AABB
    closest.x = clamp(-h_width, h_width, closest.x);
    closest.y = clamp(-h_height, h_height, closest.y);

    bool inside = false;

    // Окружность внутри AABB, поэтому нам нужно ограничить центр окружности
    // до ближайшего ребра
    if(rp == closest)
    {
        inside = true;

        // Находим ближайшую ось
        if(abs(rp.x) > abs(rp.y))
        {
        // Отсекаем до ближайшей ширины
        if(closest.x > 0)
            closest.x = h_width;
        else
            closest.x = -h_width;
        }

        // ось y короче
        else
        {
        // Отсекаем до ближайшей ширины
        if(closest.y > 0)
            closest.y = h_height;
        else
            closest.y = -h_height;
        }
    }

    Vec2d normal = rp - closest;

    // Если радиус меньше, чем расстояние до ближайшей точки и
    // Окружность не находится внутри AABB
    if((normal.lenSquare() > circle.radius * circle.radius) && !inside) return false;


    // Если окружность была внутри AABB, то нормаль коллизии нужно отобразить
    // в точку снаружи
    if(inside) m->normal = -rp; 
    else m->normal = rp;

    m->penetration = circle.radius - normal.len();
    m->contact_point = closest;

    return true;
}
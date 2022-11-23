#include "physics.h"


void OOBB::count_intertia()
{
    real d_mass = mass / (inertia_precision * inertia_precision);
    Vec2d n1 = h_width * (1.0f / inertia_precision), n2 = h_height * (1.0f / inertia_precision);

    for(uint32 i = 1; i <= inertia_precision; i++)
        for(uint32 j = 1; j <= inertia_precision; j++)
            inertia_moment += (n1 * i + n2 * j).lenSquare() * d_mass;
    inertia_moment *= 4;
    inv_intertia_moment = 1.0f / inertia_moment;
}



/// \brief Разрешить коллизию для oobb относительно circle
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


/// \brief Коррекция позиции oobb относительно circle
void Manifold::positionalCorrection()
{
  const float percent = 0.2; // обычно от 20% до 80%
  const float k_slop = 0.01; // обычно от 0.01 до 0.1
  Vec2d correction = normal * ((std::max( penetration - k_slop, 0.0f ) / (circle->inv_mass + oobb->inv_mass)) * percent);
  oobb->position += correction * circle->inv_mass;
}



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
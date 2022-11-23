#pragma once
#include <cmath>
#include <iostream>

typedef double real;
typedef u_int64_t uint32;

const real PI = 3.14159265358979323f;
const real EPSILON = 1e-3;
const real G_EARTH_SURFACE = 9.81f;

real degree_to_radians(real deg);

struct Vec2d
{
    real x;
    real y;

    Vec2d(real _x, real _y) { x = _x; y = _y; }

    Vec2d() { x = y = 0; }

    void set(real _x, real _y) { x = _x; y = _y; }

    Vec2d operator+(Vec2d v2)
    {
        Vec2d v_temp;
        v_temp.x = x + v2.x;
        v_temp.y = y + v2.y;
        return v_temp;
    }
    void operator+=(Vec2d v2) { x += v2.x; y += v2.y; }

    Vec2d operator-(Vec2d v2)
    {
        Vec2d v_temp;
        v_temp.x = x - v2.x;
        v_temp.y = y - v2.y;
        return v_temp;
    }
    void operator-=(Vec2d v2) { x -= v2.x; y -= v2.y; }

    bool operator==(Vec2d v2) { return (x == v2.x) && (y == v2.y); }

    // Умножение вектора на число
    Vec2d operator*(real a)
    {
        Vec2d v_temp;
        v_temp.x = x * a;
        v_temp.y = y * a;
        return v_temp;
    }
    // Умножение вектора на число
    void operator*=(real a) { x *= a; y *= a; }

    Vec2d operator=(Vec2d v2) { x = v2.x; y = v2.y; return *this; }
    Vec2d operator-() const { return Vec2d(-x, -y); }



    real len() const { return std::sqrt( x * x + y * y ); }
    real lenSquare() const { return x * x + y * y; }

    Vec2d normalized() { return *this * (1 / this->len()); }

    void print() { std::cout << x << " " << y << std::endl; }
};

real distance(Vec2d &v1, Vec2d &v2);

// Скалярное произведение
real dot_product(Vec2d v1, Vec2d v2);

// Векторное произведение; возвращает длину вектора, который по сути параллелен оси z
real cross_product(const Vec2d &v1, const Vec2d &v2);

// Более экзотичные (но необходимые) виды векторных произведений
// с вектором a и скаляром s, оба возвращают вектор
Vec2d cross_product(const Vec2d& a, float s);

Vec2d cross_product(float s, const Vec2d& a);

Vec2d max(Vec2d &v1, Vec2d &v2);

bool is_equal_with_precision_up_to_epsilon(real a, real b);

// Ограничение числа от min до max
// a < min ? min : a или a > max ? max : a
real clamp(real min, real max, real a);

// Ортогональная матрица трансформации
struct Mat22
{
    real m00, m01;
    real m10, m11;
    Mat22(real angle) { set_angle(angle); }
    Mat22(real _m00, real _m01, real _m10, real _m11) { m00 = _m00; m01 = _m01; m10 = _m10; m11 = _m11; } 

    void set_angle(real radians)
    {
        real _cos = std::cos(radians), _sin = std::sin(radians);
        m00 = _cos; m01 = -_sin;
        m10 = _sin; m11 = _cos;
    }

    // получение транспонированной матрицы
    Mat22 transpose() { return Mat22(m00, m10, m01, m11); }

    // поворот вектора при помощи текущей матрицы трансформации
    Vec2d rotate_vector(Vec2d &v) { return Vec2d(m00 * v.x + m01 * v.y, m10 * v.x + m11 * v.y); }
    
};



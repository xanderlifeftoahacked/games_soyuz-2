class Vector2d
{
public:
    double x;
    double y;

    Vector2d(double _x, double _y) { x = _x; y = _y; }

    Vector2d() { x = y = 0; }

    Vector2d operator+(Vector2d v2)
    {
        Vector2d v_temp;
        v_temp.x = x + v2.x;
        v_temp.y = y + v2.y;

        return v_temp;
    }

    Vector2d operator=(Vector2d v2)
    {
        x = v2.x; y = v2.y;
        return *this;
    }
};
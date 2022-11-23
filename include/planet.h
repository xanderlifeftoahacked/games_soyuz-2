#pragma once
#include "physics.h"
#include "utils.h"
#include "SFML/Graphics.hpp"

class Planet : public Circle
{
    sf::Texture texture;
    sf::Image image;
    sf::CircleShape shape;
public:
    Planet(real _radius, real _mass)
        : Circle(_radius, _mass)
    {
    }

    void setTexture(std::string path, uint32 point_count = 2000)
    {
        image.loadFromFile(resPath(path));  
        texture.loadFromImage(image);  
        shape.setRadius(radius);
        shape.setPointCount(point_count);
        shape.setTexture(&texture);
        shape.setOrigin(radius, radius);
    }


    void setPosition(real x, real y)
    {
        shape.setPosition(x, -y);
        position.set(x, y);
    }

    Vec2d getGravityForce(Vec2d obj_position, real acceleration_surface, real obj_mass) 
        { return (position- obj_position).normalized() * (acceleration_surface * radius * radius * obj_mass / obj_position.lenSquare()); }

    void draw(sf::RenderWindow* window) { window->draw(shape); }
};
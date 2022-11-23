#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "utils.h"

class Message
{
    sf::Text msg;
    bool isNeedToBeDrawed = true;
    public:

        Message(std::string _msg, sf::Vector2f pos, sf::Color col, sf::Font& font)
        { 
            msg.setString(_msg);
            msg.setFont(font);
            msg.setOrigin(msg.getLocalBounds().width / 2, msg.getLocalBounds().height / 2);
            msg.setPosition(pos);
            msg.setColor(col);
        }
        void changeAlpha(int d_alpha)
        {
            sf::Color col = msg.getColor();
            int alpha = col.a;
            alpha -= d_alpha;
            if(alpha <= 0) isNeedToBeDrawed = false;
            else { col.a = alpha; msg.setColor(col); }
        }
        void draw(sf::RenderWindow &window) { if(isNeedToBeDrawed) window.draw(msg); }
};
#include "SFML/Graphics.hpp"

#define WINDOWS_W 800
#define WINDOWS_H 600


int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;
    sf::RenderWindow window(sf::VideoMode(WINDOWS_W, WINDOWS_H), "Simulation", sf::Style::Default, settings);
    //window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);

    sf::View view;
    sf::Clock deltaClock;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::Resized) {
                // resize view
                view = sf::View(sf::FloatRect(0.f, 0.f, event.size.width, event.size.height));
                window.setView(view);
            }
        }
        //timer_millisec(10);

        window.clear(sf::Color::White);
        window.display();
    }


    return 0;
}
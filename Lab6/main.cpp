#include <iostream>
#include <SFML/Graphics.hpp>

#include "3D/Rectangle3D.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600, 32), "SFML");

    sf::Clock Clock;

    const float speed = 100000;

    Camera3D camera(100);
    Light3D light({0, 0, 0}, {1, 1, 1});
    Scene3D scene(camera, light);
    Rectangle3D rectangle(scene,
                          {255, 0, 0},
                          100, 100,
                          {400, 300, 200});

    while (window.isOpen()) {
        sf::Event Event{};
        while (window.pollEvent(Event)) {
            if (Event.type == sf::Event::Closed)
                window.close();

            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape))
                window.close();

            if (Event.type == sf::Event::KeyPressed && Event.key.code == sf::Keyboard::Right) {
                rectangle.rotate({0, 0, speed / CLOCKS_PER_SEC});
            }
            if (Event.type == sf::Event::KeyPressed && Event.key.code == sf::Keyboard::Left) {
                rectangle.rotate({0, 0, -speed / CLOCKS_PER_SEC});
            }

            if (Event.type == sf::Event::KeyPressed && Event.key.code == sf::Keyboard::Up) {
                rectangle.rotate({speed / CLOCKS_PER_SEC, 0, 0});
            }
            if (Event.type == sf::Event::KeyPressed && Event.key.code == sf::Keyboard::Down) {
                rectangle.rotate({-speed / CLOCKS_PER_SEC, 0, 0});
            }
        }
        window.clear();


        window.draw(rectangle);


        window.display();

    }

    return 0;
}

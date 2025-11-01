#include <SFML/Graphics.hpp>
#include "Button.h"
#include <iostream>
#include <filesystem>

int main() {
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Button Example");

    sf::Texture bg;
    bg.loadFromFile("assets/background.png");
    sf::Sprite background(bg);

    Button loginButton("assets/LoginButton.png", { 340.f, 500.f }, []() {
        std::cout << "Login button clicked!\n";
        });

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            loginButton.handleEvent(event, window);
        }

        window.clear();
        window.draw(background);
        loginButton.draw(window);
        window.display();
    }

    return 0;
}

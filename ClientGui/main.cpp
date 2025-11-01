#include <SFML/Graphics.hpp>
#include "Button.h"
#include "TextBox.h"
#include <iostream>
#include <filesystem>

int main() {
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Button Example");

    sf::Texture bg;
    bg.loadFromFile("assets/background.png");
    sf::Sprite background(bg);

    Button loginButton("assets/LoginButton.png", { 300.f, 500.f }, []() {
        std::cout << "Login button clicked!\n";
        });

    Button registerButton("assets/RegisterButton.png", { 600.f, 500.f }, []() {
        std::cout << "Register button clicked!\n";
        });

    sf::Font font;
    font.loadFromFile("assets/ARIAL.TTF");
    TextBox usernameBox(font, { 300.f, 300.f }, { 500.f, 50.f });
    TextBox passwordBox(font, { 300.f, 400.f }, { 500.f, 50.f });

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            loginButton.handleEvent(event, window);
            registerButton.handleEvent(event, window);
            usernameBox.handleEvent(event);
            passwordBox.handleEvent(event);
           
        }

        window.clear();
        window.draw(background);
        loginButton.draw(window); 
        registerButton.draw(window);
        usernameBox.draw(window);
        passwordBox.draw(window);
        window.display();
    }

    return 0;
}

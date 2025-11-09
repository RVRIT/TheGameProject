#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <vector>

class MainMenu {
public:
    MainMenu(sf::Font& font, std::function<void()> onLogin, std::function<void()> onRegister, std::function<void()> onExit);

    void handleEvent(const sf::Event& event, sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

private:
    struct Button {
        sf::RectangleShape shape;
        sf::Text text;
        std::function<void()> onClick;
    };

    std::vector<Button> buttons;
};

#pragma once
#include <SFML/Graphics.hpp>
#include <functional>

class Button {
public:
    Button(const std::string& texturePath, sf::Vector2f position, std::function<void()> callback = nullptr);

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

private:
    sf::Sprite sprite;
    sf::Texture texture;
    std::function<void()> onClick;
    bool hovered = false;

};

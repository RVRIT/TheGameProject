#include <SFML/Graphics.hpp>
#include "button.h"
#include "textBox.h"
#include "loginMenu.h"
#include "registerMenu.h"
#include <iostream>
#include <filesystem>


enum class Scene {
    LoginMenu,
    Register
};

int main() {
    sf::RenderWindow window(sf::VideoMode(1280, 720), "The Game");

    sf::Font font;
    font.loadFromFile("assets/ARIAL.TTF");
    Scene currentScene = Scene::LoginMenu;

    NetworkClient client("http://localhost", 18080);

    LoginMenu loginMenu(font,client,[&]()
        {
            currentScene = Scene::Register;
        });


    RegisterMenu registerMenu(font,client, [&]() {
            currentScene = Scene::LoginMenu;
        });


    sf::Texture cardTest;
    cardTest.loadFromFile("assets/cardflip.png");
    sf::Sprite cardSprite;
    cardSprite.setTexture(cardTest);
    cardSprite.setScale(10.f, 10.f);
    cardSprite.setPosition({ 600.f, 100.f }); 
    const int frameWidth = 64;
    const int frameHeight = 64;
    const int frameCount = 16;
    int currentFrame = 0;
    sf::Clock clock;


    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (currentScene == Scene::LoginMenu)
                loginMenu.handleEvent(event, window);
            else if (currentScene == Scene::Register)
                registerMenu.handleEvent(event, window);
           
        }

        if (clock.getElapsedTime().asSeconds() > 0.15f) { 
            currentFrame = (currentFrame + 1) % frameCount;
            cardSprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 0, frameWidth, frameHeight));
            clock.restart();
        }

        window.clear();

   
        if (currentScene == Scene::LoginMenu) {
            loginMenu.draw(window);
        }
        else if (currentScene == Scene::Register) {
            registerMenu.draw(window);
            window.draw(cardSprite);
        }

        

     
        window.display();
    }

    return 0;
}

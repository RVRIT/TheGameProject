#include <SFML/Graphics.hpp>
#include "button.h"
#include "textBox.h"
#include "loginMenu.h"
#include "registerMenu.h"
#include "Animation.h"
#include "MainMenu.h"
#include <iostream>
#include <filesystem>


enum class Scene {
    LoginMenu,
    Register,
    MainMenu
};

int main() {
    sf::RenderWindow window(sf::VideoMode(1280, 720), "The Game");

    sf::Font font;
    font.loadFromFile("assets/ARIAL.TTF");
    Scene currentScene = Scene::LoginMenu;

    NetworkClient client("http://localhost", 18080);

    LoginMenu loginMenu(font, client, [&]() { currentScene = Scene::Register;},
        [&]() { currentScene = Scene::MainMenu;});


        RegisterMenu registerMenu(font, client, [&]() {
            currentScene = Scene::LoginMenu;
            });

    MainMenu mainMenu(window);

    Animation cardTest("assets/cardflip.png", 64, 64, 16, 12.f,true);
    cardTest.setPosition({ 600.f,100.f });
    cardTest.setScale(10.f, 10.f);


    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (currentScene == Scene::LoginMenu)
                loginMenu.handleEvent(event, window);
            else if (currentScene == Scene::Register)
                registerMenu.handleEvent(event, window);
            else if (currentScene == Scene::MainMenu)
                mainMenu.handleInput(event, window);
        }

        cardTest.update();

        window.clear();

        switch (currentScene) {
        case Scene::LoginMenu:
            loginMenu.draw(window);
            break;

        case Scene::Register:
            registerMenu.draw(window);
            window.draw(cardTest);
            break;

        case Scene::MainMenu:
            mainMenu.draw();
            break;
        }

        window.display();
    }

    return 0;
}

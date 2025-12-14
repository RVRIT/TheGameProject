#include <SFML/Graphics.hpp>
#include "button.h"
#include "textBox.h"
#include "loginMenu.h"
#include "registerMenu.h"
#include "Animation.h"
#include "MainMenu.h"
#include "SettingsMenu.h"
#include "SceneManager.h"
#include <iostream>
#include <filesystem>
#include "GameScene.h"
#include "LobbyScene.h"

int main() {

    sf::RenderWindow window(sf::VideoMode(1280, 720), "The Game");

    sf::Font font;
    font.loadFromFile("assets/PIXEL.TTF");
    NetworkClient client("http://localhost", 18080);
    SceneManager sceneManager(window);
    sceneManager.pushScene(std::make_unique<LobbyScene>(font, client, sceneManager));
    //sceneManager.pushScene(std::make_unique<LoginMenu>(font, client, sceneManager, window));
    //sceneManager.pushScene(std::make_unique<GameScene>(font, sceneManager, window));

    sf::Clock clock;

    while (window.isOpen()) {

        if (sceneManager.isEmpty()) {
            window.close();
        }

        sf::Time dt = clock.restart(); 

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            sceneManager.handleEvent(event);
        }

        sceneManager.update(dt);

        window.clear();

        sceneManager.draw();

        window.display();
    }

    return 0;
}

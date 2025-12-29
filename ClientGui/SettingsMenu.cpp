#include "SettingsMenu.h"

SettingsMenu::SettingsMenu(sf::RenderWindow& win, SceneManager& mgr, sf::Font& f)
    : window(win), sceneManager(mgr), font(f),
    backButton("assets/back.png", { 50.f, 50.f }, [&]() { mgr.popScene(); }),

    fullscreenButton("assets/fullscreen.png", { 100.f, 100.f }, [&]() {
    static bool isFullscreen = false;
    isFullscreen = !isFullscreen;

    if (isFullscreen) {
        window.create(sf::VideoMode::getDesktopMode(), "Card Game", sf::Style::Fullscreen);
    }
    else {
        window.create(sf::VideoMode(1280, 720), "Card Game", sf::Style::Close | sf::Style::Titlebar);
    }

    window.setFramerateLimit(60);
    window.setView(window.getDefaultView());
        }), 

    volumeButton("assets/volume.png", { 400.f, 450.f }, []() {})
{
    if (bgTexture.loadFromFile("assets/backgroundSettings.png")) {
        background.setTexture(bgTexture);
       // updateBackgroundScale(); 
    }

    titleText.setFont(font);
    titleText.setString("Settings");
    titleText.setPosition(500, 50);

    sliderBar.setSize(sf::Vector2f(300.f, 10.f));
    sliderBar.setFillColor(sf::Color(100, 100, 100)); 
    sliderBar.setPosition(400.f, 500.f); 

    sliderFill.setSize(sf::Vector2f(150.f, 10.f)); 
    sliderFill.setFillColor(sf::Color::Green);
    sliderFill.setPosition(400.f, 500.f);

    sliderHandle.setRadius(10.f);
    sliderHandle.setFillColor(sf::Color::White);
    sliderHandle.setOrigin(10.f, 5.f); 
    sliderHandle.setPosition(400.f + 150.f, 500.f); 

    volumeText.setString("Volume: 50%");

    window.setFramerateLimit(60);


}

void SettingsMenu::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    backButton.handleEvent(event, mousePos);
    fullscreenButton.handleEvent(event, mousePos);
    volumeButton.handleEvent(event, mousePos);

    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            if (sliderHandle.getGlobalBounds().contains(mousePos) ||
                sliderBar.getGlobalBounds().contains(mousePos)) {
                isDragging = true;
            }
        }
    }

    if (event.type == sf::Event::MouseButtonReleased) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            isDragging = false;
        }
    }

    if (event.type == sf::Event::MouseMoved && isDragging) {
        float barLeft = sliderBar.getPosition().x;
        float barRight = barLeft + sliderBar.getSize().x;

        float newX = mousePos.x;
        if (newX < barLeft) newX = barLeft;
        if (newX > barRight) newX = barRight;

        sliderHandle.setPosition(newX, sliderHandle.getPosition().y);

        float percentage = (newX - barLeft) / sliderBar.getSize().x;
        currentVolume = percentage * 100.f;

        sliderFill.setSize(sf::Vector2f(sliderBar.getSize().x * percentage, sliderBar.getSize().y));

        volumeText.setString("Volume: " + std::to_string(static_cast<int>(currentVolume)) + "%");
    }
}

void SettingsMenu::update(sf::Time dt) {}

void SettingsMenu::draw(sf::RenderWindow& window) {
    //window.draw(background);
    window.draw(titleText);
    backButton.draw(window);
    fullscreenButton.draw(window);
    volumeButton.draw(window);

    window.draw(volumeText);   
    window.draw(sliderBar);    
    window.draw(sliderFill);   
    window.draw(sliderHandle);
}
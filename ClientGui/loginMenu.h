#pragma once
#include "button.h"
#include "textBox.h"
#include "networkClient.h"
#include "Scene.h"
#include <functional>

class SceneManager;

class LoginMenu : public Scene
{
public:
	LoginMenu(sf::Font& font, NetworkClient& client, SceneManager& manager, sf::RenderWindow& window);

	virtual void handleEvent(const sf::Event& event, sf::RenderWindow& window) override;	
	virtual void update(sf::Time dt) override;
	virtual void draw(sf::RenderWindow& window) override;
private:
	sf::Font& font;
	SceneManager& sceneManager;
	sf::RenderWindow& window;
	NetworkClient& client;
	Button LoginButton;
	Button RegisterButton;
	TextBox username;
	TextBox password;
	sf::Texture bgTexture;
	sf::Sprite background;
	sf::Text errorText;
	
};


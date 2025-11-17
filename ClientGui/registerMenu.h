#pragma once
#include "button.h"
#include "textBox.h"
#include "networkClient.h"
#include "Scene.h"

class RegisterMenu : public Scene
{
public:
	RegisterMenu(sf::Font& font, NetworkClient& client, SceneManager& manager);

	virtual void handleEvent(const sf::Event& event, sf::RenderWindow& window) override;
	virtual void update(sf::Time dt) override;
	virtual void draw(sf::RenderWindow& window) override;

private:
	SceneManager& sceneManager;
	NetworkClient& client;
	Button CreateAccount;
	Button Back;
	TextBox username;
	TextBox password;
	sf::Texture bgTexture;
	sf::Sprite background;

};


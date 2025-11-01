#include <SFML/Graphics.hpp>
#include <iostream>


int main()
{
    sf::RenderWindow window(sf::VideoMode({ 1920, 1080}), "SFML Background Test");
    window.setFramerateLimit(60);

    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("assets/background.png"))
    {
        std::cerr << "Nu s-a putut incarca 'assets/background.png'\n";
        std::cerr << "Calea curenta: " << std::filesystem::current_path() << "\n";
        return -1;
    }

    const sf::Font font("assets/ARIAL.TTF");
    sf::Text text(font, "Hello SFML", 100);

    sf::Sprite background(backgroundTexture);


    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }
        window.clear();
        window.draw(background);
        window.draw(text);
        window.display();
    }

    std::cout << "Window closed successfully." << std::endl;
    return 0;
}

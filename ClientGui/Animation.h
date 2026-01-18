#pragma once
#include <SFML/Graphics.hpp>

class Animation : public sf::Drawable {
private:
    sf::Sprite sprite;
    sf::Texture texture;
    int frameWidth, frameHeight;
    int frameCount;
    int currentFrame = 0;
    int direction = 1;
    bool reversesOncePlayed = false;
    bool finished = false;
    /*
    TODO
    bool loop = true;
    bool pingPong = false;
    ca sa am optiunea sa aleg cum dau play la animatii, acum e cam blocat pe loop permanent ping pong
    */
    float frameTime;
    sf::Clock clock;
    
    void updateFrame();

public:
    Animation(const std::string& filePath,
        int frameW, int frameH,
        int frames,
        float fps,
        bool reverseOncePlayed = false);

    void update();
    bool isFinished() const;
    void setPosition(sf::Vector2f pos); 
    void setScale(float x, float y); 

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
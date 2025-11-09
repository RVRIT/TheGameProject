#include "Animation.h"
#include <iostream>

Animation::Animation(const std::string& filePath, int frameW, int frameH,
    int frames, float fps, bool reversesOncePlayed)
    : frameWidth(frameW),
    frameHeight(frameH),
    frameCount(frames),
    frameTime(1.f / fps),
    reversesOncePlayed(reversesOncePlayed)
{
    if (!texture.loadFromFile(filePath)) {
        std::cerr << "Failed to load texture: " << filePath << "\n";
    }

    texture.setSmooth(false);
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
}

void Animation::update() {
    if (finished) return;

    if (clock.getElapsedTime().asSeconds() >= frameTime) {
        currentFrame += direction;

        if (currentFrame >= frameCount - 1) {
            direction = -1; 
        }
        else if (currentFrame <= 0 && direction == -1) {
            direction = 1; 
        }

        updateFrame();
        clock.restart();
    }
}

void Animation::updateFrame() {
    sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 0, frameWidth, frameHeight));
}

void Animation::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(sprite, states);
}
#pragma once
#include <SFML/Graphics.hpp>
#include "lib.h"

class Platform {
public:
    Platform(float x, float y, float width, float height, sf::Texture* tex = nullptr);
    ~Platform() = default;

    void render(sf::RenderTarget& target) const;
    sf::FloatRect getGlobalBounds() const;
    sf::Vector2f getPosition() const;

private:
    bool hasTexture = false;
    sf::Sprite sprite;
    sf::RectangleShape rect; // fallback visual
    float w = 0.f, h = 0.f;
};

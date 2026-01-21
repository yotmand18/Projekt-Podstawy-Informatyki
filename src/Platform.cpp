#include "Platform.h"
#include <iostream>
#include <cmath>

Platform::Platform(float x, float y, float width, float height, sf::Texture* tex)
    : hasTexture(false), w(width), h(height)
{
    if (!tex) return; // no texture, nothing to draw

    sf::Vector2u ts = tex->getSize();
    if (ts.x == 0 || ts.y == 0) {
        std::cout << "[Platform] texture has zero size - skipping\n";
        return;
    }

    hasTexture = true;
    sprite.setTexture(*tex, true);
    sprite.setOrigin(0.f, 0.f);

    float sx = width / static_cast<float>(ts.x);
    float sy = height / static_cast<float>(ts.y);

    // Prevent degenerate scales
    if (!(sx > 0.f && sy > 0.f && std::isfinite(sx) && std::isfinite(sy))) {
        std::cout << "[Platform] invalid scale - skipping\n";
        hasTexture = false;
        return;
    }

    sprite.setScale(sx, sy);
    sprite.setPosition(x, y);
    sprite.setColor(sf::Color(255, 255, 255, 255)); // preserve alpha
}

void Platform::render(sf::RenderTarget& target) const {
    if (hasTexture) {
        sf::RenderStates states(sf::BlendAlpha);
        target.draw(sprite, states);
    }
}

sf::FloatRect Platform::getGlobalBounds() const {
    return hasTexture ? sprite.getGlobalBounds() : sf::FloatRect{};
}

sf::Vector2f Platform::getPosition() const {
    return hasTexture ? sprite.getPosition() : sf::Vector2f{};
}

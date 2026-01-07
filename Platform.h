#pragma once

#include "lib.h"

class Platform{
    private:
        // Core

        sf::Sprite sprite;

    public:
        // Constructors / Destructors

        Platform(float x, float y, float width, float height, sf::Texture* texture = nullptr);
        virtual ~Platform();

        // Accessors
        sf::FloatRect getBounds() const;

        // Functions

        void render(sf::RenderTarget& target);


};
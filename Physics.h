#pragma once

#include "lib.h"

class Physics{
    private:
        // Core

        sf::Vector2f velocity;
        sf::Sprite* sprite;
        float velocityMaxY;
        float gravity;
        bool canJump;
        bool applyGravity;

    public:
        // Constructors / Destructors

        Physics(sf::Vector2f currentVelocity, sf::Sprite* theSprite, bool applyGrv);
        virtual ~Physics();

        // Accessors

        inline sf::Vector2f getVelocity() const { return this->velocity; };
        inline bool getCanJump() const { return this->canJump; };
        
        // Modifiers

        inline void resetVelocityX() { this->velocity.x = 0.f; };
        inline void resetVelocityY() { this->velocity.y = 0.f; };
        inline void setCanJump(bool value) { this->canJump = value; };
        
        // Functions
        void move(const float dir_x, const float dir_y);
        void jump();

        void update();




};
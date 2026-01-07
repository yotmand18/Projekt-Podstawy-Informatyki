#pragma once

#include "lib.h"

enum ENEMY_TYPE = {DROWNER = 0, // slow, walking, patrol
                    NEKKER,     // fast, small, weak, walking, aggressive
                    GHOUL,      // strong, walking, aggressive
                    WRAITH,     // strong, hovering, patrol
                    GRIFFIN     // boss, flying, aggressive
                };

class Enemies{
    private:
        // Core

        sf::Sprite sprite;
        sf::FloatRect hitbox;
        sf::CLock animationClock;

        // Animations
        sf::IntRect currentFrame;

        // Stats

        ENEMY_TYPE enemyType;
        int health;
        int damage;
        float moveSpeed;
        bool movingRight;

        // Movement bounds
        float patrolLeft;
        float patrolRight;

        // Initialization
        void initSprite(sf::Texture* texture);
        void initStats();

    public:
        // Constructors / Destructors

        Enemies(float x, float y, ENEMY_TYPE enemyType, sf::Texture* texture);
        virtual ~Enemies();

        // Accesssors

        const sf::Vector2f getPosition() const;
        const sf::FloatRect getGlobalBounds() const;
        int getDamage() const;
        bool isAlive() const;

        // Modifiers
        void setPosition(float x, float y);
        void takeDamage(int ammount);

        // Functions
        void updateMovement();
        void updateAnimations();

        void update();

        void render(sf::RenderTarget& target);
 

};
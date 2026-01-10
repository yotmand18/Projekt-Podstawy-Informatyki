#pragma once

#include "lib.h"
#include "Physics.h"

enum ENEMY_TYPE {
    DROWNER = 0,
    NEKKER,
    GHOUL,
    WRAITH,
    GRIFFIN
};

class Enemy{
    private:
        sf::Sprite sprite;
        Physics* physics;
        sf::FloatRect hitbox;
        sf::IntRect enemyBoundaries;
        sf::Clock animationClock;
        sf::IntRect currentFrame;
        sf::Vector2f previousPosition;
        
        ENEMY_TYPE enemyType;
        int health;
        int damage;
        float moveSpeed;
        bool isFlying;
        bool movingRight;
        sf::Clock damageClock;
        
        float patrolLeft;
        float patrolRight;
        bool isAgressive;
        bool isChasing;
        float agroRange;
        sf::Vector2f playerPos;
        
        void initSprite(sf::Texture* texture);
        void initStats();

    public:
        Enemy(float x, float y, ENEMY_TYPE enemyType, sf::Texture* texture, float patrol_left, float patrol_right);
        virtual ~Enemy();

        // Accessors
        
        const sf::Vector2f getPosition() const;
        const sf::FloatRect getGlobalBounds() const;
        inline const sf::Vector2f getPrevPosition() const { return previousPosition; }

        int getDamage() const;
        bool isAlive() const;
        int getHealth() const;
        bool canChase() const;

        sf::Vector2f getVelocity() const;
        inline bool getCanJump() const { return this->physics->getCanJump(); };
    
        // Modifiers
    
        inline void resetVelocityY() { physics->resetVelocityY(); };
        inline void resetVelocityX() { physics->resetVelocityX(); };
        inline void setCanJump(bool value) { physics->setCanJump(value); };
        void setPosition(float x, float y);
        void takeDamage(int amount);

        // Functions

        void jump();
        void updatePlayerPosition(const sf::Vector2f& pos);
        void updateMovement();
        void updateAnimations();
        void update();
        void render(sf::RenderTarget& target);
};
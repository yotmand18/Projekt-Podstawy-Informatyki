#pragma once

#include "Physics.h"

enum PLAYER_ANIMATION_STATES{ IDLE = 0, MOVING_LEFT, MOVING_RIGHT, RUNNING_RIGHT, RUNNING_LEFT, ATTACKING };

class Player{
    private:
        // Core
        sf::Sprite sprite;
        Physics* physics;
        sf::Clock animationClock;
        sf::FloatRect hitbox; 
        short previousAnimState;
        sf::Clock damageCooldownClock;
        float damageCooldown;
        sf::Vector2f previousPosition;

        // Player stats
        int health;
        int maxHealth;
        int points;

        // Player flags
        bool lostHealth;
        bool gainedHealth;
        bool hasDealtDamage;
        bool facingRight;

        // Animations
        short animState;
        sf::IntRect currentFrame;
        bool canSwitchAnimation;
        bool animationForward;

        // Initialization
        void initAnimations();
        void initStats();

    public:
        // Constructors / Destructors
        Player(sf::Texture& texture_sheet);
        virtual ~Player();

        // Accessors
        bool getCanSwitchAnim();
        const sf::FloatRect getGlobalBounds() const;

        const sf::Vector2f getPosition() const { return sprite.getPosition(); };
        inline const sf::Vector2f getPrevPosition() const { return previousPosition; }
        inline sf::Vector2f getVelocity() const { return physics->getVelocity(); };

        sf::FloatRect getAttackBounds() const;

        inline bool isAttacking() const { return animState == ATTACKING; }
        inline bool hasAttackHit() const { return hasDealtDamage; }

        inline int getHealth() const { return health; }
        inline int getMaxHealth() const { return maxHealth; };
        inline int getPoints() const { return points; }
        bool isAlive() const { return (health > 0); };
        
        inline bool getCanJump() const { return physics->getCanJump(); }

        // Modifiers
        void setPosition(const float x, const float y);
        void setAnimState(short state);
        void takeDamage(int amount);
        void gainHealth(int amount);
        
        inline void addPoints(int amount) { points += amount; }
        inline void setAttackHit() { hasDealtDamage = true; }
        inline void setCanJump(bool value) { physics->setCanJump(value); }
        inline void resetVelocityX() { physics->resetVelocityX(); }
        inline void resetVelocityY() { physics->resetVelocityY(); }

        // Functions
        bool isInAttackHitFrame() const;
        void resetAnimationClock();
        void move(const float dir_x, const float dir_y);
        void jump();
        void updateMovement();
        void updateAnimations();\
        void update();
        void render(sf::RenderTarget& target);
};
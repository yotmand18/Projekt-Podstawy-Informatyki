#pragma once

#include "lib.h"

enum PLAYER_ANIMATION_STATES{ IDLE = 0, MOVING_LEFT, MOVING_RIGHT, RUNNING_RIGHT, RUNNING_LEFT, ATTACKING };

class Player{
    private:
        // Core

        sf::Sprite sprite;
        sf::Clock animationClock;
        sf::FloatRect hitbox; 
        short previousAnimState;

        // Player stats

        int health;
        int maxHealth;
        int points;

        // Player flags

        bool lostHealth;
        bool gainedHealth;

        // Animations

        short animState;
        sf::IntRect currentFrame;
        bool canSwitchAnimation;
        bool animationForward;

        // Physics

        sf::Vector2f velocity;
        // float velocityMaxX;
        // float velocityMinX;
        // float acceleration;
        // float drag; for ice level?
        float gravity;
        float velocityMaxY;
        bool canJump;

        // Initialization

        void initSprite();
        void initAnimations();
        void initPhysics();
        void initStats();

    public:
        // Constructors / Destructors

        Player(sf::Texture& texture_sheet);
        virtual ~Player();

        // Accessors

        bool getCanSwitchAnim();
        const sf::Vector2f getPosition() const;
        const sf::FloatRect getGlobalBounds() const;
        sf::Vector2f getVelocity() const;
        int getHealth() const;
        bool isAlive() const;

        // Inline Functions

        inline bool getCanJump() const { return canJump; }
        inline void setCanJump(bool value) { canJump = value; }
        inline bool isAttacking() const { return animState == ATTACKING; };

        inline int getPoints() const { return points; };
        inline void addPoints( int ammount ) { points += ammount; };
        

        // Modifiers

        void setPosition(const float x, const float y);
        void resetVelocityY();
        void resetVelocityX();
        void setAnimState(short state);

        void takeDamage(int ammount);
        void gainHealth(int ammount);

        // Functions

        void resetAnimationClock();
        void move(const float dir_x, const float dir_y);
        // void move(const float dir_x, const float dir_y);
        void jump();
        void updatePhysics();
        void updateMovement();
        void updateAnimations();

        void update();
        void render(sf::RenderTarget& target);


};
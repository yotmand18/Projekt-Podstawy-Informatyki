#pragma once

#include "lib.h"

class Settings;

class Input{
    private:
        // Key Bindings

        Settings* settings;

// ?
        // sf::Keyboard::Key moveLeft;
        // sf::Keyboard::Key moveRight;
        // sf::Keyboard::Key jump;
        // sf::Keyboard::Key attack;
        // sf::Keyboard::Key run;
        // sf::Keyboard::Key HealthPotion;
        // sf::Keyboard::Key SpeedPotion;
        // sf::Keyboard::Key AttackPotion;



    public:
        // Constructors / Destructors

        Input(Settings* the_settings);
        virtual ~Input();

        // Accessors
        bool isAction(std::string action) const;

        // Modifiers

// ?
        // bool isMoveLeft() const;
        // bool isMoveRight() const;
        // bool isJump() const;
        // bool isAttack() const;
        // bool isRunning() const;
        // bool isHealthPotion() const;
        // bool isSpeedPotion() const;
        // bool isAttackPotion() const;

        // Functions

    


};
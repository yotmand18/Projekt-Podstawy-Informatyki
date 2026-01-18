#pragma once

#include "lib.h"

class Input{
    private:
        // Key Bindings
        sf::Keyboard::Key moveLeft;
        sf::Keyboard::Key moveRight;
        sf::Keyboard::Key jump;
        sf::Keyboard::Key attack;
        sf::Keyboard::Key run;
        sf::Keyboard::Key HealthPotion;
        sf::Keyboard::Key SpeedPotion;
        sf::Keyboard::Key AttackPotion;


    public:
        // Constructors / Destructors

        Input();
        virtual ~Input();

        // Accessors
        bool isMoveLeft() const;
        bool isMoveRight() const;
        bool isJump() const;
        bool isAttack() const;
        bool isRunning() const;
        bool isHealthPotion() const;
        bool isSpeedPotion() const;
        bool isAttackPotion() const;

        // Functions

        //--Rebindinds, later
    


};
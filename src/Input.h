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

        // Functions

        //--Rebindinds, later
    


};
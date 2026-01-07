#include "Input.h"

// Constructors / Destructors

Input::Input(){
    // Default keybindings
    this->moveLeft = sf::Keyboard::Key::A;
    this->moveRight = sf::Keyboard::Key::D;
    this->jump = sf::Keyboard::Key::W;
    this->attack = sf::Keyboard::Key::J;
    this->run = sf::Keyboard::Key::LShift;

}
Input::~Input(){

}

bool Input::isMoveLeft() const {
    return sf::Keyboard::isKeyPressed(this->moveLeft);
}

bool Input::isMoveRight() const {
    return sf::Keyboard::isKeyPressed(this->moveRight);
}

bool Input::isJump() const {
    return sf::Keyboard::isKeyPressed(this->jump);
}
bool Input::isAttack() const {
    return sf::Keyboard::isKeyPressed(this->attack);
}
bool Input::isRunning() const{
    return sf::Keyboard::isKeyPressed(this->run);
}
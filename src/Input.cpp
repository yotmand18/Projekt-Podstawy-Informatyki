#include "Input.h"

// Constructors / Destructors

Input::Input(){
    // Default keybindings
    this->moveLeft = sf::Keyboard::Key::A;
    this->moveRight = sf::Keyboard::Key::D;
    this->jump = sf::Keyboard::Key::W;
    this->attack = sf::Keyboard::Key::J;
    this->run = sf::Keyboard::Key::LShift;
    this->HealthPotion = sf::Keyboard::Key::Num1;
    this->SpeedPotion = sf::Keyboard::Key::Num2;
    this->AttackPotion = sf::Keyboard::Key::Num3;

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
bool Input::isRunning() const {
    return sf::Keyboard::isKeyPressed(this->run);
}

bool Input::isHealthPotion() const{
    return sf::Keyboard::isKeyPressed(this->HealthPotion);
}

bool Input::isSpeedPotion() const{
    return sf::Keyboard::isKeyPressed(this->SpeedPotion);
}

bool Input::isAttackPotion() const{
    return sf::Keyboard::isKeyPressed(this->AttackPotion);
}

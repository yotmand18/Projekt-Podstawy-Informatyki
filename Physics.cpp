#include "Physics.h"

// Constructors / Destructors

Physics::Physics(sf::Vector2f currentVelocity, sf::Sprite* theSprite, bool applyGrv){
    this->velocity = currentVelocity;
    this->sprite = theSprite;
    this->gravity = 4.f;
    this->velocityMaxY = 200.f;
    this->applyGravity = applyGrv;
    this->canJump = true;
}

Physics::~Physics(){


}

// Functions

void Physics::move(const float dir_x, const float dir_y){
    // Acceleration
    //--this->velocity.x += dir_x * this->acceleration;
    this->velocity.x = dir_x;

    // Limit Velocity
    //--if(std::abs(this->velocity.x) > this->velocityMax){
    //     this->velocity.x = this->velocityMax*((this->velocity.x < 0) ? -1.f : 1.f);
    // }
}

void Physics::jump(){
    if(this->canJump){  // ADD THIS CHECK if not there
        this->velocity.y = -50.f;
        this->canJump = false;
    }
}

void Physics::update(){
    // Gravity

    if(this->applyGravity)
        this->velocity.y += this->gravity;

    // Falling
    if(this->velocity.y > this->velocityMaxY)
        this->velocity.y = this->velocityMaxY;

    this->sprite->move(this->velocity.x, this->velocity.y);
}
#include "Player.h"

// Initialization

void Player::initAnimations(){
    this->animationClock.restart();
    this->animState = IDLE;
    this->previousAnimState = IDLE;
    this->velocity = {0.f, 0.f};

    this->canSwitchAnimation = true;
    this->animationForward = true;
}

void Player::initPhysics(){
    this->gravity = 4.f;
    this->velocityMaxY = 200.f;
    this->canJump = false;
}

void Player::initStats(){
    this->health = 10;
    this->maxHealth = 30;
    this->points = 0;

    // Flags

    this->lostHealth = false;
    this->gainedHealth = false;
}

// Constructors / Destructors

Player::Player(sf::Texture& texture_sheet){
    this->sprite.setTexture(texture_sheet);
    this->currentFrame = sf::IntRect(0, 0, 28, 30);
    
    this->sprite.setScale(5.f, 5.f);
    
    this->hitbox = sf::FloatRect(0.f, 0.f, 22.f * 5.f, 30.f * 5.f);
    
    this->sprite.setTextureRect(this->currentFrame);
    
    this->initAnimations();
    this->initPhysics();
    this->initStats();
}

Player::~Player(){


}

// Accessors

bool Player::getCanSwitchAnim(){
    bool anim_switch = this->canSwitchAnimation;

    if(this->canSwitchAnimation)
        canSwitchAnimation = false;

    return anim_switch;
}

const sf::Vector2f Player::getPosition() const{
    return this->sprite.getPosition();
}

const sf::FloatRect Player::getGlobalBounds() const{
    sf::FloatRect bounds = this->hitbox;
    bounds.left = this->sprite.getPosition().x;
    bounds.top = this->sprite.getPosition().y;
    return bounds;
}
sf::Vector2f Player::getVelocity() const{
    return this->velocity;
}
int Player::getHealth() const{
    return this->health;
}
bool Player::isAlive() const{
    return (this->health > 0);
}

// Modifiers

void Player::setPosition(const float x, const float y){
    this->sprite.setPosition(x, y);
}
void Player::resetVelocityY(){
    this->velocity.y = 0.f;
}
void Player::resetVelocityX(){
    this->velocity.x = 0.f;
}
void Player::setAnimState(short state){
    if(state == ATTACKING){
        this->animationClock.restart();  // Start attack animation from beginning
        this->currentFrame.left = 0.f;   // Reset to first frame
    }
    this->animState = state;
}
void Player::takeDamage(int ammount){
    this->lostHealth = true;

    if(this->health - ammount <= 0)
        this->health = 0;
    else
        this->health -= ammount;

}
void Player::gainHealth(int ammount){
    if(this->health + ammount > this->maxHealth)
        this->health = this->maxHealth;
    else
        this->health += ammount;

    this->gainedHealth = true;
}


// Functions

void Player::resetAnimationClock(){
    this->animationClock.restart();
    this->canSwitchAnimation = true;
}



void Player::move(const float dir_x, const float dir_y){
    // Acceleration
    //--this->velocity.x += dir_x * this->acceleration;
    this->velocity.x = dir_x;

    // Limit Velocity
    //--if(std::abs(this->velocity.x) > this->velocityMax){
    //     this->velocity.x = this->velocityMax*((this->velocity.x < 0) ? -1.f : 1.f);
    // }
}

void Player::jump(){
    this->velocity.y = -50.f;
    setCanJump(false);
}

void Player::updatePhysics(){
    // Gravity


    this->velocity.y += this->gravity;

    // Falling
    if(this->velocity.y > this->velocityMaxY)
        this->velocity.y = this->velocityMaxY;

    this->sprite.move(this->velocity.x, this->velocity.y);
}

void Player::updateMovement(){
    // Don't change state if attacking
    if(this->animState == ATTACKING)
        return;
    
    // Check for running (faster movement)
    if(this->velocity.x > 3.f)  // Threshold for running
        this->animState = RUNNING_RIGHT;
    else if(this->velocity.x < -3.f)
        this->animState = RUNNING_LEFT;
    else if(this->velocity.x > 0.f)
        this->animState = MOVING_RIGHT;
    else if(this->velocity.x < 0.f)
        this->animState = MOVING_LEFT;
    else   
        this->animState = IDLE;
}

void Player::updateAnimations(){
    if(this->animState == ATTACKING){
        if(this->animationClock.getElapsedTime().asSeconds() >= 0.05f){
            this->currentFrame.top = 90.f;
            this->currentFrame.width = 37.f;
            this->currentFrame.left += 37.f;
            
            if(this->currentFrame.left >= 333.f){
                this->currentFrame.left = 0.f;
                this->animState = IDLE;
            }
            
            this->animationClock.restart();
            this->sprite.setTextureRect(this->currentFrame);
        }
        
    } else if(this->animState == IDLE){
        if(previousAnimState != IDLE){
            this->currentFrame.left = 0.f;
        }
        
        this->currentFrame.width = 28.f;
        this->currentFrame.height = 30.f;
        
        if(this->animationClock.getElapsedTime().asSeconds() >= 0.2f || this->getCanSwitchAnim()){
            this->currentFrame.top = 0.f;
            if(this->animationForward == true)
                this->currentFrame.left += 28.f;
            else
                this->currentFrame.left -= 28.f;
            if(this->currentFrame.left >= 280.f){
                this->animationForward = false;
                this->currentFrame.left -= 28.f;
            }
            if(this->currentFrame.left <= 0.f)
                this->animationForward = true;

            this->animationClock.restart();
            this->sprite.setTextureRect(this->currentFrame);
        }
        this->sprite.setScale(5.f, 5.f);
        this->sprite.setOrigin(0.f, 0.f);
        
    } else if(this->animState == MOVING_LEFT){
        if(previousAnimState != MOVING_LEFT){
            this->currentFrame.left = 0.f;
        }
        
        this->currentFrame.width = 22.f;
        this->currentFrame.height = 30.f;
        
        if(this->animationClock.getElapsedTime().asSeconds() >= 0.15f || this->getCanSwitchAnim()){
            this->currentFrame.top = 30.f;
            this->currentFrame.left += 22.f;

            if(this->currentFrame.left >= 110.f)
                this->currentFrame.left = 0.f;

            this->animationClock.restart();
            this->sprite.setTextureRect(this->currentFrame);
        }
        this->sprite.setScale(-5.f, 5.f);
        this->sprite.setOrigin(22.f, 0.f);

    } else if(this->animState == MOVING_RIGHT){
        if(previousAnimState != MOVING_RIGHT){
            this->currentFrame.left = 0.f;
        }
        
        this->currentFrame.width = 22.f;  
        this->currentFrame.height = 30.f;  
        
        if(this->animationClock.getElapsedTime().asSeconds() >= 0.15f || this->getCanSwitchAnim()){
            this->currentFrame.top = 30.f;  
            this->currentFrame.left += 22.f;

            if(this->currentFrame.left >= 110.f)  
                this->currentFrame.left = 0.f;

            this->animationClock.restart();
            this->sprite.setTextureRect(this->currentFrame);
        }
        this->sprite.setScale(5.f, 5.f);
        this->sprite.setOrigin(0.f, 0.f); 
    } else if(this->animState == RUNNING_LEFT){
        if(previousAnimState != RUNNING_LEFT){
            this->currentFrame.left = 0.f;
        }
        
        this->currentFrame.width = 22.f;  
        this->currentFrame.height = 30.f;
        
        if(this->animationClock.getElapsedTime().asSeconds() >= 0.1f){  
            this->currentFrame.top = 60.f;  
            this->currentFrame.left += 22.f;

            if(this->currentFrame.left >= 110.f) 
                this->currentFrame.left = 0.f;

            this->animationClock.restart();
            this->sprite.setTextureRect(this->currentFrame);
        }
        this->sprite.setScale(-5.f, 5.f);
        this->sprite.setOrigin(22.f, 0.f);

    } else if(this->animState == RUNNING_RIGHT){
        if(previousAnimState != RUNNING_RIGHT){
            this->currentFrame.left = 0.f;
        }
        
        this->currentFrame.width = 22.f;
        this->currentFrame.height = 30.f;
        
        if(this->animationClock.getElapsedTime().asSeconds() >= 0.1f){
            this->currentFrame.top = 60.f;  // Running sprite row
            this->currentFrame.left += 22.f;

            if(this->currentFrame.left >= 110.f)
                this->currentFrame.left = 0.f;

            this->animationClock.restart();
            this->sprite.setTextureRect(this->currentFrame);
        }
        this->sprite.setScale(5.f, 5.f);
        this->sprite.setOrigin(0.f, 0.f);
    }
    
    previousAnimState = this->animState;
}

void Player::update(){
    this->updatePhysics();
    this->updateMovement();
    this->updateAnimations();
}

void Player::render(sf::RenderTarget& target){
    target.draw(this->sprite);
}
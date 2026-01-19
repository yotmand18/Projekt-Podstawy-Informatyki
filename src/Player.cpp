#include "Player.h"

// Initialization
void Player::initAnimations(){
    this->animationClock.restart();
    this->animState = IDLE;
    this->previousAnimState = IDLE;
    this->canSwitchAnimation = true;
    this->animationForward = true;
}

void Player::initStats(){
    this->level = 1;
    this->health = 20*level;
    this->attack = 2*level;
    this->maxHealth = 20*level;
    this->points = 0;

    this->levelUpPoints = 20;
    this->damageCooldown = 1.f;
    this->lostHealth = false;
    this->gainedHealth = false;
    this->hasDealtDamage = false;
    this->facingRight = true;

    this->HealthPotions = 1;
    this->SpeedPotions= 1;
    this->AttackPotions = 1;
    this->timeSpeedPotion = 0;
    this->timeAttackPotion = 0;
    this->timeHealthPotion = 0;

}

// Constructors / Destructors
Player::Player(sf::Texture& texture_sheet){
    this->sprite.setTexture(texture_sheet);
    this->currentFrame = sf::IntRect(0, 0, 28, 30);
    this->sprite.setScale(5.f, 5.f);
    this->hitbox = sf::FloatRect(0.f, 0.f, 22.f * 5.f, 30.f * 5.f);
    this->sprite.setTextureRect(this->currentFrame);
    
    // Create physics object
    this->physics = new Physics(sf::Vector2f(0.f, 0.f), &this->sprite, true);
    
    this->initAnimations();
    this->initStats();
}

Player::~Player(){
    delete this->physics;
}

// Accessors
bool Player::getCanSwitchAnim(){
    bool anim_switch = this->canSwitchAnimation;
    if(this->canSwitchAnimation)
        canSwitchAnimation = false;
    return anim_switch;
}
const sf::FloatRect Player::getGlobalBounds() const{
    sf::FloatRect bounds = this->hitbox;
    bounds.left = this->sprite.getPosition().x;
    bounds.top = this->sprite.getPosition().y;
    return bounds;
}

sf::FloatRect Player::getAttackBounds() const{
    sf::FloatRect attackBounds = this->getGlobalBounds();
    if(this->facingRight) {
        attackBounds.width += 50.f;
    } else {
        attackBounds.left -= 50.f;
        attackBounds.width += 50.f;
    }
    return attackBounds;
}

// Modifiers
void Player::setPosition(const float x, const float y){
    this->sprite.setPosition(x, y);
}

void Player::setAnimState(short state){
    if(state == ATTACKING){
        this->animationClock.restart();
        this->currentFrame.left = 0.f;
        this->hasDealtDamage = false;
    }
    this->animState = state;
}

void Player::takeDamage(int amount){
    if(this->damageCooldownClock.getElapsedTime().asSeconds() < this->damageCooldown)
        return;
    
    std::cout << "Player health: " << this->health << "\n";
    this->lostHealth = true;
    
    if(this->health - amount <= 0)
        this->health = 0;
    else
        this->health -= amount;
    
    this->damageCooldownClock.restart();
    this->sprite.setColor(sf::Color::Red);
}

void Player::gainHealth(int amount){
    if(this->health + amount > this->maxHealth)
        this->health = this->maxHealth;
    else
        this->health += amount;
    this->gainedHealth = true;
    this->sprite.setColor(sf::Color::Green);
}


void Player::modHealthPotion(int amount) { this->HealthPotions += amount; }
void Player::modAttackPotion(int amount) { this->AttackPotions += amount; }
void Player::modSpeedPotion(int amount) { this->SpeedPotions += amount; }




inline void Player::LevelUp() {
    this->level++;
    this->attack = 2 * this->getLevel();
    this->health = 20 * this->getLevel(); //Heal on levelup?
    this->maxHealth = 20 * this->getLevel();
    this->modHealthPotion(1); this->modSpeedPotion(1); this->modAttackPotion(1);

}

// Functions
bool Player::isInAttackHitFrame() const{
    if(this->animState != ATTACKING)
        return false;
    return (this->currentFrame.left >= 148.f && this->currentFrame.left <= 222.f);
}

void Player::resetAnimationClock(){
    this->animationClock.restart();
    this->canSwitchAnimation = true;
}

void Player::move(const float dir_x, const float dir_y){
    this->physics->move(dir_x, dir_y);
}

void Player::jump(){
    this->physics->jump();
}

void Player::updateMovement(){
    if(this->animState == ATTACKING)
        return;
    
    sf::Vector2f velocity = this->physics->getVelocity();
    
    if(velocity.x > 0.f)
        this->facingRight = true;
    else if(velocity.x < 0.f)
        this->facingRight = false;
    
    if(velocity.x > 3.f)
        this->animState = RUNNING_RIGHT;
    else if(velocity.x < -3.f)
        this->animState = RUNNING_LEFT;
    else if(velocity.x > 0.f)
        this->animState = MOVING_RIGHT;
    else if(velocity.x < 0.f)
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
            this->currentFrame.top = 60.f;
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
    this->previousPosition = this->sprite.getPosition();
    this->physics->update();
    this->updateMovement();
    this->updateAnimations();

    if(this->lostHealth && this->damageCooldownClock.getElapsedTime().asSeconds() > 0.2f){
        this->sprite.setColor(sf::Color::White);
        this->lostHealth = false;
    }
    if(this->gainedHealth){
        this->sprite.setColor(sf::Color::White);
        this->gainedHealth = false;
    }
    if ((clock() - this->timeAttackPotion) / CLOCKS_PER_SEC > 15) { this->timeAttackPotion = 0;}
    if ((clock() - this->timeSpeedPotion) / CLOCKS_PER_SEC > 15) { this->timeSpeedPotion = 0; }
    if ((clock() - this->timeHealthPotion) / CLOCKS_PER_SEC > 0.5) { this->timeHealthPotion = 0; }
    if (this->getPoints() >= this->levelUpPoints) { this->points -= this->levelUpPoints; this->LevelUp(); }
}

void Player::render(sf::RenderTarget& target){
    target.draw(this->sprite);
}
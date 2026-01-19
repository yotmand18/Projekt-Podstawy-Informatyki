#include "Enemy.h"

void Enemy::initStats(){
    switch(this->enemyType){
        case DROWNER:
            this->health = 10;
            this->damage = 2;
            this->moveSpeed = 2.f;
            this->enemyBoundaries = {0, 0, 32, 32};
            this->isFlying = false;
            this->isAgressive = false;
            this->XP = 10;
            break;
        case NEKKER:
            this->health = 5;
            this->damage = 1;
            this->moveSpeed = 4.f;
            this->enemyBoundaries = {0, 0, 16, 16};
            this->isFlying = false;
            this->isAgressive = true;
            this->agroRange = 300.f;
            this->XP = 5;
            break;
        case GHOUL:
            this->health = 15;
            this->damage = 5;
            this->moveSpeed = 3.f;
            this->enemyBoundaries = {0, 0, 32, 32};
            this->isFlying = false;
            this->isAgressive = true;
            this->agroRange = 500.f;
            this->XP = 20;
            break;
        case WRAITH:
            this->health = 30;
            this->damage = 4;
            this->moveSpeed = 1.f;
            this->enemyBoundaries = {0, 0, 16, 32};
            this->isFlying = true;
            this->isAgressive = false;
            this->XP = 40;
            break;
        case GRIFFIN:
            this->health = 60;
            this->damage = 8;
            this->moveSpeed = 1.f;
            this->enemyBoundaries = {0, 0, 48, 48};
            this->isFlying = true;
            this->isAgressive = true;
            this->agroRange = 600.f;
            break;
        default:
            std::cout << "ERROR::ENEMY::Unknown type loaded";
            break;
    }
    this->movingRight = true;
    this->isChasing = false;
}

void Enemy::initSprite(sf::Texture* texture){
    if(texture != nullptr){
        this->sprite.setTexture(*texture);
        this->sprite.setScale(5.f, 5.f);
        this->currentFrame = this->enemyBoundaries;
        this->sprite.setTextureRect(this->currentFrame);
    }
    this->hitbox = sf::FloatRect(
        0.f, 0.f, 
        this->enemyBoundaries.width * 5.f,
        this->enemyBoundaries.height * 5.f
    );
}

Enemy::Enemy(float x, float y, ENEMY_TYPE enemy_type, sf::Texture* texture, float patrol_left, float patrol_right){
    this->enemyType = enemy_type;
    this->patrolLeft = patrol_left;
    this->patrolRight = patrol_right;
    
    this->initStats();
    this->initSprite(texture);
    
    // Create physics - flying enemies don't use gravity
    this->physics = new Physics(sf::Vector2f(0.f, 0.f), &this->sprite, !this->isFlying);
    
    this->sprite.setPosition(x, y);
}

Enemy::~Enemy(){
    delete this->physics;
}

const sf::Vector2f Enemy::getPosition() const{
    return this->sprite.getPosition();
}

const sf::FloatRect Enemy::getGlobalBounds() const{
    return this->sprite.getGlobalBounds();
}

int Enemy::getDamage() const{
    return this->damage;
}

int Enemy::getHealth() const {
    return this->health;
}

int Enemy::getXP() const {
    return this->XP;
}

bool Enemy::isAlive() const{
    return (this->health > 0);
}

bool Enemy::canChase() const{
    return this->isAgressive && this->isChasing;
}

sf::Vector2f Enemy::getVelocity() const{
    return this->physics->getVelocity();
}



// Modifiers

void Enemy::setPosition(float x, float y){
    this->sprite.setPosition(x, y);
}

void Enemy::takeDamage(int amount){
    this->health -= amount;
    if(this->health < 0)
        this->health = 0;
    this->sprite.setColor(sf::Color::Red);
    this->damageClock.restart();
}

void Enemy::updatePlayerPosition(const sf::Vector2f& pos){
    this->playerPos = pos;
    if(this->isAgressive){
        float distance = std::sqrt(
            std::pow(pos.x - this->sprite.getPosition().x, 2) +
            std::pow(pos.y - this->sprite.getPosition().y, 2)
        );
        this->isChasing = (distance <= this->agroRange);
    }
}

// Functions

void Enemy::jump(){
    this->physics->jump();
}

void Enemy::updateMovement(){
    if(!this->isFlying){
        float currentX = this->sprite.getPosition().x;
        
        if(this->canChase()){
            float chaseSpeed = this->moveSpeed * 1.5f;
            
            if(this->playerPos.x > currentX){
                this->sprite.move(chaseSpeed, 0.f);
                if(!this->movingRight){
                    this->movingRight = true;
                    this->sprite.setScale(5.f, 5.f);
                    this->sprite.setOrigin(0.f, 0.f);
                }
            } else {
                this->sprite.move(-chaseSpeed, 0.f);
                if(this->movingRight){
                    this->movingRight = false;
                    this->sprite.setScale(-5.f, 5.f);
                    this->sprite.setOrigin(this->enemyBoundaries.width, 0.f);
                }
            }
        } else {
            if(this->movingRight){
                this->sprite.move(this->moveSpeed, 0.f);
                if(currentX >= this->patrolRight){
                    this->movingRight = false;
                    this->sprite.setScale(-5.f, 5.f);
                    this->sprite.setOrigin(this->enemyBoundaries.width, 0.f);
                }
            } else {
                this->sprite.move(-this->moveSpeed, 0.f);
                if(currentX <= this->patrolLeft){
                    this->movingRight = true;
                    this->sprite.setScale(5.f, 5.f);
                    this->sprite.setOrigin(0.f, 0.f);
                }
            }
        }
    }
}

void Enemy::updateAnimations(){
    if(this->animationClock.getElapsedTime().asSeconds() >= 0.3f) {
        this->currentFrame.left += this->enemyBoundaries.width;
        if(this->currentFrame.left >= this->enemyBoundaries.width * 2)
            this->currentFrame.left = 0;
        this->sprite.setTextureRect(this->currentFrame);
        this->animationClock.restart();
    }
}

void Enemy::update(){
    if(this->isAlive()) {
        this->previousPosition = this->sprite.getPosition();
        this->physics->update();
        this->updateMovement();
        this->updateAnimations();
        if(this->damageClock.getElapsedTime().asSeconds() > 0.2f){
            this->sprite.setColor(sf::Color::White);
        }
    }
}

void Enemy::render(sf::RenderTarget& target){
    if(this->isAlive()) {
        target.draw(this->sprite);
    }
}
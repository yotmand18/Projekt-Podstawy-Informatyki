#include "Game.h"

// Initialization

void Game::initWindow(){
    this->window.create(sf::VideoMode(800, 600), "Game", sf::Style::Close | sf::Style::Titlebar);
    this->window.setFramerateLimit(60);

    // Initialize view
    this->view.setSize(800.f, 600.f);
    this->view.setCenter(400.f, 300.f);

}

void Game::initTextures(){
    if(!this->playerTextureSheet.loadFromFile("./textures/player/player_stylesheet.png"))
        std::cout << "ERROR::GAME::Failed to load texture\n";
    else
        this->playerTextureSheet.setSmooth(false);  // Sharp pixels
}

void Game::initInput(){
    this->input = new Input();
}

void Game::initLevel(){
    this->level = new Level();
    if(!this->level->loadFromFile("./levels/level1.txt"))
        std::cout << "ERROR::GAME::Failed to load level\n";
}

void Game::initPlayer(){
    this->player = new Player(this->playerTextureSheet);
    this->player->setPosition(0.f, 450.f);
}

// Constructors / Destructors

Game::Game(){
    this->initWindow();
    this->initTextures();
    this->initInput();
    this->initLevel();
    this->initPlayer();

}
Game::~Game(){
    delete this->input;
    delete this->player;
    delete this->level;
}

// Functions

void Game::updateInput(){
    float moveSpeed = 2.f;
    
    // Check if running
    if(this->input->isRunning())
        moveSpeed = 6.f;  // 2x speed when running
    
    float moveX = 0.f;
    
    if (this->input->isMoveLeft()) moveX -= moveSpeed;
    if (this->input->isMoveRight()) moveX += moveSpeed;
    
    this->player->move(moveX, 0.f);

    // Only allow attack if not already attacking
    if(this->input->isAttack() && !this->player->isAttacking())
        this->player->setAnimState(ATTACKING);
}

void Game::updatePlayer(){
    this->player->update();
}

void Game::updateCollision() {
    sf::FloatRect playerBounds = this->player->getGlobalBounds();
    sf::Vector2f velocity = this->player->getVelocity();
    bool onGround = false;

    for (const auto* platform : this->level->getPlatforms()) {
        sf::FloatRect platformBounds = platform->getBounds();

        if (playerBounds.intersects(platformBounds)) {
            // Calculate overlaps on both axes
            float overlapX = 0.f;
            float overlapY = 0.f;

            // Horizontal overlap
            if (playerBounds.left < platformBounds.left)
                overlapX = playerBounds.left + playerBounds.width - platformBounds.left;
            else
                overlapX = platformBounds.left + platformBounds.width - playerBounds.left;

            // Vertical overlap
            if (playerBounds.top < platformBounds.top)
                overlapY = playerBounds.top + playerBounds.height - platformBounds.top;
            else
                overlapY = platformBounds.top + platformBounds.height - playerBounds.top;

            // Resolve on the SHORTEST axis first (this prevents "snagging" on corners)
            if (std::abs(overlapX) < std::abs(overlapY)) {
                // Horizontal Resolution
                if (playerBounds.left < platformBounds.left)
                    this->player->setPosition(platformBounds.left - playerBounds.width, playerBounds.top);
                else
                    this->player->setPosition(platformBounds.left + platformBounds.width, playerBounds.top);
                
                this->player->resetVelocityX();
            } 
            else {
                // Vertical Resolution
                if (playerBounds.top < platformBounds.top) { // Falling onto platform
                    this->player->setPosition(playerBounds.left, platformBounds.top - playerBounds.height);
                    onGround = true;
                } else { // Hitting head on ceiling
                    this->player->setPosition(playerBounds.left, platformBounds.top + platformBounds.height);
                }
                this->player->resetVelocityY();
            }
            // Update bounds after resolution for the next platform check
            playerBounds = this->player->getGlobalBounds();
        }
    }

    // Screen Boundary
    if (this->player->getPosition().y + playerBounds.height >= this->window.getSize().y) {
        this->player->resetVelocityY();
        this->player->setPosition(this->player->getPosition().x, this->window.getSize().y - playerBounds.height);
        onGround = true;
    }

    this->player->setCanJump(onGround);
}

void Game::updateView(){
    // Center view on player horizontally, keep Y fixed
    this->view.setCenter(
        this->player->getPosition().x + this->player->getGlobalBounds().width / 2.f,
        300.f
    );

    // Add boundaries so camera doesn't go too far left
    if(this->view.getCenter().x < 400.f)
        this->view.setCenter(400.f, 300.f);
}

void Game::update(){
    // Polling window events
    while(this->window.pollEvent(this->ev)){
        if(this->ev.type == sf::Event::Closed)
            this->window.close();
        else if(this->ev.type == sf::Event::KeyPressed && this->ev.key.code == sf::Keyboard::Escape)
            this->window.close();
        
        // ADD JUMP HERE - only triggers once per press
        else if(this->ev.type == sf::Event::KeyPressed && this->ev.key.code == sf::Keyboard::W){
            if(this->player->getCanJump())
                this->player->jump();
        }
    }

    this->updateInput();
    this->updatePlayer();
    this->updateCollision();
    this->updateView();
}
void Game::renderPlayer(){
    this->player->render(this->window);
}
void Game::render(){
    // Clearing window
    this->window.clear(sf::Color::White);

    // apply camera

    this->window.setView(this->view);

    // Rendering level

    this->level->render(this->window);

    // Rendering player

    this->renderPlayer();

    // Displaynig

    this->window.display();
}

const sf::RenderWindow& Game::getWindow() const{
    return this->window;
}
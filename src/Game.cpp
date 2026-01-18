#include "Game.h"

// Initialization

void Game::initWindow(){
    sf::Vector2u res = this->settings.getResolution();

    this->window.create(
        sf::VideoMode(res.x, res.y),
        "Game",
        sf::Style::Close | sf::Style::Titlebar | sf::Style::Resize
    );    this->window.setFramerateLimit(60);

    // Initialize view
    this->view.setSize(1280.f, 720.f);
    this->view.setCenter(630.f, 360.f);

}

void Game::initUI(){
    this->ui = new UI();
    this->ui->setSettings(&this->settings);
}

void Game::initTextures(){
    if(!this->playerTextureSheet.loadFromFile("./textures/player/player_stylesheet.png"))
        std::cout << "ERROR::GAME::Failed to load player texture\n";
    else
        this->playerTextureSheet.setSmooth(false);
    
    // Load background
    if(!this->backgroundTexture.loadFromFile("./textures/backgrounds/background.png"))
        std::cout << "ERROR::GAME::Failed to load background\n";
    else {
        this->backgroundTexture.setSmooth(false);
        this->backgroundSprite.setTexture(this->backgroundTexture);
        
        // Scale to fit view
        float scaleX = 1280.f / this->backgroundTexture.getSize().x;
        float scaleY = 720.f / this->backgroundTexture.getSize().y;
        this->backgroundSprite.setScale(scaleX, scaleY);
    }
}

void Game::initInput(){
    this->input = new Input(&this->settings);
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
    this->settings.loadFromFile();

    this->initWindow();
    this->initUI();
    this->initTextures();
    this->initInput();
    this->initLevel();
    this->initPlayer();

}
Game::~Game(){
    delete this->input;
    delete this->player;
    delete this->level;
    delete this->ui;
}

// Functions

void Game::updateInput(){
    float moveSpeed = 2.f;
    
    // Check if running
    if(this->input->isAction("RUN"))
        moveSpeed = 6.f;  // 2x speed when running
    
    float moveX = 0.f;
    
    if (this->input->isAction("MOVE_LEFT")) moveX -= moveSpeed;
    if (this->input->isAction("MOVE_RIGHT")) moveX += moveSpeed;
    
    this->player->move(moveX, 0.f);

    // Only allow attack if not already attacking
    if(this->input->isAction("ATTACK") && !this->player->isAttacking())
        this->player->setAnimState(ATTACKING);
}

void Game::updatePlayer(){
    this->player->update();
}

void Game::updateCollision(){
    bool playerOnGround = false;

    // =========================
    // PLAYER vs PLATFORMS
    // =========================
    {
        sf::FloatRect bounds = player->getGlobalBounds();
        sf::Vector2f prevPos = player->getPrevPosition();

        sf::FloatRect prevBounds = bounds;
        prevBounds.left = prevPos.x;
        prevBounds.top  = prevPos.y;


        for (const auto* platform : level->getPlatforms()){
            sf::FloatRect plat = platform->getBounds();

            if (!bounds.intersects(plat))
                continue;

            // penetration depths
            float dxLeft   = (bounds.left + bounds.width)  - plat.left;
            float dxRight  = (plat.left + plat.width)      - bounds.left;
            float dyTop    = (bounds.top + bounds.height)  - plat.top;
            float dyBottom = (plat.top + plat.height)      - bounds.top;

            float penX = std::min(dxLeft, dxRight);
            float penY = std::min(dyTop, dyBottom);

            bool cameFromSide =
            prevBounds.top + prevBounds.height > plat.top &&
            prevBounds.top < plat.top + plat.height;

            bool falling = prevPos.y < bounds.top;

            if (penX < penY && cameFromSide && !falling) {
                // -------- horizontal --------
                if (prevPos.x + bounds.width <= plat.left){
                    // came from left
                    player->setPosition(plat.left - bounds.width, bounds.top);
                } else {
                    // came from right
                    player->setPosition(plat.left + plat.width, bounds.top);
                }
                player->resetVelocityX();
            } else {
                // -------- vertical --------
                if (prevPos.y + bounds.height <= plat.top){
                    // landed on platform
                    player->setPosition(bounds.left, plat.top - bounds.height);
                    playerOnGround = true;
                } else {
                    // hit ceiling
                    player->setPosition(bounds.left, plat.top + plat.height);
                }
                player->resetVelocityY();
            }

            bounds = player->getGlobalBounds();
        }
    }

    // =========================
    // PLAYER vs SCREEN BOTTOM
    // =========================
    {
        sf::FloatRect b = player->getGlobalBounds();
        float bottom = this->view.getSize().y;

        if (b.top + b.height >= bottom){
            player->setPosition(b.left, bottom - b.height);
            player->resetVelocityY();
            playerOnGround = true;
        }
    }

    player->setCanJump(playerOnGround);

    // =========================
    // ENEMIES vs PLATFORMS
    // =========================
    for (auto* enemy : level->getEnemies()){
        bool hitWall = false;

        if (!enemy->isAlive())
            continue;

        bool enemyOnGround = false;
        sf::FloatRect bounds = enemy->getGlobalBounds();
        sf::Vector2f prevPos = enemy->getPrevPosition();

        sf::FloatRect prevBounds;
        prevBounds.left   = prevPos.x;
        prevBounds.top    = prevPos.y;
        prevBounds.width  = bounds.width;
        prevBounds.height = bounds.height;

        for (const auto* platform : level->getPlatforms()){
            sf::FloatRect plat = platform->getBounds();

            if (!bounds.intersects(plat))
                continue;

            float dxLeft   = (bounds.left + bounds.width)  - plat.left;
            float dxRight  = (plat.left + plat.width)      - bounds.left;
            float dyTop    = (bounds.top + bounds.height)  - plat.top;
            float dyBottom = (plat.top + plat.height)      - bounds.top;

            float penX = std::min(dxLeft, dxRight);
            float penY = std::min(dyTop, dyBottom);

            bool cameFromSide =
            prevBounds.top + prevBounds.height > plat.top &&
            prevBounds.top < plat.top + plat.height;

            if (penX < penY && cameFromSide) {  
                // horizontal collision
                hitWall = true;

                if (prevPos.x + bounds.width <= plat.left)
                    enemy->setPosition(plat.left - bounds.width, bounds.top);
                else
                    enemy->setPosition(plat.left + plat.width, bounds.top);

                enemy->resetVelocityX();
            } else {
                // vertical
                if (prevPos.y + bounds.height <= plat.top){
                    enemy->setPosition(bounds.left, plat.top - bounds.height);
                    enemyOnGround = true;
                } else {
                    enemy->setPosition(bounds.left, plat.top + plat.height);
                }
                enemy->resetVelocityY();
            }

            bounds = enemy->getGlobalBounds();
        }

        // =========================
        // ENEMY vs SCREEN BOTTOM
        // =========================
        {
            sf::FloatRect b = enemy->getGlobalBounds();
            float bottom = this->view.getSize().y;

            if (b.top + b.height >= bottom){
                enemy->setPosition(b.left, bottom - b.height);
                enemy->resetVelocityY();
                enemyOnGround = true;
            }
        }

        if (hitWall && enemyOnGround) {
            enemy->jump();
        }


        enemy->setCanJump(enemyOnGround);
    }
}



void Game::updateView(){
    // Center view on player horizontally, keep Y fixed
    this->view.setCenter(
        this->player->getPosition().x + this->player->getGlobalBounds().width / 2.f,
        360.f
    );

    // Add boundaries so camera doesn't go too far left
    if(this->view.getCenter().x < 640.f)
        this->view.setCenter(640.f, 360.f);
}

void Game::updateCombat() {
    sf::FloatRect playerBounds = this->player->getGlobalBounds();
    sf::FloatRect attackBounds = this->player->getAttackBounds();
    bool isAttacking = this->player->isAttacking();
    bool isHitFrame = this->player->isInAttackHitFrame();  // ADD THIS
    
    for(auto* enemy : this->level->getEnemies()) {
        if(!enemy->isAlive())
            continue;
        
        sf::FloatRect enemyBounds = enemy->getGlobalBounds();
        
        // Only deal damage during the hit frames AND if haven't hit yet
        if(isAttacking && isHitFrame && !this->player->hasAttackHit() && 
           attackBounds.intersects(enemyBounds)) {
            enemy->takeDamage(2);
            this->player->setAttackHit();
            std::cout << "Hit enemy! Enemy health: " << enemy->getHealth() << "\n";
        }
        
        if(playerBounds.intersects(enemyBounds)) {
            this->player->takeDamage(enemy->getDamage());
        }
    }
}

void Game::update(){
    while(this->window.pollEvent(this->ev)){
        if(this->ev.type == sf::Event::Closed)
            this->window.close();
        else if(this->ev.type == sf::Event::Resized){
            // ... resize handling
        }
        
        // Handle UI based on state
        if(this->ui->getGameState() == GameState::MAINMENU){
            this->ui->handleMainMenuInput(this->ev);
            
            if(this->ev.type == sf::Event::KeyPressed && this->ev.key.code == sf::Keyboard::Return){
                int selection = this->ui->getMenuSelection();
                switch(selection){
                    case 0:  // New Game
                        this->ui->setState(GameState::PLAYING);
                        break;
                    case 1:  // Continue
                        // TODO: Load save
                        this->ui->setState(GameState::PLAYING);
                        break;
                    case 2:  // Exit
                        this->window.close();
                        break;
                }
            }
            return;  // Don't update game while in menu
        }else if(this->ui->getGameState() == GameState::PAUSED){
            this->ui->handlePauseMenuInput(this->ev);
            
            if(this->ev.type == sf::Event::KeyPressed && this->ev.key.code == sf::Keyboard::Return){
                int selection = this->ui->getPauseSelection();
                switch(selection){
                    case 0:  // Resume
                        this->ui->setState(GameState::PLAYING);
                        break;
                    case 1:  // Main Menu
                        // TODO: Restart level
                        this->ui->setState(GameState::MAINMENU);
                        break;
                    case 2:  // EXIT
                        this->window.close();
                        break;
                }
            }
            return;  // Don't update game while paused
        }else if(this->ui->getGameState() == GameState::GAMEOVER || this->ui->getGameState() == GameState::VICTORY){
            this->ui->handleGameOverMenuInput(this->ev);
            
            if(this->ev.type == sf::Event::KeyPressed && this->ev.key.code == sf::Keyboard::Return){
                int selection = this->ui->getGameOverSelection();
                switch(selection){
                    case 0:  // Main Menu
                        this->ui->setState(GameState::MAINMENU);
                        break;
                    case 1:  // EXIT
                        this->window.close();
                        break;
                }
            }
            return;  // Don't update game while paused
        }
        
        // Game events (only during gameplay)
        if(this->ui->getGameState() == GameState::PLAYING){
            if(this->ev.type == sf::Event::KeyPressed && this->ev.key.code == sf::Keyboard::Escape){
                this->ui->setState(GameState::PAUSED);
            }
            else if(this->ev.type == sf::Event::KeyPressed && this->ev.key.code == sf::Keyboard::W){
                if(this->player->getCanJump())
                    this->player->jump();
            }
        }
    }
    
    // Only update game if playing
    if(this->ui->getGameState() == GameState::PLAYING){
        this->updateInput();
        this->updatePlayer();
        this->updateCollision();
        this->level->update(this->player->getPosition(), this->view);
        this->updateCombat();
        this->updateView();

        // Game Over

        if(!this->player->isAlive())
            this->ui->setState(GameState::GAMEOVER);
        
        // Update HUD
        this->ui->updateHUD(
            this->player->getHealth(), 
            this->player->getMaxHealth(), 
            this->player->getPoints(),
            1 // TODO
        );
    }
}

void Game::renderPlayer(){
    this->player->render(this->window);
}

void Game::render(){
    this->window.clear(sf::Color(20, 20, 30));
    
    // Main menu
    if(this->ui->getGameState() == GameState::MAINMENU){
        this->ui->render(this->window);
        this->window.display();
        return;
    }
    
    // Game rendering
    if(this->ui->getGameState() == GameState::PLAYING || this->ui->getGameState() == GameState::PAUSED){
        this->window.setView(this->view);
        this->level->render(this->window);
        this->renderPlayer();
        
        // Render HUD on top (reset to default view for fixed position)
        this->window.setView(this->window.getDefaultView());
        this->ui->renderHUD(this->window);
        
        // Render pause menu on top if paused
        if(this->ui->getGameState() == GameState::PAUSED){
            this->ui->renderPauseMenu(this->window);
        }
    }
    
    // Game over / Victory
    if(this->ui->getGameState() == GameState::GAMEOVER || this->ui->getGameState() == GameState::VICTORY){
        this->ui->render(this->window);
    }
    
    this->window.display();
}

const sf::RenderWindow& Game::getWindow() const{
    return this->window;
}
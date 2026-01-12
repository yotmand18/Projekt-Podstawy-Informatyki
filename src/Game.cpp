#include "Game.h"

// Initialization

void Game::initWindow(){
    this->window.create(sf::VideoMode(1280, 720), "Game", sf::Style::Close | sf::Style::Titlebar | sf::Style::Resize);
    this->window.setFramerateLimit(60);

    // Initialize view
    this->view.setSize(1280.f, 720.f);
    this->view.setCenter(630.f, 360.f);

}

void Game::initTextures(){
    if(!this->playerTextureSheet.loadFromFile("./textures/player/player_stylesheet.png"))
        std::cout << "ERROR::GAME::Failed to load player texture\n";
    else
        this->playerTextureSheet.setSmooth(false);
    
    // Load background
    if(!this->backgroundTexture.loadFromFile("./textures/backgrounds/level1.png"))
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

            if (penX < penY && cameFromSide) {
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
    // Polling window events
    while(this->window.pollEvent(this->ev)){
        if(this->ev.type == sf::Event::Closed)
            this->window.close();
        else if(this->ev.type == sf::Event::KeyPressed && this->ev.key.code == sf::Keyboard::Escape)
            this->window.close();

        // Handle window resize
        else if(this->ev.type == sf::Event::Resized){
            // Calculate aspect ratios
            float windowRatio = static_cast<float>(this->ev.size.width) / static_cast<float>(this->ev.size.height);
            float viewRatio = this->view.getSize().x / this->view.getSize().y;  // 1280/720 = 16:9
            
            sf::FloatRect viewport;
            
            if(windowRatio > viewRatio){
                // Window is wider - add pillarboxes (black bars on sides)
                float viewportWidth = viewRatio / windowRatio;
                viewport = sf::FloatRect((1.f - viewportWidth) / 2.f, 0.f, viewportWidth, 1.f);
            } else {
                // Window is taller - add letterboxes (black bars top/bottom)
                float viewportHeight = windowRatio / viewRatio;
                viewport = sf::FloatRect(0.f, (1.f - viewportHeight) / 2.f, 1.f, viewportHeight);
            }
            
            this->view.setViewport(viewport);
        }
        
        // Jump
        else if(this->ev.type == sf::Event::KeyPressed && this->ev.key.code == sf::Keyboard::W){
            if(this->player->getCanJump())
                this->player->jump();
        }
    }

    this->updateInput();
    this->updatePlayer();
    //this->level->update();
    this->updateCollision();
    this->updateCombat();

    this->updateView();
    this->level->update(this->player->getPosition(), this->view);

    if(!this->player->isAlive()){
        this->gameOver = true;
    }
}

void Game::renderPlayer(){
    this->player->render(this->window);
}
void Game::render(){
    // Clearing window
    this->window.clear();

    // apply camera

    this->window.setView(this->view);

    // Rendering level

    // Render background FIRST (behind everything)
    this->backgroundSprite.setPosition(
        this->view.getCenter().x - 640.f,  // Center on view
        0.f
    );
    this->window.draw(this->backgroundSprite);

    this->level->render(this->window);

    // Rendering player

    this->renderPlayer();

    // Displaynig

    this->window.display();
}

const sf::RenderWindow& Game::getWindow() const{
    return this->window;
}
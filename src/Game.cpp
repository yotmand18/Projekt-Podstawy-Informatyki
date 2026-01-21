#include "Game.h"
void Game::initWindow() {
    // Use settings resolution if present
    sf::Vector2u res = this->settings.getResolution();
    if (res.x == 0 || res.y == 0) {
        res = {1280, 720}; // fallback
    }

    this->window.create(
        sf::VideoMode(res.x, res.y),
        "Game",
        sf::Style::Close | sf::Style::Titlebar | sf::Style::Resize
    );
    this->window.setFramerateLimit(60);

    // Make camera size match resolution
    this->view.setSize(static_cast<float>(res.x), static_cast<float>(res.y));
    this->view.setCenter(static_cast<float>(res.x) / 2.f, static_cast<float>(res.y) / 2.f);
}

void Game::initUI() {
    this->ui = new UI(assetRoot);
    this->ui->setSettings(&this->settings);
}

void Game::initTextures() {
    // Build paths relative to normalized assetRoot (assetRoot ends with '/')
    if (!this->playerTextureSheet.loadFromFile(assetRoot + "textures/player/player_stylesheet.png")) {
        std::cout << "ERROR::GAME::Failed to load player texture\n";
    } else {
        this->playerTextureSheet.setSmooth(false);
    }

    if (!this->backgroundTexture.loadFromFile(assetRoot + "textures/backgrounds/background_2.png")) {
        std::cout << "ERROR::GAME::Failed to load background\n";
    } else {
        this->backgroundTexture.setSmooth(false);
        this->backgroundSprite.setTexture(this->backgroundTexture);

        // Scale background to cover the window resolution
        sf::Vector2u res = this->settings.getResolution();
        if (res.x == 0 || res.y == 0) {
            res = {1280, 720};
        }

        float scaleX = static_cast<float>(res.x) / static_cast<float>(this->backgroundTexture.getSize().x);
        float scaleY = static_cast<float>(res.y) / static_cast<float>(this->backgroundTexture.getSize().y);
        this->backgroundSprite.setScale(scaleX, scaleY);
    }
}

void Game::initInput() {
    // Input requires Settings pointer
    this->input = new Input(&this->settings);
}


void Game::initLevel() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    this->level = new Level(assetRoot);

    this->level->generateRandomLevel(10000.f, 3);

    this->level->setSettings(&this->settings);

    std::cout << "Level generated succesfully!\n";
}

void Game::initPlayer() {
    this->player = new Player(this->playerTextureSheet);

    // Place player on top of the starting platform
    const auto& platforms = this->level->getPlatforms();
    if (!platforms.empty()) {
        sf::FloatRect startPlatBounds = platforms[0]->getGlobalBounds();
        float playerHeight = 158.f; // frame height
        this->player->setPosition(
            startPlatBounds.left,               // X: left edge of platform
            startPlatBounds.top - playerHeight  // Y: on top of platform
        );
    } else {
        // fallback if no platforms
        this->player->setPosition(0.f, this->level->getWorldFloor() - 158.f);
    }
}



Game::Game()
    : level(nullptr)
    , ui(nullptr)
    , previousState(GameState::MAINMENU)
    , gameOver(false)
{
    // Determine executable directory (PathUtils::getExecutableDir assumed)
    assetRoot = getExecutableDir();

    // Normalize assetRoot to ensure trailing slash
    if (!assetRoot.empty() && assetRoot.back() != '/' && assetRoot.back() != '\\') {
        assetRoot.push_back('/');
    }

    // Load settings from assetRoot/settings.txt (falls back to defaults internally)
    if (!this->settings.loadFromFile(assetRoot)) {
        std::cout << "WARNING::GAME::Using default settings\n";
    }

    // Initialize subsystems (order matters: window may use settings)
    this->initWindow();
    this->initUI();
    this->initTextures();
    this->initInput();
    this->initLevel();
    this->initPlayer();
}


Game::~Game() {
    delete this->input;
    delete this->player;
    delete this->level;
    delete this->ui;
}

void Game::updateInput() {
    float moveSpeed = 2.f;

    if (this->input->isAction("RUN"))
        moveSpeed = 6.f;

    float moveX = 0.f;

    if (this->input->isAction("MOVE_LEFT")) moveX -= moveSpeed + (this->player->timeSpeedPotion > 0) * 1;
    if (this->input->isAction("MOVE_RIGHT")) moveX += moveSpeed + (this->player->timeSpeedPotion > 0) * 3;

    this->player->move(moveX, 0.f);

    if (this->input->isAction("ATTACK") && !this->player->isAttacking())
        this->player->setAnimState(ATTACKING);

    // Potions (kept from colleague)
    if (this->input->isAction("HEALTH_POTION") && !this->player->isAttacking() && this->player->timeHealthPotion == 0) {
        if (this->player->getHealthPotions() > 0) {
            this->player->timeHealthPotion = clock();
            this->player->gainHealth(this->player->getMaxHealth() / 2);
            this->player->modHealthPotion(-1);
        }
    }
    if (this->input->isAction("SPEED_POTION") && !this->player->isAttacking() && this->player->timeSpeedPotion == 0) {
        if (this->player->getSpeedPotions() > 0) {
            this->player->timeSpeedPotion = clock();
            this->player->modSpeedPotion(-1);
        }
    }
    if (this->input->isAction("ATTACK_POTION") && !this->player->isAttacking() && this->player->timeAttackPotion == 0) {
        if (this->player->getAttackPotions() > 0) {
            this->player->timeAttackPotion = clock();
            this->player->modAttackPotion(-1);
        }
    }
}

void Game::updatePlayer() {
    this->player->update();
}

void Game::updateCollision() {
    constexpr float EPS = 0.001f;
    bool playerOnGround = false;

    // ---------- Player collision ----------
    {
        sf::FloatRect bounds = player->getGlobalBounds();
        sf::Vector2f prevPos = player->getPrevPosition();

        sf::FloatRect prevBounds = bounds;
        prevBounds.left = prevPos.x;
        prevBounds.top  = prevPos.y;

        for (const auto* platform : level->getPlatforms()) {
            if (!platform) continue;
            sf::FloatRect plat = platform->getGlobalBounds();

            if (!bounds.intersects(plat)) continue;

            float overlapLeft   = (bounds.left + bounds.width) - plat.left;
            float overlapRight  = (plat.left + plat.width) - bounds.left;
            float overlapTop    = (bounds.top + bounds.height) - plat.top;
            float overlapBottom = (plat.top + plat.height) - bounds.top;

            float penX = std::min(overlapLeft, overlapRight);
            float penY = std::min(overlapTop, overlapBottom);

            if (penX + EPS < penY) {
                // Horizontal collision
                if (prevPos.x + bounds.width <= plat.left + EPS) {
                    player->setPosition(plat.left - bounds.width, bounds.top);
                } else if (prevPos.x >= plat.left + plat.width - EPS) {
                    player->setPosition(plat.left + plat.width, bounds.top);
                } else {
                    if (overlapLeft < overlapRight)
                        player->setPosition(bounds.left - overlapLeft, bounds.top);
                    else
                        player->setPosition(bounds.left + overlapRight, bounds.top);
                }
                player->resetVelocityX();
            } else {
                // Vertical collision
                if (prevPos.y + bounds.height <= plat.top + EPS) {
                    // landed on top
                    player->setPosition(bounds.left, plat.top - bounds.height);
                    player->resetVelocityY();
                    playerOnGround = true;

                    bounds = player->getGlobalBounds();
                    break; // landing is final
                } else if (prevPos.y >= plat.top + plat.height - EPS) {
                    // hit from below
                    player->setPosition(bounds.left, plat.top + plat.height);
                    player->resetVelocityY();
                } else {
                    if (overlapTop < overlapBottom)
                        player->setPosition(bounds.left, bounds.top - overlapTop);
                    else
                        player->setPosition(bounds.left, bounds.top + overlapBottom);
                    player->resetVelocityY();
                }
            }
            bounds = player->getGlobalBounds();
        }
    }

    // ---------- Floor collision ----------
    {
        sf::FloatRect b = player->getGlobalBounds();
        float floorY = this->level->getWorldFloor();
        constexpr float floorEpsilon = 0.5f;

        if (b.top + b.height > floorY - floorEpsilon) {
            player->setPosition(b.left, floorY - b.height);
            player->resetVelocityY();
            playerOnGround = true;

            // SWAMP DAMAGE: only on newly landed frames
            if (!player->wasOnGroundLastFrame()) {
                // tweak damage amount as needed
                player->takeDamage(1);
            }
        }
    }

    player->setCanJump(playerOnGround);
    player->setOnGroundLastFrame(playerOnGround);

    // ---------- Enemy collision (unchanged) ----------
    for (auto* enemy : level->getEnemies()) {
        if (!enemy) continue;
        if (!enemy->isAlive()) continue;

        bool enemyOnGround = false;
        bool hitWall = false;

        sf::FloatRect bounds = enemy->getGlobalBounds();
        sf::Vector2f prevPos = enemy->getPrevPosition();
        sf::FloatRect prevBounds = bounds;
        prevBounds.left = prevPos.x;
        prevBounds.top  = prevPos.y;

        for (const auto* platform : level->getPlatforms()) {
            if (!platform) continue;
            sf::FloatRect plat = platform->getGlobalBounds();
            if (!bounds.intersects(plat)) continue;

            float overlapLeft   = (bounds.left + bounds.width) - plat.left;
            float overlapRight  = (plat.left + plat.width) - bounds.left;
            float overlapTop    = (bounds.top + bounds.height) - plat.top;
            float overlapBottom = (plat.top + plat.height) - bounds.top;

            float penX = std::min(overlapLeft, overlapRight);
            float penY = std::min(overlapTop, overlapBottom);

            if (penX + EPS < penY) {
                hitWall = true;
                if (prevPos.x + bounds.width <= plat.left + EPS) {
                    enemy->setPosition(plat.left - bounds.width, bounds.top);
                } else if (prevPos.x >= plat.left + plat.width - EPS) {
                    enemy->setPosition(plat.left + plat.width, bounds.top);
                } else {
                    if (overlapLeft < overlapRight)
                        enemy->setPosition(bounds.left - overlapLeft, bounds.top);
                    else
                        enemy->setPosition(bounds.left + overlapRight, bounds.top);
                }
                enemy->resetVelocityX();
            } else {
                if (prevPos.y + bounds.height <= plat.top + EPS) {
                    enemy->setPosition(bounds.left, plat.top - bounds.height);
                    enemy->resetVelocityY();
                    enemyOnGround = true;
                    bounds = enemy->getGlobalBounds();
                    break;
                } else if (prevPos.y >= plat.top + plat.height - EPS) {
                    enemy->setPosition(bounds.left, plat.top + plat.height);
                    enemy->resetVelocityY();
                } else {
                    if (overlapTop < overlapBottom)
                        enemy->setPosition(bounds.left, bounds.top - overlapTop);
                    else
                        enemy->setPosition(bounds.left, bounds.top + overlapBottom);
                    enemy->resetVelocityY();
                }
            }
            bounds = enemy->getGlobalBounds();
        }

        sf::FloatRect eb = enemy->getGlobalBounds();
        if (eb.top + eb.height > level->getWorldFloor()) {
            enemy->setPosition(eb.left, level->getWorldFloor() - eb.height);
            enemy->resetVelocityY();
            enemyOnGround = true;
        }

        if (hitWall && enemyOnGround) enemy->jump();
        enemy->setCanJump(enemyOnGround);
    }
}




void Game::updateView() {
    // Horizontal center follows player
    float centerX = this->player->getPosition().x + this->player->getGlobalBounds().width / 2.f;

    // Make camera vertical such that the bottom of the view sits near the world floor.
    // This ensures platforms (which are placed with y = worldFloor - height) are visible.
    float floor = this->level->getWorldFloor();

    // How many pixels of "ground margin" to show at the bottom (tweak as desired)
    const float groundMargin = 50.f;

    // Desired bottom coordinate of view (world coordinate)
    float desiredViewBottom = floor - groundMargin;

    // Compute center.y so that view bottom equals desiredViewBottom
    float centerY = desiredViewBottom - (this->view.getSize().y * 0.5f);

    // Prevent camera from moving above top of the level (optional)
    float minCenterY = this->view.getSize().y * 0.5f;
    if (centerY < minCenterY) centerY = minCenterY;

    this->view.setCenter(centerX, centerY);

    // Left boundary (keep existing behaviour)
    if (this->view.getCenter().x < 640.f) {
        this->view.setCenter(640.f, this->view.getCenter().y);
    }
}


void Game::updateCombat() {
    sf::FloatRect playerBounds = this->player->getGlobalBounds();
    sf::FloatRect attackBounds = this->player->getAttackBounds();
    bool isAttacking = this->player->isAttacking();
    bool isHitFrame = this->player->isInAttackHitFrame();

    for (auto* enemy : this->level->getEnemies()) {
        if (!enemy->isAlive())
            continue;

        sf::FloatRect enemyBounds = enemy->getGlobalBounds();

        if (isAttacking && isHitFrame && !this->player->hasAttackHit() &&
            attackBounds.intersects(enemyBounds)) {
            enemy->takeDamage(this->player->getAttack());
            this->player->setAttackHit();
            std::cout << "Hit enemy! Enemy health: " << enemy->getHealth() << "\n";
            if (enemy->getHealth() == 0)player->addPoints(enemy->getXP());
        }

        if (playerBounds.intersects(enemyBounds)) {
            this->player->takeDamage(enemy->getDamage());
        }
    }
}




void Game::update() {
    while (this->window.pollEvent(this->ev)) {
    if (this->ev.type == sf::Event::Closed)
        this->window.close();

    // Handle UI input
    GameState state = this->ui->getGameState();

    if (state == GameState::MAINMENU) {
        this->ui->handleMainMenuInput(this->ev);

        if (this->ev.type == sf::Event::KeyPressed && this->ev.key.code == sf::Keyboard::Return) {
            int selection = this->ui->getMenuSelection();
            switch (selection) {
                case 0: this->ui->setState(GameState::PLAYING); break;
                case 1: this->ui->setState(GameState::SETTINGS); this->previousState = GameState::MAINMENU; break;
                case 2: this->window.close(); break;
            }
        }
    }
    else if (state == GameState::PAUSED) {
        this->ui->handlePauseMenuInput(this->ev);

        if (this->ev.type == sf::Event::KeyPressed && this->ev.key.code == sf::Keyboard::Return) {
            int selection = this->ui->getPauseSelection();
            switch (selection) {
                case 0: this->ui->setState(GameState::PLAYING); break;
                case 1: this->ui->setState(GameState::MAINMENU); break;
                case 2: this->ui->setState(GameState::SETTINGS); this->previousState = GameState::PAUSED; break;
                case 3: this->window.close(); break;
            }
        }
    }
    else if (state == GameState::SETTINGS) {
        if (this->ui->getIsRebinding()) {
            if (this->ev.type == sf::Event::KeyPressed) {
                std::string action = this->ui->getSettingsOptions()[this->ui->getSettingsSelection()].getString();
                if (action != "BACK" && action != "----------") {
                    this->settings.setKeybind(action, this->ev.key.code);
                    this->settings.saveToFile();
                }
                this->ui->setIsRebinding(false);
            }
        } else {
            this->ui->handleSettingsMenuInput(this->ev);
            if (this->ev.type == sf::Event::KeyPressed && this->ev.key.code == sf::Keyboard::Return) {
                std::string opt = this->ui->getSettingsOptions()[this->ui->getSettingsSelection()].getString();
                if (opt == "BACK") this->ui->setState(this->previousState);
                else if (opt == "DIFFICULTY") this->settings.toggleDifficulty();
                else if (opt != "----------") this->ui->setIsRebinding(true);
            }
        }
    }
    else if (state == GameState::GAMEOVER) {
        this->ui->handleGameOverMenuInput(this->ev);
        if (this->ev.type == sf::Event::KeyPressed && this->ev.key.code == sf::Keyboard::Return) {
            int selection = this->ui->getGameOverSelection();
            if (selection == 0) this->window.close();
        }
    }

    // Only handle gameplay keys if PLAYING
    if (state == GameState::PLAYING) {
        if (this->ev.type == sf::Event::KeyPressed) {
            if (this->ev.key.code == this->settings.getKeybind("MENU"))
                this->ui->setState(GameState::PAUSED);
            else if (this->ev.key.code == this->settings.getKeybind("JUMP"))
                if (this->player->getCanJump()) this->player->jump();
        }
    }
}


    if (this->ui->getGameState() == GameState::PLAYING) {
        this->updateInput();
        this->updatePlayer();
        this->updateCollision();
        this->level->update(this->player->getPosition(), this->view);
        this->updateCombat();
        this->updateView();

        if (!this->player->isAlive())
            this->ui->setState(GameState::GAMEOVER);

        this->ui->updateHUD(
            this->player->getHealth(),
            this->player->getMaxHealth(),
            this->player->getPoints(),
            this->player->getLevel(),
            this->player->getHealthPotions(),
            this->player->getSpeedPotions(),
            this->player->getAttackPotions(),
            this->player->timeSpeedPotion,
            this->player->timeAttackPotion
        );
    }
}

void Game::renderPlayer() {
    this->player->render(this->window);
}

void Game::render() {
    // Clear with a dark color (visible in Main Menu)
    this->window.clear(sf::Color(20, 20, 30));

    GameState state = this->ui->getGameState();

    // LAYER 1: GAME WORLD (Only rendered during Playing or Paused)
    if (state == GameState::PLAYING || state == GameState::PAUSED) {
        // Render background on a static view (doesn't follow camera)
        this->window.setView(this->window.getDefaultView());
        this->window.draw(this->backgroundSprite);

        // Render level and player on the moving camera view
        this->window.setView(this->view);
        this->level->render(this->window);
        this->renderPlayer();
    }

    // LAYER 2: INTERFACE (Always on top, using default screen view)
    this->window.setView(this->window.getDefaultView());
    
    if (state == GameState::MAINMENU || state == GameState::SETTINGS || state == GameState::GAMEOVER) {
        // Fullscreen menus
        this->ui->render(this->window);
    } 
    else {
        // Gameplay HUD elements
        this->ui->renderHUD(this->window);
        
        if (state == GameState::PAUSED) {
            this->ui->renderPauseMenu(this->window);
        }
    }

    this->window.display();
}

const sf::RenderWindow& Game::getWindow() const {
    return this->window;
}

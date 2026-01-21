#pragma once

#include "Player.h"
#include "Input.h"
#include "Level.h"
#include "Platform.h"
#include "Enemy.h"
#include "UI.h"
#include "Settings.h"
#include "PathUtils.h"

class Game {
private:
    // Window and Rendering
    sf::RenderWindow window;
    sf::Event ev;
    sf::View view;

    // Assets
    sf::Texture playerTextureSheet;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    std::string assetRoot;

    // Logic and State
    GameState previousState; // Used to remember if we came from Pause or Main Menu
    bool gameOver;

    // Systems
    Settings settings;
    UI* ui;
    Player* player;
    Input* input;
    Level* level;

    // Initialization helpers
    void initWindow();
    void initUI();
    void initTextures();
    void initInput();
    void initLevel();
    void initPlayer();

public:
    // Constructors / Destructors
    Game();
    virtual ~Game();

    // Accessors
    const sf::RenderWindow& getWindow() const;
    inline bool getGameOver() const { return this->gameOver; }

    // Core Game Loop Functions
    void updateInput();
    void updatePlayer();
    void updateCollision();
    void updateView();
    void updateCombat();

    void update(); // Main update router
    void render(); // Main render router
    void renderPlayer();
};
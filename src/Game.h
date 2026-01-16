#pragma once

#include "Player.h"
#include "Input.h"
#include "Level.h"
#include "UI.h"
#include "PathUtils.h"

class Game{
    private:
        // Core

        sf::RenderWindow window;
        sf::Event ev;
        sf::View view;
        sf::Texture playerTextureSheet;
        bool gameOver = false;
        sf::Texture backgroundTexture;
        sf::Sprite backgroundSprite;
        // sf::Font font;
        // sf::Text gameOverText;

        std::string assetRoot;

        // Class

        UI* ui;
        Player* player;
        Input* input;
        Level* level;
        
        // Initialization

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
        
        // Inline functions

        inline bool getGameOver() const { return this->gameOver; };

        // Functions

        void updateInput();
        void updatePlayer();
        void updateCollision();
        void updateView();
        void updateCombat();
        
        void update();

        void renderPlayer();

        void render();



};
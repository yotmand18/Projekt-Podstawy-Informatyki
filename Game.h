#pragma once

#include "Player.h"
#include "Input.h"
#include "Level.h"

class Game{
    private:
        // Core

        sf::RenderWindow window;
        sf::Event ev;
        sf::View view;
        sf::Texture playerTextureSheet;

        // Class

        Player* player;
        Input* input;
        Level* level;
        
        // Initialization

        void initWindow();
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

        // Functions

        void updateInput();
        void updatePlayer();
        void updateCollision();
        void updateView();
        
        void update();

        void renderPlayer();

        void render();



};
#pragma once

#include "lib.h"

enum class GameState {
    MAINMENU,
    PLAYING,
    PAUSED,
    GAMEOVER,
    VICTORY
};

class UI{
    private:
        // Core
        sf::Font font;
        GameState currentState;

        std::string assetRoot;

        // Defaults

        unsigned characterTitle = 100;
        unsigned characterSecondary = 50;
        unsigned characterHUD = 25;

        
        sf::Color colorNormal = sf::Color(220, 220, 220);      // Light gray
        sf::Color colorSelected = sf::Color(255, 215, 0);      // Gold
        sf::Color colorTitle = sf::Color(180, 180, 180);       // Silver
        sf::Color colorBackground = sf::Color(20, 20, 30);     // Dark blue-ish
        sf::Color colorHealth = sf::Color(200, 50, 50);        // Dark red
        sf::Color colorScore = sf::Color(255, 215, 0);        // Dark red
        sf::Color colorHpPotions= sf::Color(244, 111, 34);        // orange
        sf::Color colorAttackPotions = sf::Color(23, 212, 215);        // Cyan
        sf::Color colorSpeedPotions = sf::Color(148, 48, 187);         // Purple


        // Main Menu

        sf::Text menuTitle;
        std::vector<sf::Text> menuOptions;
        int menuSelectedOption;

        // HUD

        sf::Text healthText;
        sf::Text scoreText;
        sf::Text levelText;
        sf::Text HealthPotions;
        sf::Sprite HealthPotionsIcon;
        sf::Text SpeedPotions;
        sf::Sprite SpeedPotionsIcon;
        sf::Text AttackPotions;
        sf::Sprite AttackPotionsIcon;

        // Pause Manu
        sf::Text pausedTitle;
        std::vector<sf::Text> pauseOptions;
        int pauseSelectedOption;

        // GameOver Menu
        sf::Text gameOverTitle;
        std::vector <sf::Text> gameOverOptions;
        int gameOverSelectedOption;

        // Victory Menu
        sf::Text victoryTitle;
        sf::Text finalScoreText;

        // Initialization

        void initFont();
        void initMainMenu();
        void initHUD();
        void initPauseMenu();
        void initGameOverMenu(bool didWin);

    public:
        // Constructors / Destructors
        explicit UI(const std::string& assetRoot);
        virtual ~UI();

        // Accessors
        inline GameState getGameState() const { return this->currentState; };
        inline int getMenuSelection() const { return this->menuSelectedOption; };
        inline int getPauseSelection() const { return this->pauseSelectedOption; };
        inline int getGameOverSelection() const { return this->gameOverSelectedOption; };

        // Modifiers
        inline void setState(GameState state) { this->currentState = state; };

        // Event handling 

        void handleMainMenuInput(const sf::Event& ev);
        void handlePauseMenuInput(const sf::Event& ev);
        void handleGameOverMenuInput(const sf::Event& ev);

        // Update

        void updateHUD(int health, int maxHealth, int score, int level, int Health_Potions, int Speed_Potions, int Attack_Potions);
        void update();

        // Render
        void renderMainMenu(sf::RenderTarget& target);
        void renderHUD(sf::RenderTarget& target);
        void renderPauseMenu(sf::RenderTarget& target);
        void renderGameOverMenu(sf::RenderTarget& target);
        void renderVictoryMenu(sf::RenderTarget& target);
        void render(sf::RenderTarget& target);
};
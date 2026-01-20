#pragma once

#include "lib.h"

enum class GameState {
    MAINMENU,
    SETTINGS,
    PLAYING,
    PAUSED,
    GAMEOVER,
    VICTORY
};

class Settings;

class UI{
    private:
        // Core
        sf::Font font;
        GameState currentState;

        // Class include

        Settings* settings;

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
        void initMainMenu();

        // Settings Menu

        sf::Text settingsTitle;
        std::vector<sf::Text> settingsOptions;
        int settingsSelectedOption;
        void initSettingsMenu();
        int settingsOffset = 0;
        const int maxVisibleOptions = 5;
        bool isRebinding = false;

        // HUD

        sf::Text healthText;
        sf::Text scoreText;
        sf::Text levelText;

        void initHUD();        
        
        sf::Text HealthPotions;
        sf::Sprite HealthPotionsIcon;
        sf::Text SpeedPotions;
        sf::Text timeSpeedPotion;
        sf::Sprite SpeedPotionsIcon;
        sf::Text AttackPotions;
        sf::Text timeAttackPotion;
        sf::Sprite AttackPotionsIcon;


        // Pause Manu
        sf::Text pausedTitle;
        std::vector<sf::Text> pauseOptions;
        int pauseSelectedOption;
        void initPauseMenu();

        // GameOver Menu
        sf::Text gameOverTitle;
        std::vector <sf::Text> gameOverOptions;
        int gameOverSelectedOption;
        void initGameOverMenu(bool didWin);

        // Victory Menu
        sf::Text victoryTitle;
        sf::Text finalScoreText;

        // Initialization

        void initFont();

    public:
        // Constructors / Destructors
        explicit UI(const std::string& assetRoot);
        virtual ~UI();

        // Accessors
        inline GameState getGameState() const { return this->currentState; };
        inline int getMenuSelection() const { return this->menuSelectedOption; };
        inline int getSettingsSelection() const { return this->settingsSelectedOption; };
        inline int getPauseSelection() const { return this->pauseSelectedOption; };
        inline int getGameOverSelection() const { return this->gameOverSelectedOption; };

        // Settings
        inline bool getIsRebinding() const { return isRebinding; };
        inline void setIsRebinding(bool value) { isRebinding = value; };
        inline std::vector<sf::Text>& getSettingsOptions() { return this->settingsOptions; };

        // Modifiers
        inline void setState(GameState state) { this->currentState = state; };

        // Functions

        void setSettings(Settings* settings);
        void navigateMenu(std::vector<sf::Text>& options, int& selected, const sf::Event& ev);

        // Event handling 

        void handleMainMenuInput(const sf::Event& ev);
        void handleSettingsMenuInput(const sf::Event& ev);
        void handlePauseMenuInput(const sf::Event& ev);
        void handleGameOverMenuInput(const sf::Event& ev);

        
        // Update

        void updateHUD(int health, int maxHealth, int score, int level, int Health_Potions, int Speed_Potions, int Attack_Potions, int timeSpeed, int timeAttack);
        void update();

        // Render
        void renderMainMenu(sf::RenderTarget& target);
        void renderSettingsMenu(sf::RenderTarget& target);
        void renderHUD(sf::RenderTarget& target);
        void renderPauseMenu(sf::RenderTarget& target);
        void renderGameOverMenu(sf::RenderTarget& target);
        void renderVictoryMenu(sf::RenderTarget& target);
        void render(sf::RenderTarget& target);
};
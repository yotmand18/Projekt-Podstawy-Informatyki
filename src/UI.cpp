#include "UI.h"
#include "Settings.h"

// Initialization

void UI::initFont(){
    if(!this->font.loadFromFile("./fonts/thewitcher-font/thewitcher-font.ttf")){
        std::cout << "ERROR::UI::Failed to load font\n";
    }
}

void UI::initMainMenu(){
    this->menuSelectedOption = 0;

    this->menuTitle.setFont(this->font);
    this->menuTitle.setString("THE WITCHER");
    this->menuTitle.setCharacterSize(this->characterTitle);
    this->menuTitle.setFillColor(this->colorTitle);
    this->menuTitle.setPosition(350.f, 100.f);

    // Menu options
    std::vector<std::string> options = { "New Game", "Continue", "Exit" };
    
    for(size_t i = 0; i < options.size(); i++){
        sf::Text text;
        text.setFont(this->font);   
        text.setString(options[i]);
        text.setCharacterSize(this->characterSecondary);
        text.setFillColor(this->colorNormal);
        text.setPosition(500.f, 300.f + i * 80.f);
        
        this->menuOptions.push_back(text);
    }
    
    // Highlight first option
    this->menuOptions[0].setFillColor(this->colorSelected);
}

void UI::initSettingsMenu(){
    this->menuSelectedOption = 0;

    this->menuTitle.setFont(this->font);
    this->menuTitle.setString("SETTINGS");
    this->menuTitle.setCharacterSize(this->characterTitle);
    this->menuTitle.setFillColor(this->colorTitle);
    this->menuTitle.setPosition(350.f, 100.f);

    // Menu options
    std::vector<std::string> options = { "Resolution", "Difficilty", "Move Left", "Move Right", "Jump", "Attack", "Run", "Menu" };
    std::vector<std::string> currentSettings;
    
    for(size_t i = 0; i < options.size(); i++){
        sf::Text text;
        text.setFont(this->font);   
        text.setString(options[i]);
        text.setCharacterSize(this->characterSecondary);
        text.setFillColor(this->colorNormal);
        text.setPosition(500.f, 300.f + i * 80.f);
        
        this->menuOptions.push_back(text);
    }
    
    // Highlight first option
    this->menuOptions[0].setFillColor(this->colorSelected);
}


void UI::initHUD(){
    healthText.setFont(this->font);   
    healthText.setCharacterSize(this->characterHUD);
    healthText.setFillColor(this->colorHealth);
    healthText.setPosition(1000.f, 0.f);

    scoreText.setFont(this->font);   
    scoreText.setCharacterSize(this->characterHUD);
    scoreText.setFillColor(this->colorScore);
    scoreText.setPosition(1000.f, 25.f);

    levelText.setFont(this->font);   
    levelText.setCharacterSize(this->characterHUD);
    levelText.setFillColor(this->colorNormal);
    levelText.setPosition(1000.f, 50.f);
}

void UI::initPauseMenu(){
    this->pauseSelectedOption = 0;

    // Title - PAUSED
    this->pausedTitle.setFont(this->font);
    this->pausedTitle.setString("PAUSED");
    this->pausedTitle.setCharacterSize(this->characterTitle);
    this->pausedTitle.setFillColor(this->colorTitle);
    this->pausedTitle.setPosition(350.f, 100.f);

    // Menu options
    std::vector<std::string> options = { "Continue", "Main Menu", "Exit" };
    
    for(size_t i = 0; i < options.size(); i++){
        sf::Text text;
        text.setFont(this->font);   
        text.setString(options[i]);
        text.setCharacterSize(this->characterSecondary);
        text.setFillColor(this->colorNormal);
        text.setPosition(500.f, 300.f + i * 80.f);
        
        this->pauseOptions.push_back(text);
    }

    // Highlight first option
    this->pauseOptions[0].setFillColor(this->colorSelected);
}

void UI::initGameOverMenu(bool didWin){
    this->gameOverSelectedOption = 0;

    if(didWin){
        // Title - VICTORY
        this->gameOverTitle.setString("GAME OVER");
    }else{
        // Title - GAME OVER
        this->gameOverTitle.setString("GAME OVER");
    }

    // The other values are shared
    this->gameOverTitle.setFont(this->font);
    this->gameOverTitle.setCharacterSize(this->characterTitle);
    this->gameOverTitle.setFillColor(this->colorTitle);
    this->gameOverTitle.setPosition(350.f, 100.f);

    // Menu options
    std::vector<std::string> options = { "Main Menu", "Exit" };
    
    for(size_t i = 0; i < options.size(); i++){
        sf::Text text;
        text.setFont(this->font);   
        text.setString(options[i]);
        text.setCharacterSize(this->characterSecondary);
        text.setFillColor(this->colorNormal);
        text.setPosition(500.f, 300.f + i * 80.f);
        
        this->gameOverOptions.push_back(text);
    }

    // Highlight first option
    this->gameOverOptions[0].setFillColor(this->colorSelected);
}

// Constructors / Destructors

UI::UI(){
    this->currentState = GameState::MAINMENU;

    this->initFont();
    this->initMainMenu();
    this->initHUD();
    this->initPauseMenu();
    this->initGameOverMenu(this->currentState == GameState::VICTORY);
}

UI::~UI(){


}

// Functions

void UI::setSettings(Settings* settings){

}

void UI::navigateMenu(std::vector<sf::Text>& options, int& selected, const sf::Event& ev){
    if (ev.type == sf::Event::KeyPressed) {
        
        options[selected].setFillColor(sf::Color::White);

        if (ev.key.code == sf::Keyboard::Up) {
            selected--;
            if (selected < 0) 
                selected = options.size() - 1;
        } 
        else if (ev.key.code == sf::Keyboard::Down) {
            selected++;
            if (selected >= (int)options.size()) 
                selected = 0; // Pełne zawijanie
        }

        options[selected].setFillColor(this->colorSelected);
    }
}

// Event handling

void UI::handleMainMenuInput(const sf::Event& ev){
    navigateMenu(this->menuOptions, this->menuSelectedOption, ev);
}

void UI::handleSettingsMenuInput(const sf::Event& ev){
    navigateMenu(this->settingsOptions, this->settingsSelectedOptions, ev);
}

void UI::handlePauseMenuInput(const sf::Event& ev){
    navigateMenu(this->pauseOptions, this->pauseSelectedOption, ev);
}

void UI::handleGameOverMenuInput(const sf::Event& ev){
    navigateMenu(this->gameOverOptions, this->gameOverSelectedOption, ev);
}

// Update

void UI::updateHUD(int health, int maxHealth, int score, int level){
    this->healthText.setString("Health: " + std::to_string(health) + "/" + std::to_string(maxHealth));
    this->scoreText.setString("Score: " + std::to_string(score));
    this->levelText.setString("Level: " + std::to_string(level)); 
}

void UI::update(){

}

// Render

void UI::renderMainMenu(sf::RenderTarget& target){
    target.draw(this->menuTitle);

    for(auto& option : this->menuOptions)
        target.draw(option);
}

void UI::renderHUD(sf::RenderTarget& target){
    target.draw(this->healthText);
    target.draw(this->scoreText);
    target.draw(this->levelText);
}

void UI::renderPauseMenu(sf::RenderTarget& target){
    // Darken background
    sf::RectangleShape overlay(sf::Vector2f(1280.f, 720.f));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));  // Semi-transparent black
    target.draw(overlay);

    target.draw(this->pausedTitle);

    for(auto& option : this->pauseOptions)
        target.draw(option);
}

void UI::renderGameOverMenu(sf::RenderTarget& target){
    // Background
    sf::RectangleShape bg(sf::Vector2f(1280.f, 720.f));
    bg.setFillColor(sf::Color(20, 20, 30));
    target.draw(bg);

    target.draw(this->gameOverTitle);

    for(auto& option : this->gameOverOptions)
        target.draw(option);
}

void UI::render(sf::RenderTarget& target){
    switch(this->currentState){
        case GameState::MAINMENU:
            this->renderMainMenu(target);
            break;
            
        case GameState::PAUSED:
            this->renderPauseMenu(target);
            break;
            
        case GameState::GAMEOVER:
        case GameState::VICTORY:
            this->renderGameOverMenu(target);
            break;
            
        case GameState::PLAYING:
            // HUD is rendered on top of game, handle in Game.cpp
            break;
    }
}
#include "Settings.h"

// Constructors / Destructors

Settings::Settings(){

}

Settings::~Settings(){


}

// Accessors

sf::Keyboard::Key Settings::getKeybind(const std::string& action) const {
    auto it = keybinds.find(action);
    if (it != keybinds.end())
        return it->second;
    return sf::Keyboard::Unknown;
}

// Functions

bool Settings::loadFromFile(){
    std::ifstream file("settings.txt");
    if(!file.is_open()){
        std::cout << "ERROR::SETTINGS::Could not open settings.txt\n";
        return false;
    }
    
    std::string line;
    while(std::getline(file, line)){
        if(line.empty() || line[0] == '#')
            continue;
        
        std::istringstream iss(line);
        std::string setting;
        
        if(iss >> setting){
            if(setting == "resolution"){
                // reading resolution
                unsigned x, y;
                if(iss >> x >> y){
                    this->resolution.x = x;
                    this->resolution.y = y;
                }else
                    std::cout << "ERROR:SETTINGS::Failed parsing file settings.txt\n"; 
                
            } else if(setting == "difficulty"){
                // reading difficulty
                std::string dif;
                if(iss >> dif){
                    if(dif == "easy")
                        currentDifficulty = Difficulty::EASY;
                    else if(dif == "normal")
                        currentDifficulty = Difficulty::NORMAL;
                    else if(dif == "hard")
                        currentDifficulty = Difficulty::HARD;
                    else
                        std::cout << "ERROR:SETTINGS::DIFFICULTY::Incorrect value assigned settings.txt";
                }else
                    std::cout << "ERROR:SETTINGS::Failed parsing file settings.txt\n"; 
                
            } else if (setting == "bind") {
                // reading keybindings
                std::string bindKeyName;
                int bindKey;

                if (iss >> bindKeyName >> bindKey) {
                    if (bindKey >= sf::Keyboard::Unknown && bindKey < sf::Keyboard::KeyCount)
                        this->keybinds[bindKeyName] = static_cast<sf::Keyboard::Key>(bindKey);
                    else 
                        std::cout << "ERROR::SETTINGS::Invalid key id\n";
                } else 
                    std::cout << "ERROR::SETTINGS::Failed parsing keybind\n";
                
            }
        }
    }
    
    file.close();
    return true;
}
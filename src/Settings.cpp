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

void Settings::toggleResolution() {
    // Prosta rotacja między popularnymi rozdzielczościami
    if (this->resolution.x == 1280) {
        this->resolution = sf::Vector2u(1600, 900);
    } else if (this->resolution.x == 1600) {
        this->resolution = sf::Vector2u(1920, 1080);
    } else {
        this->resolution = sf::Vector2u(1280, 720);
    }
    this->saveToFile();
}

void Settings::toggleDifficulty() {
    if (this->currentDifficulty == Difficulty::EASY)
        this->currentDifficulty = Difficulty::NORMAL;
    else if (this->currentDifficulty == Difficulty::NORMAL)
        this->currentDifficulty = Difficulty::HARD;
    else
        this->currentDifficulty = Difficulty::EASY;
    this->saveToFile();
}

bool Settings::loadFromFile(const std::string& assetRoot){
    std::ifstream file(assetRoot + "/settings/settings.txt");
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
                std::string bindKeyName;
                int bindKey = -1; // Inicjalizacja wartością kontrolną

                // Czyścimy flagi stanu iss przed czytaniem (na wszelki wypadek)
                iss >> bindKeyName;
                if (iss >> bindKey) {
                    if (bindKey >= 0 && bindKey < sf::Keyboard::KeyCount) {
                        this->keybinds[bindKeyName] = static_cast<sf::Keyboard::Key>(bindKey);
                        // DEBUG: sprawdź czy faktycznie wchodzi tutaj więcej niż raz
                        // std::cout << "DEBUG: Zbinowano " << bindKeyName << " jako " << bindKey << std::endl;
                    } else {
                        std::cout << "ERROR::SETTINGS::Invalid key id: " << bindKey << "\n";
                    }
                } else {
                    std::cout << "ERROR::SETTINGS::Failed parsing keybind value for: " << bindKeyName << "\n";
                }
            }
        }
    }
    
    file.close();
    return true;
}

void Settings::saveToFile() {
    std::ofstream file(this->assetRoot + "/settings/settings.txt");
    if (file.is_open()) {
        file << "resolution " << resolution.x << " " << resolution.y << "\n";
        file << "difficulty " << (currentDifficulty == Difficulty::EASY ? "easy" : 
                                   currentDifficulty == Difficulty::NORMAL ? "normal" : "hard") << "\n\n";
        file << "# keybinds\n";
        for (auto const& [name, key] : keybinds) {
            file << "bind " << name << " " << static_cast<int>(key) << "\n";
        }
        file.close();
    }
}

std::string Settings::keyToString(sf::Keyboard::Key key) {
    // Klawisze A-Z
    if (key >= sf::Keyboard::A && key <= sf::Keyboard::Z) 
        return std::string(1, 'A' + (key - sf::Keyboard::A));
    
    // Klawisze 0-9 (nad literami)
    if (key >= sf::Keyboard::Num0 && key <= sf::Keyboard::Num9) 
        return std::string(1, '0' + (key - sf::Keyboard::Num0));

    // Klawisze F1-F12
    if (key >= sf::Keyboard::F1 && key <= sf::Keyboard::F12)
        return "F" + std::to_string(key - sf::Keyboard::F1 + 1);

    // Numpad 0-9
    if (key >= sf::Keyboard::Numpad0 && key <= sf::Keyboard::Numpad9)
        return "Num " + std::to_string(key - sf::Keyboard::Numpad0);

    // Pozostałe klawisze specjalne
    switch(key) {
        case sf::Keyboard::LControl:  return "L-Ctrl";
        case sf::Keyboard::RControl:  return "R-Ctrl";
        case sf::Keyboard::LShift:    return "L-Shift";
        case sf::Keyboard::RShift:    return "R-Shift";
        case sf::Keyboard::LAlt:      return "L-Alt";
        case sf::Keyboard::RAlt:      return "R-Alt";
        case sf::Keyboard::LSystem:   return "L-Win";
        case sf::Keyboard::RSystem:   return "R-Win";
        case sf::Keyboard::Space:     return "Space";
        case sf::Keyboard::Enter:     return "Enter";
        case sf::Keyboard::Backspace: return "Backsp";
        case sf::Keyboard::Tab:       return "Tab";
        case sf::Keyboard::Escape:    return "Esc";
        case sf::Keyboard::Pause:     return "Pause";
        
        case sf::Keyboard::Insert:    return "Ins";
        case sf::Keyboard::Delete:    return "Del";
        case sf::Keyboard::Home:      return "Home";
        case sf::Keyboard::End:       return "End";
        case sf::Keyboard::PageUp:    return "PgUp";
        case sf::Keyboard::PageDown:  return "PgDn";

        case sf::Keyboard::Up:        return "Up";
        case sf::Keyboard::Down:      return "Down";
        case sf::Keyboard::Left:      return "Left";
        case sf::Keyboard::Right:     return "Right";

        case sf::Keyboard::Add:       return "+ (Num)";
        case sf::Keyboard::Subtract:  return "- (Num)";
        case sf::Keyboard::Multiply:  return "* (Num)";
        case sf::Keyboard::Divide:    return "/ (Num)";

        case sf::Keyboard::SemiColon: return ";";
        case sf::Keyboard::Comma:     return ",";
        case sf::Keyboard::Period:    return ".";
        case sf::Keyboard::Quote:     return "'";
        case sf::Keyboard::Slash:     return "/";
        case sf::Keyboard::BackSlash: return "\\";
        case sf::Keyboard::Tilde:     return "~";
        case sf::Keyboard::Equal:     return "=";
        case sf::Keyboard::Dash:      return "-";
        case sf::Keyboard::LBracket:  return "[";
        case sf::Keyboard::RBracket:  return "]";

        case sf::Keyboard::Unknown:   return "Unknown";
        default:                      return "ID: " + std::to_string(static_cast<int>(key));
    }
}
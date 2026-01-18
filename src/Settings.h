#pragma once

#include "lib.h"

enum class Difficulty{
    EASY,
    NORMAL,
    HARD
};

class Settings{
    private:
        // Core
        sf::Vector2u resolution;
        std::map<std::string, sf::Keyboard::Key> keybinds;
        Difficulty currentDifficulty;

        // Initialization

    public:
        // Constructors / Destructors
        Settings();
        virtual ~Settings();

        // Accessors
        inline sf::Vector2u getResolution() { return resolution; };
        inline Difficulty getDifficulty() { return currentDifficulty; };
        sf::Keyboard::Key getKeybind(const std::string& action) const;

        // Functions

        bool loadFromFile();

        // Update
        //void updateKeybinds(std::map<std::string>, sf::Keyboard::Key&);

};
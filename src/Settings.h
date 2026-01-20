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

        std::string assetRoot;

        // Initialization

    public:
        // Constructors / Destructors
        Settings();
        virtual ~Settings();

        // Accessors
        inline sf::Vector2u getResolution() { return resolution; };
        inline Difficulty getDifficulty() { return currentDifficulty; };
        sf::Keyboard::Key getKeybind(const std::string& action) const;
        const std::map<std::string, sf::Keyboard::Key>& getKeybinds() const { return keybinds; }

        // Modifiers

        inline void setKeybind(const std::string& action, sf::Keyboard::Key key) { this->keybinds[action] = key; };

        // Functions

        void toggleResolution();
        void toggleDifficulty();


        bool loadFromFile(const std::string& assetRoot);
        void saveToFile();
        static std::string keyToString(sf::Keyboard::Key key);
        

        // Update
        //void updateKeybinds(std::map<std::string>, sf::Keyboard::Key&);

};
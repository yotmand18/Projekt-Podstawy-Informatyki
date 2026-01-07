#pragma once

#include "lib.h"
#include "Platform.h"
//#include "Enemy.h"

class Level{
    private:
        // Core

        std::vector<Platform*> platforms;
        //std::vector<Enemy*> enemies;
        std::map<std::string, sf::Texture*> textures;

        void LoadTextures();

    public:
        // Constructors / Destructors

        Level();
        virtual ~Level();

        // Accessors
        
        const std::vector<Platform*>& getPlatforms() const;

        // Functions

        bool loadFromFile(const std::string& filename);

        void render(sf::RenderTarget& target);
        
};
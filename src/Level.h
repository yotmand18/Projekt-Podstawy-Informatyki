#pragma once

#include "Platform.h"
#include "Enemy.h"
#include <vector>
#include <string>
#include <map>

class Level {
private:

    std::vector<Platform*> platforms;
    std::vector<Enemy*> enemies;
    std::map<std::string, sf::Texture*> textures;

    std::string assetRoot;

    void LoadTextures();

public:

    explicit Level(const std::string& assetRoot);
    virtual ~Level();

    const std::vector<Platform*>& getPlatforms() const;
    const std::vector<Enemy*>& getEnemies() const;

    bool loadFromFile(const std::string& filename);

    void generateRandomLevel(float length, int difficulty);

    void update(const sf::Vector2f& playerPos, const sf::View& view);
    void render(sf::RenderTarget& target);
};

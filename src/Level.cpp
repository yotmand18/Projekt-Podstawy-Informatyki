#include "Level.h"
#include <fstream>
#include <sstream>
#include <algorithm> 
#include <iostream>
#include <ctime>   
#include <cstdlib> 

std::string trim(const std::string& str) {
    std::string s = str;
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
        }).base(), s.end());
    return s;
}

Level::Level(const std::string& root)
    : assetRoot(root)
{
    this->LoadTextures();
}

Level::~Level() {
    for (auto* platform : platforms) {
        delete platform;
    }
    for (auto* enemy : enemies) {
        delete enemy;
    }
    for (auto& pair : textures) {
        delete pair.second;
    }
}

void Level::LoadTextures() {
    // Tekstury Platform
    std::vector<std::string> platformTextures = { "rock1", "rock2" };
    for (const auto& name : platformTextures) {
        sf::Texture* tex = new sf::Texture();
        std::string path = assetRoot + "textures/platforms/" + name + ".png";

        if (tex->loadFromFile(path)) {
            tex->setSmooth(false);
            textures[name] = tex;
        }
        else {
            std::cout << "ERROR::LEVEL::Could not load texture: " << name << "\n";
            delete tex;
        }
    }

    std::vector<std::string> enemyTextures = { "drowner", "nekker", "ghoul", "wraith", "griffin", "ghost" };
    for (const auto& name : enemyTextures) {
        sf::Texture* tex = new sf::Texture();
        std::string path = assetRoot + "textures/enemies/" + name + ".png";

        if (tex->loadFromFile(path)) {
            tex->setSmooth(false);
            textures[name] = tex;
        }
        else {
            std::cout << "ERROR::LEVEL::Could not load enemy texture: " << name << "\n";
            delete tex;
        }
    }
}

bool Level::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "ERROR::LEVEL::Could not open level file: " << filename << "\n";
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#')
            continue;

        std::istringstream iss(line);
        std::string type;

        if (iss >> type) {
            if (type == "platform") {
                float x, y, width, height;
                std::string textureName;

                if (iss >> x >> y >> width >> height >> textureName) {
                    textureName = trim(textureName);

                    sf::Texture* tex = nullptr;
                    auto it = textures.find(textureName);
                    if (it != textures.end())
                        tex = it->second;
                    else
                        std::cout << "WARNING::LEVEL::Texture not found: [" << textureName << "]\n";

                    platforms.push_back(new Platform(x, y, width, height, tex));
                }

            }
            else if (type == "enemy") {
                float x, y, width, height, patrolLeft, patrolRight;
                std::string enemyTypeName;

                if (iss >> x >> y >> width >> height >> enemyTypeName >> patrolLeft >> patrolRight) {
                    enemyTypeName = trim(enemyTypeName);

                    ENEMY_TYPE enemyType;
                    if (enemyTypeName == "drowner") enemyType = DROWNER;
                    else if (enemyTypeName == "nekker") enemyType = NEKKER;
                    else if (enemyTypeName == "ghoul") enemyType = GHOUL;
                    else if (enemyTypeName == "wraith") enemyType = WRAITH;
                    else if (enemyTypeName == "griffin") enemyType = GRIFFIN;
                    else {
                        std::cout << "WARNING::LEVEL::Unknown enemy type: [" << enemyTypeName << "]\n";
                        continue;
                    }

                    sf::Texture* tex = nullptr;
                    auto it = textures.find(enemyTypeName);
                    if (it != textures.end())
                        tex = it->second;
                    else
                        std::cout << "WARNING::LEVEL::Enemy texture not found: " << enemyTypeName << "\n";

                    enemies.push_back(new Enemy(x, y, enemyType, tex, patrolLeft, patrolRight));
                }
            }
        }
    }

    file.close();
    return true;
}

const std::vector<Platform*>& Level::getPlatforms() const {
    return this->platforms;
}

const std::vector<Enemy*>& Level::getEnemies() const {
    return this->enemies;
}

void Level::update(const sf::Vector2f& playerPos, const sf::View& view) {
    float updateMargin = 500.f;
    sf::FloatRect updateBounds(
        view.getCenter().x - view.getSize().x / 2.f - updateMargin,
        view.getCenter().y - view.getSize().y / 2.f - updateMargin,
        view.getSize().x + updateMargin * 2.f,
        view.getSize().y + updateMargin * 2.f
    );

    for (auto* enemy : enemies) {
        if (enemy->getGlobalBounds().intersects(updateBounds)) {
            enemy->updatePlayerPosition(playerPos);
            enemy->update();
        }
    }
}

void Level::render(sf::RenderTarget& target) {
    sf::View view = target.getView();
    float renderMargin = 100.f;
    sf::FloatRect viewBounds(
        view.getCenter().x - view.getSize().x / 2.f - renderMargin,
        view.getCenter().y - view.getSize().y / 2.f - renderMargin,
        view.getSize().x + renderMargin * 2.f,
        view.getSize().y + renderMargin * 2.f
    );

    for (auto* platform : platforms) {
        if (platform->getBounds().intersects(viewBounds)) {
            platform->render(target);
        }
    }

    for (auto* enemy : enemies) {
        if (enemy->getGlobalBounds().intersects(viewBounds)) {
            enemy->render(target);
        }
    }
}

void Level::generateRandomLevel(float length, int difficulty)
{

    for (auto* p : platforms) delete p;
    platforms.clear();
    for (auto* e : enemies) delete e;
    enemies.clear();

    float currentX = 0.f;
    float currentY = 600.f;
    float maxX = length;

    sf::Texture* startTex = textures["rock1"];
    platforms.push_back(new Platform(0, 600, 600, 200, startTex));
    currentX += 600;

    while (currentX < maxX) {
        float gap = 50.f + (rand() % (100 + difficulty * 10));

        float heightChange = (rand() % 350) - 250.f;

        currentY += heightChange;

        if (currentY > 650.f) currentY = 650.f;

        if (currentY < -10000.f) currentY = -10000.f;

        float width = 200.f + (rand() % 400);
        float height = 150.f;

        std::string texName = (rand() % 2 == 0) ? "rock1" : "rock2";
        sf::Texture* tex = textures[texName];

        platforms.push_back(new Platform(currentX + gap, currentY, width, height, tex));

        if (width > 250.f && (rand() % 100) < (20 + difficulty * 5)) {

            int enemyRoll = rand() % 5;
            ENEMY_TYPE type;
            std::string enemyTexName;

            switch (enemyRoll) {
            case 0: type = DROWNER; enemyTexName = "drowner"; break;
            default: type = GHOUL; enemyTexName = "ghoul"; break;
            }
            if (textures.count(enemyTexName))
                enemies.push_back(new Enemy(currentX + gap + width / 2, currentY - 50, type, textures[enemyTexName], currentX + gap, currentX + gap + width));
        }

        currentX += gap + width;
    }
}

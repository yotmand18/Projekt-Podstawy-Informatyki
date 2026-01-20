#include "Level.h"
#include <fstream>
#include <sstream>

// Constructors / Destructors
Level::Level(const std::string& root)
    : assetRoot(root)
{
    this->LoadTextures();
}


Level::~Level(){
    for (auto* platform : platforms) {
        delete platform;
    }
    for (auto* enemy : enemies) {  // ADD THIS
        delete enemy;
    }
    for (auto& pair : textures) {
        delete pair.second;
    }
}

// Core
void Level::LoadTextures(){
    // Platform textures
    std::vector<std::string> platformTextures = {"rock1", "rock2"};
    for(const auto& name : platformTextures) {
        sf::Texture* tex = new sf::Texture();
        if(tex->loadFromFile(assetRoot + "/textures/platforms/" + name + ".png")){
            tex->setSmooth(false);
            textures[name] = tex;
        } else {
            std::cout << "ERROR::LEVEL::Could not load texture: " << name << "\n";
            delete tex;
        }
    }
    
    // Enemy textures - only load what you have
    std::vector<std::string> enemyTextures = {"drowner", "ghoul", "wraith", "griffin"};
    for(const auto& name : enemyTextures) {
        sf::Texture* tex = new sf::Texture();
        if(tex->loadFromFile(assetRoot + "/textures/enemies/" + name + ".png")){
            tex->setSmooth(false);  // ADD THIS - already there, but make sure!
            textures[name] = tex;
        } else {
            std::cout << "ERROR::LEVEL::Could not load enemy texture: " << name << "\n";
            delete tex;
        }
    }
}

bool Level::loadFromFile(const std::string& filename){
    std::ifstream file(filename);
    if(!file.is_open()){
        std::cout << "ERROR::LEVEL::Could not open " << filename << "\n";
        return false;
    }
    
    std::string line;
    while(std::getline(file, line)){
        if(line.empty() || line[0] == '#')
            continue;
        
        std::istringstream iss(line);
        std::string type;
        
        if(iss >> type){
            if(type == "platform"){
                float x, y, width, height;
                std::string textureName;
                
                if(iss >> x >> y >> width >> height >> textureName){
                    sf::Texture* tex = nullptr;
                    auto it = textures.find(textureName);
                    if(it != textures.end())
                        tex = it->second;
                    else
                        std::cout << "WARNING::LEVEL::Texture not found: " << textureName << "\n";
                    
                    platforms.push_back(new Platform(x, y, width, height, tex));
                }
                
            } else if(type == "enemy"){
                float x, y, width, height, patrolLeft, patrolRight;
                std::string enemyTypeName;
                
                // NOW read everything for enemy
                if(iss >> x >> y >> width >> height >> enemyTypeName >> patrolLeft >> patrolRight){
                    // Convert string to ENEMY_TYPE enum
                    ENEMY_TYPE enemyType;
                    if(enemyTypeName == "drowner")
                        enemyType = DROWNER;
                    else if(enemyTypeName == "nekker")
                        enemyType = NEKKER;
                    else if(enemyTypeName == "ghoul")
                        enemyType = GHOUL;
                    else if(enemyTypeName == "wraith")
                        enemyType = WRAITH;
                    else if(enemyTypeName == "griffin")
                        enemyType = GRIFFIN;
                    else {
                        std::cout << "WARNING::LEVEL::Unknown enemy type: " << enemyTypeName << "\n";
                        continue;
                    }
                    
                    // Get texture
                    sf::Texture* tex = nullptr;
                    auto it = textures.find(enemyTypeName);
                    if(it != textures.end())
                        tex = it->second;
                    else
                        std::cout << "WARNING::LEVEL::Enemy texture not found: " << enemyTypeName << "\n";
                    
                    std::cout << "Creating enemy at: " << x << ", " << y << " patrol: " << patrolLeft << "-" << patrolRight << "\n";
                    enemies.push_back(new Enemy(x, y, enemyType, tex, patrolLeft, patrolRight));
                }
                
            } else if(type == "modifier"){
                // TODO
            }
        }
    }
    
    file.close();
    return true;
}

// Accessors
const std::vector<Platform*>& Level::getPlatforms() const {
    return this->platforms;
}

const std::vector<Enemy*>& Level::getEnemies() const {
    return this->enemies;
}

// Functions

void Level::update(const sf::Vector2f& playerPos, const sf::View& view){
    // Large margin for updates - enemies move even when offscreen
    float updateMargin = 500.f;  // Update enemies 500px beyond screen
    
    sf::FloatRect updateBounds(
        view.getCenter().x - view.getSize().x / 2.f - updateMargin,
        view.getCenter().y - view.getSize().y / 2.f - updateMargin,
        view.getSize().x + updateMargin * 2.f,
        view.getSize().y + updateMargin * 2.f
    );
    
    // Only update enemies that are reasonably close
    for(auto* enemy : enemies){
        if(enemy->getGlobalBounds().intersects(updateBounds)){
            enemy->updatePlayerPosition(playerPos);
            enemy->update();
        }
    }
}

void Level::render(sf::RenderTarget& target){
    sf::View view = target.getView();
    
    // Smaller margin for rendering - only draw what's visible
    float renderMargin = 100.f;
    
    sf::FloatRect viewBounds(
        view.getCenter().x - view.getSize().x / 2.f - renderMargin,
        view.getCenter().y - view.getSize().y / 2.f - renderMargin,
        view.getSize().x + renderMargin * 2.f,
        view.getSize().y + renderMargin * 2.f
    );
    
    // Render platforms in view
    for(auto* platform : platforms){
        if(platform->getBounds().intersects(viewBounds)){
            platform->render(target);
        }
    }

    // Render enemies in view
    for(auto* enemy : enemies){
        if(enemy->getGlobalBounds().intersects(viewBounds)){
            enemy->render(target);
        }
    }
}
#include "Level.h"
#include <fstream>
#include <sstream>

// Constructors / Destructors
Level::Level(){
    this->LoadTextures();
}

Level::~Level(){
    for (auto* platform : platforms) {
        delete platform;
    }
    for (auto& pair : textures) {
        delete pair.second;
    }
}

// Core
void Level::LoadTextures(){
    std::vector<std::string> textureNames = {"rock1", "rock2"};
    
    for(const auto& name : textureNames) {
        sf::Texture* tex = new sf::Texture();
        if(tex->loadFromFile("./textures/platforms/" + name + ".png")){
            tex->setSmooth(false);  // ADD THIS
            textures[name] = tex;
        } else {
            std::cout << "ERROR::LEVEL::Could not load texture: " << name << "\n";
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
        float x, y, width, height;
        std::string textureName;
        
        if(iss >> type >> x >> y >> width >> height >> textureName){
            if(type == "platform"){
                sf::Texture* tex = nullptr;
                
                auto it = textures.find(textureName);
                if(it != textures.end())
                    tex = it->second;
                else
                    std::cout << "WARNING::LEVEL::Texture not found: " << textureName << "\n";
                
                platforms.push_back(new Platform(x, y, width, height, tex));
            }else if(type == "enemy"){
                // TODO
            }else if(type == "modifier"){
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

// Functions
void Level::render(sf::RenderTarget& target){
    for(auto* platform : platforms)
        platform->render(target);
}
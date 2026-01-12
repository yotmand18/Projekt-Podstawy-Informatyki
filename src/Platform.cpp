#include "Platform.h"

Platform::Platform(float x, float y, float width, float height, sf::Texture* texture){
    if(texture != nullptr){ 
        this->sprite.setTexture(*texture);
        this->sprite.setPosition(x, y);
        
        // Calculate scale based on desired size AND 5x base scale
        float scaleX = (width / texture->getSize().x);
        float scaleY = (height / texture->getSize().y);
        
        this->sprite.setScale(scaleX, scaleY);
    } else {
        std::cout << "ERROR::PLATFORM::Texture is null!\n";
    }
}

Platform::~Platform(){}

sf::FloatRect Platform::getBounds() const{
    return this->sprite.getGlobalBounds();
}

void Platform::render(sf::RenderTarget& target){
    target.draw(this->sprite);
}
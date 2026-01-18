#include "Input.h"
#include "Settings.h"

Input::Input(Settings* the_settings){
    this->settings = new Settings();
}

Input::~Input(){

}

bool Input::isAction(std::string action) const {
    if (!settings) return false;
    return sf::Keyboard::isKeyPressed(settings->getKeybind(action));
}

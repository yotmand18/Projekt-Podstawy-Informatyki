#pragma once

#include "lib.h"

class Settings;

class Input{
    private:
        // Key Bindings
        Settings* settings;


    public:
        // Constructors / Destructors

        Input(Settings* the_settings);
        virtual ~Input();

        // Accessors
        bool isAction(std::string action) const;

        // Modifiers


        // Functions

    


};
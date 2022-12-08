#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include "PerlinNoise.hpp"

#ifndef APP_H
#define APP_H

/*
    Main App class. It has everything related to app.
*/

class App {
    private:
        // ----- VARIABLES ------ //
        sf::RenderWindow* window;
        sf::VideoMode videoMode;
        sf::Uint32 windowStyle;
        std::string windowTitle;
        sf::Event event;
        const int FPS = 30;

        // ----- APP OBJECTS ----- //

        // ----- PRIVATE FUNCTIONS -----//
        void initVariables();
        void initWindow();

    public:
        // ----- CONSTRUCTORS/DESTRUCTORS ----- //
        App();
        virtual ~App();

        // ----- ACCESSORS ----- //
        const bool isRunning() const;

        // ----- FUNCTIONS ----- //
        void pollEvents();
        void update();
        void render();
    
};

#endif

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>

#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include "PerlinNoise.hpp"
#include "Textbox.h"
#include "Button.h"

#ifndef APP_H
#define APP_H

/*
    Main App class.
*/

class App {
    private:
        // ----- VARIABLES ------ //
        sf::RenderWindow* window;
        sf::VideoMode videoMode;
        sf::Uint32 windowStyle;
        sf::Event event;
        const int FPS = 30;
        const int SCROLL_SPEED = 8;
        const float PADDING = 16;
        int mapWidth;
        int mapHeight;
        int** map;
        float panelPosX;
        sf::Vector2i scroll;
        bool toggleGrid;
        std::map<std::string, bool> inputs;
        sf::RectangleShape panelRect;
        sf::Font font;
        std::map<int, sf::Texture> textures;
        std::map<int, sf::Sprite> blocks;
        std::vector<sf::Text> labels;
        Button* buttonGenerate;
        Button* buttonExport;
        Textbox* textboxMapWidth;
        Textbox* textboxMapHeight;

        // ----- APP OBJECTS ----- //

        // ----- PRIVATE FUNCTIONS -----//
        void initVariables();
        void initWindow();
        void initFont();
        void initSprites();
        void initPanel();
        void initMap();

        void drawMap();
        void drawGrid();
        void drawPanel();
        void resetTerrain();  // TEMP workaround
        void generateTerrain();
        void exportToCSV();

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

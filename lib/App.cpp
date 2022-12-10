#include "App.h"

// ----- PRIVATE FUNCTIONS ----- //
void App::initVariables() {
    /*
        Initialize all necessary variables related to app.
    */
    this->window = nullptr;
    this->map = nullptr;
}

void App::initWindow() {
    /*
        Initialize window - its size and framerate limit.
    */
    this->videoMode.width = 1280;
    this->videoMode.height = 256; // 720

    this->windowStyle = sf::Style::Titlebar | sf::Style::Close;
    this->windowTitle = "Mapeczka Tomeczka";

    this->window = new sf::RenderWindow(this->videoMode, this->windowTitle, this->windowStyle);
    this->window->setFramerateLimit(this->FPS);
}

void App::initMap() {
    /*
        Initialize map - 2D array filled with zeros.
        Size of the array is declared by user.
    */
    // std::cout << "Enter map width: ";
    // std::cin >> this->mapWidth;
    // std::cout << "Enter map height: ";
    // std::cin >> this->mapHeight;
    this->mapWidth = 80;
    this->mapHeight = 16;

    this->map = new int*[this->mapHeight];
    for (int i = 0; i < this->mapHeight; i++) {
        this->map[i] = new int[this->mapWidth];
        for (int j = 0; j < this->mapWidth; j++) {
            this->map[i][j] = 0;
        }
    }
}

void App::drawMap() {
    /*
        Iterate through array and draw correspond tiles on the window.
    */
    sf::RectangleShape tile;
    tile.setSize(sf::Vector2f(16.f, 16.f));
    // tile.setOutlineColor(sf::Color::Black);
    // tile.setOutlineThickness(1.f);
    for (int y = 0; y < this->mapHeight; y++) {
        for (int x = 0; x < this->mapWidth; x++) {
            if (this->map[y][x] == 0) {  // empty
                tile.setFillColor(sf::Color::Cyan);
            } else if (this->map[y][x] == 1) { // dirt
                tile.setFillColor(sf::Color(210, 105, 30));
            } else if (this->map[y][x] == 2) { // grass
                tile.setFillColor(sf::Color::Green);
            }
            tile.setPosition(x * 16, y * 16);
            this->window->draw(tile);
        }
    }
}

void App::generateTerrain() {
    /*
        Generate terrain using Perlin noise.
    */
    const siv::PerlinNoise::seed_type seed = time(NULL);
    const siv::PerlinNoise perlin{seed};

    float smoothness = 0.2;
    int heightDiff = 6;
    for (int y = 0; y < this->mapHeight; y++) {
        for (int x = 0; x < this->mapWidth; x++) {
            const double height = int(perlin.noise1D((x * smoothness)) * heightDiff);
            if (y > 8 - height) {
                map[y][x] = 1;
            } else if (y == 8 - height) {
                map[y][x] = 2;
            }
        }
    }
}


// ----- CONSTRUCTORS/DESTRUCTORS ----- //
App::App() {
    /*
        Call all init functions.
    */
    this->initVariables();
    this->initMap();
    this->generateTerrain();
    this->initWindow();
}

App::~App() {
    /*
        Delete window and map to prevent memory leak.
    */
    delete this->window;
    delete this->map;
}


// ----- ACCESSORS ----- //
const bool App::isRunning() const {
    /*
        Check if app is still running (X button or escape key was not pressed).
    */
    return this->window->isOpen();
}


// ----- FUNCTIONS ----- //
void App::pollEvents() {
    /*
        Iterate through events during frame and handle them.
    */
    while (this->window->pollEvent(this->event)) {
        switch (this->event.type) {
            // X button - close the app
            case sf::Event::Closed:
                this->window->close();
                break;
            case sf::Event::KeyPressed:
                // escape key - close the app
                if (this->event.key.code == sf::Keyboard::Escape) {
                    this->window->close();
                    break;
                }
            default:
                break;
            }
    }
}

void App::update() {
    /*
        Handle events and update all things related to app.
    */
    this->pollEvents();
}

void App::render() {
    /*
        Clear old frame, render all objects and display them on current frame.
    */
    this->window->clear();

    // draw objects
    this->drawMap();

    this->window->display();
}

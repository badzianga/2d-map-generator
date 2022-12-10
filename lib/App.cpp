#include "App.h"

#define EMPTY 0
#define DIRT 1
#define GRASS 2
#define STONE 3
#define BEDROCK 4

// ----- PRIVATE FUNCTIONS ----- //
void App::initVariables() {
    /*
        Initialize all necessary variables related to app.
    */
    this->window = nullptr;
    this->map = nullptr;

    this->scroll.x = 0;
    this->scroll.y = 0;
}

void App::initWindow() {
    /*
        Initialize window - its size and framerate limit.
    */
    this->videoMode.width = 1280;
    this->videoMode.height = 512;  // 720;

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
    this->mapWidth = 160;
    this->mapHeight = 32;

    this->map = new int*[this->mapHeight];
    for (int i = 0; i < this->mapHeight; i++) {
        this->map[i] = new int[this->mapWidth];
        for (int j = 0; j < this->mapWidth; j++) {
            this->map[i][j] = EMPTY;
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
            if (this->map[y][x] == EMPTY) {  // empty
                tile.setFillColor(sf::Color::Cyan);
            } else if (this->map[y][x] == DIRT) { // dirt
                tile.setFillColor(sf::Color(210, 105, 30));
            } else if (this->map[y][x] == STONE) {  // stone
                tile.setFillColor(sf::Color(128, 128, 128));
            } else if (this->map[y][x] == BEDROCK) {
                tile.setFillColor(sf::Color(64, 64, 64));
            } else if (this->map[y][x] == GRASS) { // grass
                tile.setFillColor(sf::Color::Green);
            }
            tile.setPosition(x * 16 - this->scroll.x, y * 16 - this->scroll.y);
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
            if (y > this->mapHeight - 5) {  // bedrock and stone
                if (y == this->mapHeight - 4) {
                    if (std::rand() % 10 >= 8) {
                        map[y][x] = BEDROCK;
                    } else {
                        map[y][x] = STONE;
                    }
                } else if (y == this->mapHeight - 3) {
                    if (std::rand() % 10 >= 6) {
                        map[y][x] = BEDROCK;
                    } else {
                        map[y][x] = STONE;
                    }
                } else if (y == this->mapHeight - 2) {
                    if (std::rand() % 10 >= 4) {
                        map[y][x] = BEDROCK;
                    } else {
                        map[y][x] = STONE;
                    }
                } else {  // bottom of the world
                    map[y][x] = BEDROCK;
                }
            } else if (y > 12 - height) { // stone
                map[y][x] = STONE;
            } else if (y > 8 - height) {  // dirt
                map[y][x] = DIRT;
            } else if (y == 8 - height) {  // grass
                map[y][x] = GRASS;
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
        // X button - close the app
        if (this-> event.type == sf::Event::Closed) {
            this->window->close();
        }
        if (this->event.type == sf::Event::KeyPressed) {
            // escape key - close the app
            if (this->event.key.code == sf::Keyboard::Escape) {
                this->window->close();
            }

            // left/right camera movement
            if (this->event.key.code == sf::Keyboard::Left) {
                this->scroll.x -= this->SCROLL_SPEED;
            } else if (this->event.key.code == sf::Keyboard::Right) {
                this->scroll.x += this->SCROLL_SPEED;
            }

            // up/down camera movement
            if (this->event.key.code == sf::Keyboard::Up) {
                this->scroll.y -= this->SCROLL_SPEED;
            } else if (this->event.key.code == sf::Keyboard::Down) {
                this->scroll.y += this->SCROLL_SPEED;
            }
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

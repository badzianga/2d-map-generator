#include "App.h"

// ----- PRIVATE FUNCTIONS ----- //
void App::initVariables() {
    /*
        Initialize all necessary variables related to app.
    */
    this->window = nullptr;
}

void App::initWindow() {
    /*
        Initialize window - its size and framerate limit.
    */
    this->videoMode.width = 1280;
    this->videoMode.height = 720;

    this->windowStyle = sf::Style::Titlebar | sf::Style::Close;
    this->windowTitle = "Mapeczka Tomeczka";

    this->window = new sf::RenderWindow(this->videoMode, this->windowTitle, this->windowStyle);
    this->window->setFramerateLimit(this->FPS);
}


// ----- CONSTRUCTORS/DESTRUCTORS ----- //
App::App() {
    /*
        Call all init functions.
    */
    this->initVariables();
    this->initWindow();
}

App::~App() {
    /*
        Delete window to prevent memory leak.
    */
    delete this->window;
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

    this->window->display();
}

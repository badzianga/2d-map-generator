#include "App.h"

#define TILE_SIZE 16
#define TILE_SIZE_F 16.f

#define EMPTY 0
#define DIRT 1
#define GRASS 2
#define STONE 3
#define BEDROCK 4
#define DANDELION 5
#define BG_STONE 6

// ----- PRIVATE FUNCTIONS ----- //
void App::initVariables() {
    /*
        Initialize all necessary variables related to app.
    */
    this->window = nullptr;
    this->map = nullptr;
    this->buttonGenerate = nullptr;
    this->buttonExport = nullptr;
    this->textboxMapWidth = nullptr;
    this->textboxMapHeight = nullptr;
    this->textboxSmoothness = nullptr;
    this->textboxHeightDiff = nullptr;
    this->textboxAliveProb = nullptr;
    this->textboxGenerations = nullptr;
    this->textboxDirtDensity = nullptr;
    this->textboxDirtProb = nullptr;
    this->textboxGroundLevel = nullptr;
    this->textboxFloodCaveSize = nullptr;
    this->caves = nullptr;

    this->mapWidth = 80;
    this->mapHeight = 46;
    this->smoothness = 0.2;
    this->heightDiff = 6;
    this->dirtProb = 0.4;
    this->dirtDensity = 0.1;
    this->groundLevel = 64;
    this->floodFillCounter = 0;
    this->floodCaveSize = 200;

    this->scroll.x = 0;
    this->scroll.y = 0;

    this->toggleGrid = false;

    this->inputs["up"] = false;
    this->inputs["down"] = false;
    this->inputs["left"] = false;
    this->inputs["right"] = false;
}

void App::initWindow() {
    /*
        Initialize window - its size and framerate limit.
    */
    this->videoMode.width = 1280;
    this->videoMode.height = 720;

    this->windowStyle = sf::Style::Titlebar | sf::Style::Close;

    this->window = new sf::RenderWindow(this->videoMode, "2D Map Generator", this->windowStyle);
    this->window->setFramerateLimit(this->FPS);

    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

    this->window->setPosition(sf::Vector2i(desktop.width / 2 - this->videoMode.width / 2, desktop.height / 2 - this->videoMode.height / 2));
}

void App::initFont() {
    /*
        Initialize font - load it from file. If error occurs, the app is terminated.
    */
    if (!this->font.loadFromFile("fonts/Minecraft.ttf")) {
        std::cout << "Cannot load Minecraft.ttf font! Exiting...\n";
        exit(-1);
    }
}

void App::initSprites() {
    /*
        Load all textures from the folder and create sprites. If pre-defined sprites
        are not found, the app is terminated.
    */
    std::string images[] = {"dirt", "grass", "stone", "bedrock", "dandelion", "bgstone"};
    int index = 1;

    // load textures from directory and save them as sprites in map
    for (std::string image : images) {
        if (!this->textures[index].loadFromFile("sprites/" + image + ".png")) {
            std::cout << "Cannot load " << image << ".png. Exiting...\n";
            exit(-1);
        }
        this->blocks[index].setTexture(this->textures[index]);
        this->blocks[index].setScale(0.1, 0.1);
        index++;
    }
}

void App::initPanel() {
    /*
        Initialize panel and all things related to it.
    */
    // configure panel rect
    this->panelPosX = this->videoMode.width * 0.7;
    this->panelRect.setSize(sf::Vector2f(float(this->videoMode.width * 0.3), float(this->videoMode.height)));
    this->panelRect.setPosition(float(this->panelPosX), 0.f);
    this->panelRect.setFillColor(sf::Color(27, 29, 36));

    // configure labels
    sf::Text label;
    label.setFont(this->font);
    label.setCharacterSize(24);
    label.setFillColor(sf::Color::White);
    label.setOutlineColor(sf::Color::Black);
    label.setOutlineThickness(1.f);

    std::string texts[] = {"Map width:", "Map height:", "Smoothness:", "Height diff:", "Alive prob:", "Generations:", "Dirt density:", "Dirt prob:", "Ground level:", "Flood size:"};
    int i = 0;
    for (std::string text : texts) {
        label.setString(text);
        label.setPosition(panelPosX + PADDING, float(i * 24 + (i + 1) * PADDING));
        this->labels.push_back(label);
        i++;
    }

    // configure textboxes
    // map width
    this->textboxMapWidth = new Textbox(sf::Vector2f(96, 24), 24, false, 80);
    this->textboxMapWidth->setFont(this->font);
    this->textboxMapWidth->setLimit(true, 4);
    this->textboxMapWidth->setPosition(sf::Vector2f(panelPosX + 200.f, PADDING));

    // map height
    this->textboxMapHeight = new Textbox(sf::Vector2f(96, 24), 24, false, 96);
    this->textboxMapHeight->setFont(this->font);
    this->textboxMapHeight->setLimit(true, 4);
    this->textboxMapHeight->setPosition(sf::Vector2f(panelPosX + 200.f, 24 + 2 * PADDING));

    // smoothness
    this->textboxSmoothness = new Textbox(sf::Vector2f(96, 24), 24, false, 20);
    this->textboxSmoothness->setFont(this->font);
    this->textboxSmoothness->setLimit(true, 3);
    this->textboxSmoothness->setPosition(sf::Vector2f(panelPosX + 200.f, 48 + 3 * PADDING));

    // height diff
    this->textboxHeightDiff = new Textbox(sf::Vector2f(96, 24), 24, false, 6);
    this->textboxHeightDiff->setFont(this->font);
    this->textboxHeightDiff->setLimit(true, 2);
    this->textboxHeightDiff->setPosition(sf::Vector2f(panelPosX + 200.f, 72 + 4 * PADDING));

    // alive prob
    this->textboxAliveProb = new Textbox(sf::Vector2f(96, 24), 24, false, 58);
    this->textboxAliveProb->setFont(this->font);
    this->textboxAliveProb->setLimit(true, 2);
    this->textboxAliveProb->setPosition(sf::Vector2f(panelPosX + 200.f, 96 + 5 * PADDING));

    // generations
    this->textboxGenerations = new Textbox(sf::Vector2f(96, 24), 24, false, 10);
    this->textboxGenerations->setFont(this->font);
    this->textboxGenerations->setLimit(true, 2);
    this->textboxGenerations->setPosition(sf::Vector2f(panelPosX + 200.f, 120 + 6 * PADDING));

    // dirt density
    this->textboxDirtDensity = new Textbox(sf::Vector2f(96, 24), 24, false, 10);
    this->textboxDirtDensity->setFont(this->font);
    this->textboxDirtDensity->setLimit(true, 3);
    this->textboxDirtDensity->setPosition(sf::Vector2f(panelPosX + 200.f, 144 + 7 * PADDING));

    // dirt prob
    this->textboxDirtProb = new Textbox(sf::Vector2f(96, 24), 24, false, 40);
    this->textboxDirtProb->setFont(this->font);
    this->textboxDirtProb->setLimit(true, 2);
    this->textboxDirtProb->setPosition(sf::Vector2f(panelPosX + 200.f, 168 + 8 * PADDING));

    // ground level
    this->textboxGroundLevel = new Textbox(sf::Vector2f(96, 24), 24, false, 64);
    this->textboxGroundLevel->setFont(this->font);
    this->textboxGroundLevel->setLimit(true, 4);
    this->textboxGroundLevel->setPosition(sf::Vector2f(panelPosX + 200.f, 192 + 9 * PADDING));

    // flood cave size
    this->textboxFloodCaveSize = new Textbox(sf::Vector2f(96, 24), 24, false, 200);
    this->textboxFloodCaveSize->setFont(this->font);
    this->textboxFloodCaveSize->setLimit(true, 4);
    this->textboxFloodCaveSize->setPosition(sf::Vector2f(panelPosX + 200.f, 216 + 10 * PADDING));

    // configure buttons
    this->buttonGenerate = new Button({144, 32}, "Generate", 24, {0.f, -6.f});
    this->buttonGenerate->setFont(this->font);
    this->buttonGenerate->setPosition({panelPosX + 2.F * PADDING, float(this->videoMode.height - 3 * PADDING)});

    this->buttonExport = new Button({144, 32}, "Export", 24, {0.f, -4.f});
    this->buttonExport->setFont(this->font);
    this->buttonExport->setPosition({panelPosX + 12.f * PADDING, float(this->videoMode.height - 3 * PADDING)});
}

void App::initMap() {
    /*
        Initialize map - 2D array filled with zeros.
        Size of the array is declared by user.
    */
    this->map = new int*[this->mapHeight];
    for (int i = 0; i < this->mapHeight; i++) {
        this->map[i] = new int[this->mapWidth];
        for (int j = 0; j < this->mapWidth; j++) {
            this->map[i][j] = EMPTY;
        }
    }
}

void App::initCaves() {
    this->caves = new Caves();
}

void App::drawGrid() {
    /*
        Draw help grid.
    */
    sf::RectangleShape line(sf::Vector2f(1.f, this->videoMode.height));
    line.setFillColor(sf::Color(192, 192, 192));
    // draw vertical lines
    for (int x = 0; x < this->mapWidth; x++) {
        line.setPosition(x * TILE_SIZE - this->scroll.x, 0.f);
        this->window->draw(line);
    }

    line.setSize(sf::Vector2f(this->videoMode.width, 1.f));
    // draw horizontal lines
    for (int y = 0; y < this->mapHeight; y++) {
        line.setPosition(0.f, y * TILE_SIZE - this->scroll.y);
        this->window->draw(line);
    }
}

void App::drawMap() {
    /*
        Iterate through array and draw correspond tiles on the window.
    */
    // int counter = 0;
    // for (int y = 0; y < this->mapHeight; y++) {
    //     for (int x = 0; x < this->mapWidth; x++) {
    //         if (this->map[y][x] != EMPTY) {
    //             this->blocks[map[y][x]].setPosition(x * TILE_SIZE - this->scroll.x, y * TILE_SIZE - this->scroll.y);
    //             this->window->draw(this->blocks[map[y][x]]);
    //             counter++;
    //         }
    //     }
    // }
    int renderLimitX = ((this->scroll.x + this->videoMode.width * 0.7) / TILE_SIZE) + this->scroll.x / SCROLL_SPEED % 2;
    int renderLimitY = ((this->scroll.y + this->videoMode.height) / TILE_SIZE) + this->scroll.y / SCROLL_SPEED % 2;
    // std::cout << "x limits: " << this->scroll.x / TILE_SIZE << ' ' << renderLimitX << '\n';
    // std::cout << "y limits: " << this->scroll.y / TILE_SIZE << ' ' << renderLimitY << "\n\n";
    // int ground = this->mapHeight - this->groundLevel;

    for (int y = this->scroll.y / TILE_SIZE; y < renderLimitY; y++) {
        for (int x = this->scroll.x / TILE_SIZE; x < renderLimitX; x++) {
            if (this->map[y][x] != EMPTY) {
                this->blocks[map[y][x]].setPosition(x * TILE_SIZE - this->scroll.x, y * TILE_SIZE - this->scroll.y);
                this->window->draw(this->blocks[map[y][x]]);
                // counter++;
            }
            // } else if (y > ground) {
            //     this->blocks[BG_STONE].setPosition(x * TILE_SIZE - this->scroll.x, y * TILE_SIZE - this->scroll.y);
            //     this->window->draw(this->blocks[BG_STONE]);
            // }
        }
    }
    // std::cout << "Tiles drawn: " << counter << '\n';
}

void App::drawPanel() {
    /*
        Draw panel with texts, input fields and buttons.
    */
    // panel
    this->window->draw(this->panelRect);

    // labels
    for (sf::Text label : this->labels) {
        this->window->draw(label);
    }

    this->textboxMapWidth->drawTo(this->window);
    this->textboxMapHeight->drawTo(this->window);
    this->textboxSmoothness->drawTo(this->window);
    this->textboxHeightDiff->drawTo(this->window);
    this->textboxAliveProb->drawTo(this->window);
    this->textboxGenerations->drawTo(this->window);
    this->textboxDirtDensity->drawTo(this->window);
    this->textboxDirtProb->drawTo(this->window);
    this->textboxGroundLevel->drawTo(this->window);
    this->textboxFloodCaveSize->drawTo(this->window);
    this->buttonGenerate->drawTo(this->window);
    this->buttonExport->drawTo(this->window);
}

void App::generateTerrain() {
    /*
        Generate terrain using Perlin noise.
    */
    const siv::PerlinNoise::seed_type seed = rand() % 10 * time(NULL);
    const siv::PerlinNoise perlin{seed};

    int ground = this->mapHeight - this->groundLevel;

    for (int y = 0; y < this->mapHeight; y++) {
        for (int x = 0; x < this->mapWidth; x++) {
            const double height = int(perlin.noise1D_01((x * this->smoothness)) * this->heightDiff);
            if (y > this->mapHeight - 6) {  // bedrock and stone
                if (rand() % 10 >= (2 * (mapHeight - y - 1))) {
                    map[y][x] = BEDROCK;
                } else {
                    map[y][x] = STONE;
                }
            } else if (y > ground + 4 - height) { // stone
                // stone and empty spaces (caves)
                map[y][x] = this->caves->getCell(x, y) * STONE;

                // dirt patches
                if (perlin.noise2D_01(x * this->dirtDensity, y * this->dirtDensity) < this->dirtProb && map[y][x] != 0) {
                    map[y][x] = DIRT;
                }
                // map[y][x] = STONE;
            } else if (y > ground - height) {  // dirt
                map[y][x] = DIRT;
            } else if (y == ground - height) {  // grass
                map[y][x] = GRASS;
            } else if (y == ground - height - 1) {  // foliage
                if (rand() % 10 == 0) {
                    map[y][x] = DANDELION;
                }
            }
        }
    }

    this->generateCaveEntrances();
}

void App::generateCaveEntrances() {
    /*
        Generate cave entrance by removing grass/dirt from areas closest to big cave.
    */
    int y;
    int x = 0;
    while (x < this->mapWidth) {
        y = 0;
        while (this->map[y][x] != GRASS) y++;  // find ground level

        // check if space 5 blocks below grass block is empty
        if (this->map[y + 5][x] == EMPTY) {
            // count size of empty space below ground
            this->floodFillCounter = 0;
            this->floodFill(x, y + 5, EMPTY, -1);
            // if it's big enough, leave it as empty space
            if (this->floodFillCounter >= this->floodCaveSize) {
                this->floodFillCounter = 0;
                this->floodFill(x, y + 5, -1, EMPTY);
            } else {  // if it's not big enough, fill it with stone
                this->floodFillCounter = 0;
                this->floodFill(x, y + 5, -1, STONE);
            }
        }
        x++;  // go to the next block
    }

    // do it again, but now only create entrance
    x = 0;
    int toDwell = 3 + rand() % 4;
    while (x < this->mapWidth && toDwell > 0) {
        y = 0;
        while(this->map[y][x] != GRASS) y++;
        if (this->map[y + 5][x] == EMPTY) {
            for (int i = -1; i < 6; i++) {
                this->map[y + i][x] = EMPTY;
            }
            toDwell--;
        }
        x++;
    }
}

void App::floodFill(int x, int y, int toCheck, int filler) {
    if (x <= 0 || x >= this->mapWidth || y <= 0 || y >= this->mapHeight || this->map[y][x] != toCheck) {
        return;
    }
    this->floodFillCounter++;
    this->map[y][x] = filler;

    this->floodFill(x - 1, y, toCheck, filler);
    this->floodFill(x + 1, y, toCheck, filler);
    this->floodFill(x, y - 1, toCheck, filler);
    this->floodFill(x, y + 1, toCheck, filler);

}

void App::exportToCSV() {
    /*
        Export map data to csv file with current date and time
    */
    std::ofstream file;
    time_t now = time(0);
    tm *ltm = localtime(&now);

    // prepare date and time for filename
    std::string year = std::to_string(1900 + ltm->tm_year);
    std::string month = std::to_string(1 + ltm->tm_mon);
    if (month.length() < 2) month = '0' + month;
    std::string day = std::to_string(ltm->tm_mday);
    if (day.length() < 2)   day = '0' + day;
    std::string hour = std::to_string(ltm->tm_hour);
    if (hour.length() < 2)  hour = '0' + hour;
    std::string min = std::to_string(ltm->tm_min);
    if (min.length() < 2)   min = '0' + min;
    std::string sec = std::to_string(ltm->tm_sec);
    if (sec.length() < 2)   sec = '0' + sec;

    std::string filename = "terrain_" + year + '-' + month + '-' + day+ '_' + hour + ':' + min + ':' + sec + ".csv";

    file.open("maps/" + filename);

    for (int y = 0; y < mapHeight; y++) {
        file << map[y][0];
        for (int x = 1; x < mapWidth; x++) {
            file << ',' << map[y][x];
        }
        file << '\n';
    }

    file.close();
}


// ----- CONSTRUCTORS/DESTRUCTORS ----- //
App::App() {
    /*
        Call all init functions, start randomization.
    */
    srand(time(NULL));

    this->initVariables();
    this->initFont();
    this->initSprites();
    this->initMap();
    this->initCaves();
    this->initWindow();
    this->initPanel();
}

App::~App() {
    /*
        Delete dynamically-declared variables to prevent memory leak.
    */
    delete this->window;
    delete this->map;

    delete this->textboxMapWidth;
    delete this->textboxMapHeight;
    delete this->textboxSmoothness;
    delete this->textboxHeightDiff;
    delete this->textboxAliveProb;
    delete this->textboxGenerations;
    delete this->textboxDirtDensity;
    delete this->textboxDirtProb;
    delete this->textboxGroundLevel;
    delete this->textboxFloodCaveSize;

    delete this->buttonGenerate;
    delete this->buttonExport;

    delete this->caves;
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

            // F12 - toggling tile grid
            if (this->event.key.code == sf::Keyboard::F12) {
                this->toggleGrid = !this->toggleGrid;
            }

            // left/right camera movement
            if (this->event.key.code == sf::Keyboard::Left) {
                this->inputs["left"] = true;
            }
            if (this->event.key.code == sf::Keyboard::Right) {
                this->inputs["right"] = true;
            }

            // up/down camera movement
            if (this->event.key.code == sf::Keyboard::Up) {
                this->inputs["up"] = true;
            }
            if (this->event.key.code == sf::Keyboard::Down) {
                this->inputs["down"] = true;
            }
        }
        if (this->event.type == sf::Event::KeyReleased) {
            // left/right camera movement
            if (this->event.key.code == sf::Keyboard::Left) {
                this->inputs["left"] = false;
            }
            if (this->event.key.code == sf::Keyboard::Right) {
                this->inputs["right"] = false;
            }

            // up/down camera movement
            if (this->event.key.code == sf::Keyboard::Up) {
                this->inputs["up"] = false;
            }
            if (this->event.key.code == sf::Keyboard::Down) {
                this->inputs["down"] = false;
            }
        }

        if (this->event.type == sf::Event::TextEntered) {
            this->textboxMapWidth->typedOn(this->event);
            this->textboxMapHeight->typedOn(this->event);
            this->textboxSmoothness->typedOn(this->event);
            this->textboxHeightDiff->typedOn(this->event);
            this->textboxAliveProb->typedOn(this->event);
            this->textboxGenerations->typedOn(this->event);
            this->textboxDirtDensity->typedOn(this->event);
            this->textboxDirtProb->typedOn(this->event);
            this->textboxGroundLevel->typedOn(this->event);
            this->textboxFloodCaveSize->typedOn(this->event);
        }

        if (this->event.type == sf::Event::MouseMoved) {
            if (this->buttonGenerate->isMouseOver(this->window)) {
                this->buttonGenerate->setBgColor(sf::Color(56, 61, 73));
            } else {
                this->buttonGenerate->setBgColor(sf::Color(38, 42, 51));
            }
            if (this->buttonExport->isMouseOver(this->window)) {
                this->buttonExport->setBgColor(sf::Color(56, 61, 73));
            } else {
                this->buttonExport->setBgColor(sf::Color(38, 42, 51));
            }
        }

        if (this->event.type == sf::Event::MouseButtonPressed) {
            if (this->textboxMapWidth->isMouseOver(this->window)) {
                this->textboxMapWidth->setSelected(true);
            } else {
                this->textboxMapWidth->setSelected(false);
            }
            if (this->textboxMapHeight->isMouseOver(this->window)) {
                this->textboxMapHeight->setSelected(true);
            } else {
                this->textboxMapHeight->setSelected(false);
            }
            if (this->textboxSmoothness->isMouseOver(this->window)) {
                this->textboxSmoothness->setSelected(true);
            } else {
                this->textboxSmoothness->setSelected(false);
            }
            if (this->textboxHeightDiff->isMouseOver(this->window)) {
                this->textboxHeightDiff->setSelected(true);
            } else {
                this->textboxHeightDiff->setSelected(false);
            }
            if (this->textboxAliveProb->isMouseOver(this->window)) {
                this->textboxAliveProb->setSelected(true);
            } else {
                this->textboxAliveProb->setSelected(false);
            }
            if (this->textboxGenerations->isMouseOver(this->window)) {
                this->textboxGenerations->setSelected(true);
            } else {
                this->textboxGenerations->setSelected(false);
            }
            if (this->textboxDirtDensity->isMouseOver(this->window)) {
                this->textboxDirtDensity->setSelected(true);
            } else {
                this->textboxDirtDensity->setSelected(false);
            }
            if (this->textboxDirtProb->isMouseOver(this->window)) {
                this->textboxDirtProb->setSelected(true);
            } else {
                this->textboxDirtProb->setSelected(false);
            }
            if (this->textboxGroundLevel->isMouseOver(this->window)) {
                this->textboxGroundLevel->setSelected(true);
            } else {
                this->textboxGroundLevel->setSelected(false);
            }
            if (this->textboxFloodCaveSize->isMouseOver(this->window)) {
                this->textboxFloodCaveSize->setSelected(true);
            } else {
                this->textboxFloodCaveSize->setSelected(false);
            }

            if (buttonExport->isMouseOver(this->window)) {
                this->exportToCSV();
                std::cout << "Exported data to CSV file.\n";
            }
            if (buttonGenerate->isMouseOver(this->window)) {
                this->mapWidth = std::stoi(this->textboxMapWidth->getText());
                this->mapHeight = std::stoi(this->textboxMapHeight->getText());
                if (this->mapWidth <= 0 || this->mapHeight <= 0) {
                    continue;
                }
                this->smoothness = std::stof(this->textboxSmoothness->getText()) / 100.f;
                this->heightDiff = std::stoi(this->textboxHeightDiff->getText());
                int aliveProb = std::stoi(this->textboxAliveProb->getText());
                int generations = std::stoi(this->textboxGenerations->getText());
                this->dirtDensity = std::stof(this->textboxDirtDensity->getText()) / 100.f;
                this->dirtProb = std::stof(this->textboxDirtProb->getText()) / 100.f;
                this->groundLevel = std::stoi(this->textboxGroundLevel->getText());
                this->floodCaveSize = std::stoi(this->textboxFloodCaveSize->getText());

                this->scroll.x = 0;
                this->scroll.y = 0;

                delete this->map;
                this->initMap();
                this->caves->initArray(this->mapWidth, this->mapHeight, aliveProb);
                this->caves->generateCaves(generations);
                this->generateTerrain();
            }
        }
    }
}

void App::update() {
    /*
        Handle events and update all things related to app.
    */
    this->pollEvents();

    if (this->inputs["left"]) {
        this->scroll.x -= SCROLL_SPEED;
        if (this->scroll.x < 0) this->scroll.x = 0;
    }
    if (this->inputs["right"]) {
        this->scroll.x += SCROLL_SPEED;
        if (this->scroll.x > this->mapWidth * TILE_SIZE - this->videoMode.width)
            this->scroll.x = this->mapWidth * TILE_SIZE - this->videoMode.width;
    }
    if (this->inputs["up"]) {
        this->scroll.y -= SCROLL_SPEED;
        if (this->scroll.y < 0) this->scroll.y = 0;
    }
    if (this->inputs["down"]) {
        this->scroll.y += SCROLL_SPEED;
        if (this->scroll.y > this->mapHeight * TILE_SIZE - this->videoMode.height)
            this->scroll.y = this->mapHeight * TILE_SIZE - this->videoMode.height;
    }

}

void App::render() {
    /*
        Clear old frame, render all objects and display them on current frame.
    */
    this->window->clear(sf::Color::Cyan);

    this->drawMap();

    if (this->toggleGrid) {
        this->drawGrid();
    }

    this->drawPanel();

    this->window->display();
}

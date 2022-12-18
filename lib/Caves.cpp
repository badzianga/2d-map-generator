#include "Caves.h"

// ----- PRIVATE FUNCITONS ----- //
int Caves::countNeighbours(int x, int y) {
    /*
        Count neoghbors of the cell with given coordinates.
    */
    int count = 0;
    for (int i = y - 1; i <= y + 1; i++) {
        for (int j = x - 1; j <= x + 1; j++) {
            if (!(j == x && i == y)) {
                count += this->map[i][j];
            }
        }
    }
    return count;
}

void Caves::cellularAutomata() {
    /*
        Iterate through array and change states of cells.
    */
    int neighbours;
    for (int y = 1; y < this->mapHeight - 1; y++) {
        for (int x = 1; x < this->mapWidth - 1; x++) {
            neighbours = this->countNeighbours(x, y);
            if (map[y][x] == DEAD && neighbours >= 6) {
                map[y][x] = ALIVE;
            } else if (map[y][x] == ALIVE && neighbours <= 3) {
                map[y][x] = DEAD;
            }
        }
    }
}

void Caves::switchStates() {
    /*
        Switch states from 0s to 1s and vice versa.
        I was too lazy to make walls as 1s and empty spaces as 0s from the beginning,
        so this is necessary.
    */
    for (int y = 0; y < this->mapHeight; y++) {
        for (int x = 0; x < this->mapWidth; x++) {
            this->map[y][x] = !this->map[y][x];
        }
    }
}

// ----- CONSTRUCTORS/DESTRUCTORS ----- //
Caves::Caves() {
    this->map = nullptr;
}

Caves::~Caves() {
    delete this->map;
}

// ----- FUNCTIONS ----- //
void Caves::initArray(int mapWidth, int mapHeight, int aliveProb) {
    /*
        Create an array with 0s and 1s randomly. Borders have value 0.
        Bottom border has 6 rows of 0s.
    */
    delete this->map;
    std::srand(time(NULL));
    this->mapWidth = mapWidth;
    this->mapHeight = mapHeight;

    this->map = new bool*[mapHeight];
    for (int y = 0; y < mapHeight; y++) {
        this->map[y] = new bool[mapWidth];
        for (int x = 0; x < mapWidth; x++) {
            if (y == 0 || x == 0 || y >= mapHeight - 6 || x == mapWidth - 1) {
                this->map[y][x] = DEAD;
            } else if (rand() % 100 <= aliveProb) {
                this->map[y][x] = ALIVE;
            } else {
                this->map[y][x] = DEAD;
            }
        }
    }
}

void Caves::generateCaves(int generations) {
    for (int gen = 0; gen < generations; gen++) {
        this->cellularAutomata();
    }
    this->switchStates();
}

bool Caves::getCell(int x, int y) {
    /*
        Get cell data with given coordinates.
    */
    return this->map[y][x];
}
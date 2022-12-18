#include <cstdlib>
#include <ctime>

class Caves {
    private:
        // ----- VARIABLES ----- //
        bool** map;
        const bool ALIVE = 1;
        const bool DEAD = 0;
        int mapWidth;
        int mapHeight;

        // ----- PRIVATE FUNCTIONS ----- //
        int countNeighbours(int x, int y);
        void cellularAutomata();
        void switchStates();

    public:
        // ----- CONSTRUCTORS/DESTRUCTORS ----- //
        Caves();
        virtual ~Caves();

        // ----- FUNCTIONS ----- //
        void initArray(int mapWidth, int mapHeght, int aliveProb);
        void generateCaves(int generations);
        bool getCell(int x, int y);
};

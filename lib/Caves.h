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
        int countNeighbours(int, int);
        void cellularAutomata();
        void switchStates();

    public:
        // ----- CONSTRUCTORS/DESTRUCTORS ----- //
        Caves();
        virtual ~Caves();

        // ----- FUNCTIONS ----- //
        void initArray(int, int, int);
        bool** generateCaves(int);
};

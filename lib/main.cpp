#include <ctime>
#include <iostream>

#include "App.h"
#include "PerlinNoise.hpp"

int main() {
    // ----- CREATE APP OBJECT ----- //
    const siv::PerlinNoise::seed_type seed = time(NULL);
    const siv::PerlinNoise perlin{seed};
    for (int x = 0; x < 10; ++x)
    {
        const double noise = perlin.octave1D((x * 0.01), 1, 4);
        
        std::cout << noise << ' ';
    }
    App app;

    // ----- MAIN LOOP ----- //
    while (app.isRunning()) {
        app.update();

        app.render();
    }

    return 0;
}

#include "App.h"

int main() {
    // ----- CREATE APP OBJECT ----- //
    App app;

    // ----- MAIN LOOP ----- //
    while (app.isRunning()) {
        app.update();

        app.render();
    }

    return 0;
}

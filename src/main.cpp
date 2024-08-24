#include <SDL.h>
#include <stdexcept>

#include "GameManager.hpp"

int main(int argv, char** args) {
    (void) argv;
    (void) args;

    GameManager game;
    try {
        game.run();
    } catch (const std::runtime_error &e) {
        SDL_Log("Error: %s", e.what());
        return 1;
    }

    return 0;
}
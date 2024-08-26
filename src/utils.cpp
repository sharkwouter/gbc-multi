#include "utils.hpp"

#include <SDL.h>

std::string getResourcePath(std::string file) {
    std::string path = "";

    char * base_path = SDL_GetBasePath();
    if (base_path) {
        path += base_path;
        SDL_free(base_path);
    }

    path += file;

    return path;
}

std::string getRomPath() {
    return getResourcePath("roms");
}

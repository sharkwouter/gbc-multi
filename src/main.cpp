#include <SDL.h>
#include <stdio.h>
#include <libgbc/machine.hpp>
#include <vector>

#include "Player.hpp"

bool running = true;
std::vector<uint8_t> romdata;
std::vector<Player*> players;


static inline std::vector<uint8_t> load_file(const std::string& filename)
{
    size_t size = 0;
    FILE* f = fopen(filename.c_str(), "rb");
    if (f == NULL) throw std::runtime_error("Could not open file: " + filename);

    fseek(f, 0, SEEK_END);
    size = ftell(f);
    fseek(f, 0, SEEK_SET);

    std::vector<uint8_t> result(size);
    if (size != fread(result.data(), 1, size, f))
    {
        fclose(f);
        throw std::runtime_error("Error when reading from file: " + filename);
    }
    fclose(f);
    return result;
}


void addPlayer(SDL_JoystickID id) {
    players.push_back(new Player(id, &romdata));
}

void cleanupPlayers() {
    std::vector<Player*> new_players;
    for(Player * player : players) {
        if(player->isConnected()) {
            new_players.push_back(player);
        } else {
            SDL_Log("Removing player: %i", player->getPlayerIndex());
            delete player;
        }
    }
    players = new_players;
}

void handleGamepadInputEvent(SDL_ControllerButtonEvent cbutton) {
    switch (cbutton.button)
    {
        case SDL_CONTROLLER_BUTTON_START:
            running = false;
            break;
        case SDL_CONTROLLER_BUTTON_A:
            SDL_Log("Controller %i pressed A", cbutton.which);
            break;
    }
}

void handleKeyboardDownInputEvent(SDL_Keycode key) {
    switch (key)
    {
        case SDLK_ESCAPE:
            running = false;
            break;
    }
}

int main(int argv, char** args) {
    if (argv != 2) {
        printf("Please specify a game\n");
        exit(5);
    }
    romdata = load_file(args[1]);

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER | SDL_INIT_TIMER) != 0) {
        printf("couldn't init SDL: %s", SDL_GetError());
        exit(1);
    }

    SDL_Window * window = SDL_CreateWindow("GBC-Multi", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 144, 160, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        printf("couldn't create window: %s", SDL_GetError());
        exit(2);
    }

    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        printf("couldn't create renderer: %s", SDL_GetError());
    }

    SDL_Event event;
    while(running) {

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_CONTROLLERDEVICEADDED:
                    addPlayer(event.cdevice.which);
                    break;
                case SDL_CONTROLLERDEVICEREMOVED:
                    cleanupPlayers();
                    break;
                case SDL_CONTROLLERBUTTONDOWN:
                    handleGamepadInputEvent(event.cbutton);
                    break;
                case SDL_KEYDOWN:
                    handleKeyboardDownInputEvent(event.key.keysym.sym);
                    break;
            }
        }
        for(Player * player : players) {
            player->update();
        }

        for(Player * player : players) {
            player->render(renderer);
        }
    }
    for(Player * player : players) {
        delete player;
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
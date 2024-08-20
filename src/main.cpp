#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <libgbc/machine.hpp>
#include <vector>

#include "Player.hpp"
#include "FontManager.hpp"
#include "FontType.hpp"
#include "constants.hpp"


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

void add_input(int gamepad_id, uint8_t button) {
    for(Player * player : players) {
        if (player->getGamepadId() == gamepad_id) {
            player->add_input(button);
        }
    }
}

void remove_input(int gamepad_id, uint8_t button) {
    for(Player * player : players) {
        if (player->getGamepadId() == gamepad_id) {
            player->remove_input(button);
        }
    }
}

void handleGamepadPressEvent(SDL_ControllerButtonEvent cbutton) {
    switch (cbutton.button)
    {
        case SDL_CONTROLLER_BUTTON_BACK:
            add_input(cbutton.which, gbc::BUTTON_SELECT);
            break;
        case SDL_CONTROLLER_BUTTON_START:
            add_input(cbutton.which, gbc::BUTTON_START);
            break;
        case SDL_CONTROLLER_BUTTON_A:
            add_input(cbutton.which, gbc::BUTTON_A);
            break;
        case SDL_CONTROLLER_BUTTON_B:
            add_input(cbutton.which, gbc::BUTTON_B);
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_UP:
            add_input(cbutton.which, gbc::DPAD_UP);
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
            add_input(cbutton.which, gbc::DPAD_DOWN);
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
            add_input(cbutton.which, gbc::DPAD_LEFT);
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
            add_input(cbutton.which, gbc::DPAD_RIGHT);
            break;
    }
}

void handleGamepadReleaseEvent(SDL_ControllerButtonEvent cbutton) {
    switch (cbutton.button)
    {
        case SDL_CONTROLLER_BUTTON_BACK:
            remove_input(cbutton.which, gbc::BUTTON_SELECT);
            break;
        case SDL_CONTROLLER_BUTTON_START:
            remove_input(cbutton.which, gbc::BUTTON_START);
            break;
        case SDL_CONTROLLER_BUTTON_A:
            remove_input(cbutton.which, gbc::BUTTON_A);
            break;
        case SDL_CONTROLLER_BUTTON_B:
            remove_input(cbutton.which, gbc::BUTTON_B);
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_UP:
            remove_input(cbutton.which, gbc::DPAD_UP);
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
            remove_input(cbutton.which, gbc::DPAD_DOWN);
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
            remove_input(cbutton.which, gbc::DPAD_LEFT);
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
            remove_input(cbutton.which, gbc::DPAD_RIGHT);
            break;
    }
}

void handleKeyboardDownPressEvent(SDL_Keycode key) {
    switch (key)
    {
        case SDLK_ESCAPE:
            running = false;
            break;
        case SDLK_BACKSPACE:
            add_input(-1, gbc::BUTTON_SELECT);
            break;
        case SDLK_RETURN:
            add_input(-1, gbc::BUTTON_START);
            break;
        case SDLK_z:
            add_input(-1, gbc::BUTTON_A);
            break;
        case SDLK_x:
            add_input(-1, gbc::BUTTON_B);
            break;
        case SDLK_UP:
            add_input(-1, gbc::DPAD_UP);
            break;
        case SDLK_DOWN:
            add_input(-1, gbc::DPAD_DOWN);
            break;
        case SDLK_LEFT:
            add_input(-1, gbc::DPAD_LEFT);
            break;
        case SDLK_RIGHT:
            add_input(-1, gbc::DPAD_RIGHT);
            break;
    }
}

void handleKeyboardDownReleaseEvent(SDL_Keycode key) {
    switch (key)
    {
        case SDLK_ESCAPE:
            running = false;
            break;
        case SDLK_BACKSPACE:
            remove_input(-1, gbc::BUTTON_SELECT);
            break;
        case SDLK_RETURN:
            remove_input(-1, gbc::BUTTON_START);
            break;
        case SDLK_z:
            remove_input(-1, gbc::BUTTON_A);
            break;
        case SDLK_x:
            remove_input(-1, gbc::BUTTON_B);
            break;
        case SDLK_UP:
            remove_input(-1, gbc::DPAD_UP);
            break;
        case SDLK_DOWN:
            remove_input(-1, gbc::DPAD_DOWN);
            break;
        case SDLK_LEFT:
            remove_input(-1, gbc::DPAD_LEFT);
            break;
        case SDLK_RIGHT:
            remove_input(-1, gbc::DPAD_RIGHT);
            break;
    }
}

int main(int argv, char** args) {
    if (argv != 2) {
        printf("Please specify a game\n");
        exit(1);
    }
    romdata = load_file(args[1]);

    if (TTF_Init() == -1) {
        printf("couldn't init SDL_ttf: %s\n", TTF_GetError());
        exit(2);
    }

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER | SDL_INIT_TIMER) != 0) {
        printf("couldn't init SDL: %s", SDL_GetError());
        exit(3);
    }

    SDL_Window * window = SDL_CreateWindow("GBC-Multi", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, START_SCREEN_WIDTH, START_SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
    if (window == nullptr) {
        printf("couldn't create window: %s", SDL_GetError());
        exit(4);
    }

    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        printf("couldn't create renderer: %s", SDL_GetError());
        exit(5);
    }

    FontManager * font_manager = new FontManager();
    SDL_Texture * press_start = nullptr;

    SDL_Event event;
    while(running) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
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
                    handleGamepadPressEvent(event.cbutton);
                    break;
                case SDL_CONTROLLERBUTTONUP:
                    handleGamepadReleaseEvent(event.cbutton);
                    break;
                case SDL_KEYDOWN:
                    handleKeyboardDownPressEvent(event.key.keysym.sym);
                    break;
                case SDL_KEYUP:
                    handleKeyboardDownReleaseEvent(event.key.keysym.sym);
                    break;
            }
        }
        SDL_Rect window_rect;
        SDL_GetWindowSize(window, &window_rect.w, &window_rect.h);

        if (players.size() == 0) {
            if (!press_start) {
                press_start = font_manager->getTexture(renderer, "Press start/enter", FontType::TITLE,  {0, 0, 0, 255});
            }
            SDL_Rect press_start_rect;
            SDL_QueryTexture(press_start, NULL, NULL, &press_start_rect.w, &press_start_rect.h);
            press_start_rect.x = window_rect.w/2 - press_start_rect.w/2;
            press_start_rect.y = window_rect.h/2 - press_start_rect.h/2;
            SDL_RenderCopy(renderer, press_start, NULL, &press_start_rect);
        }

        for(Player * player : players) {
            player->update();
        }

        for(int i = 0; i < players.size(); i++) {
            SDL_Rect dst;
            dst.x = i * window_rect.w/players.size();
            dst.y = 0;
            dst.w = window_rect.w/players.size();
            dst.h = window_rect.h;
            players[i]->render(renderer, &dst);
        }
        SDL_RenderPresent(renderer);
    }
    for(Player * player : players) {
        delete player;
    }
    free(font_manager);
    if (press_start) {
        SDL_DestroyTexture(press_start);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    TTF_Quit();
    return 0;
}
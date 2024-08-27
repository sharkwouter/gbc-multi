#include "PlayerManager.hpp"

#include <memory.h>

#include "screen/ScreenSelectRom.hpp"
#include "screen/ScreenSelectName.hpp"

PlayerManager::PlayerManager(FontManager * font_manager) {
    this->font_manager = font_manager;
}

PlayerManager::~PlayerManager() {
    SDL_Log("Cleanup for player with index %i", this->gamepad_id);
    this->disconnect();
}

void PlayerManager::handleInput(Input input) {
    if (!this->active) {
        if (input.type == InputType::START || input.type == InputType::A) {
            SDL_Log("Activating player with controller id %i", this->getGamepadId());
            this->active = true;
            this->switchScreen(ScreenType::SELECT_NAME);
        }
        return;
    }

    switch(this->current_screen_type) {
        case ScreenType::SELECT_NAME:
            if(input.type == InputType::START) {
                this->name = ((ScreenSelectName*) this->screen)->getSelectedName();
                if (!this->name.empty()) {
                    SDL_Log("Name is %s!", this->name.c_str());
                    this->switchScreen(ScreenType::SELECT_ROM);
                }
            } else {
                this->screen->handleInput(input);
            }
            break;
        case ScreenType::SELECT_ROM:
            if(input.type == InputType::B) {
                this->active = false;
            } else if (input.type == InputType::A) {
                this->rom = ((ScreenSelectRom*) this->screen)->getSelectedRom();

                SDL_Log("Selected %s", this->rom.c_str());
            } else {
                this->screen->handleInput(input);
            }
            break;
        default:
            break;
    }
    
}

void PlayerManager::update() {
    if (!this->active) {
        return;
    }
    this->screen->update();
}

void PlayerManager::draw(SDL_Renderer * renderer, SDL_Rect * dst_rect) {
    if (!this->active) {
        return;
    }

    this->screen->draw(renderer, dst_rect);
}

int PlayerManager::getGamepadId() {
    return this->gamepad_id;
}


bool PlayerManager::isConnected() {
    if (this->gamepad_id == KEYBOARD_ID) {
        return true;
    }
    if (this->gamepad != nullptr) {
        return (SDL_GameControllerGetAttached(this->gamepad) == SDL_TRUE);
    }
    return false;
}

void PlayerManager::connect(SDL_JoystickID id) {
    this->disconnect();
    if(id == KEYBOARD_ID) {
        this->gamepad_id = KEYBOARD_ID;
        SDL_Log("Added player with keyboard");
    } else {
        if (SDL_IsGameController(id)) {
            this->gamepad = SDL_GameControllerOpen(id);
            SDL_Joystick * joystick = SDL_GameControllerGetJoystick(this->gamepad);
            this->gamepad_id = SDL_JoystickInstanceID(joystick);
            SDL_Log("Added player with controller %i named %s", this->gamepad_id, SDL_GameControllerName(this->gamepad));
        } else {
            SDL_Log("Could not connect controller with id %i", id);
        }
    }
}

void PlayerManager::disconnect() {
    if(this->gamepad != nullptr) {
        SDL_GameControllerClose(this->gamepad);
    }
    this->gamepad_id = NO_INPUT;
    this->active = false;
}

void PlayerManager::switchScreen(ScreenType next_screen_type) {
    if (next_screen_type == this->current_screen_type) {
        return;
    }

    switch (next_screen_type) {
        case ScreenType::SELECT_ROM:
            this->clearScreen();
            screen = new ScreenSelectRom(font_manager);
            this->current_screen_type = next_screen_type;
            break;
        case ScreenType::SELECT_NAME:
            this->clearScreen();
            screen = new ScreenSelectName(font_manager);
            this->current_screen_type = next_screen_type;
            break;
        default:
            break;
    }
}

void PlayerManager::clearScreen() {
    if (this->screen) {
        free(this->screen);
    }
    this->screen = nullptr;
}

#include "PlayerManager.hpp"

#include <memory.h>

PlayerManager::PlayerManager() {
}

PlayerManager::~PlayerManager() {
    SDL_Log("Cleanup for player with index %i", this->gamepad_id);
    this->disconnect();
}

void PlayerManager::handleInput(Input input) {
    if (!this->active && input.type == InputType::START) {
        SDL_Log("Activating player with controller id %i", this->getGamepadId());
        this->active = true;
    }
}

void PlayerManager::update() {

}

void PlayerManager::draw(SDL_Renderer * renderer, SDL_Rect * dst_rect) {
    SDL_Rect texture_rect = {0, 0, 160, 144};
    if (this->texture == nullptr) {
        this->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_BGR555, SDL_TEXTUREACCESS_STREAMING, texture_rect.w, texture_rect.h);
    }
    SDL_RenderCopy(renderer, this->texture, &texture_rect, dst_rect);
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
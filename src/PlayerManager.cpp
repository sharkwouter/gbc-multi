#include "PlayerManager.hpp"

#include <memory.h>

PlayerManager::PlayerManager() {

}

PlayerManager::~PlayerManager() {
    
}

void PlayerManager::handleInput(Input input) {
    if (!this->active && input.type == InputType::START && !input.released) {
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
    if (this->gamepad == nullptr) {
        return -1;
    }
}

#include "ScreenPlay.hpp"

#include <filesystem>
#include <memory.h>

#include "../utils.hpp"
#include "../InputType.hpp"
#include "../constants.hpp"

ScreenPlay::ScreenPlay(std::string rom) {
    
}

ScreenPlay::~ScreenPlay() {
}

void ScreenPlay::handleInput(Input input) {
    switch (input.type)
    {
        case InputType::A:
        case InputType::B:
        case InputType::LEFT:
        case InputType::RIGHT:
        case InputType::UP:
        case InputType::DOWN:
        default:
            break;
    }
}

void ScreenPlay::update() {

}

void ScreenPlay::draw(SDL_Renderer * renderer, SDL_Rect * dst_rect) {

}

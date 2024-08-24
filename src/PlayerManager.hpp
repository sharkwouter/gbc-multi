#ifndef PLAYERCONTROLLER_HPP
#define PLAYERCONTROLLER_HPP

#include <SDL.h>
#include <vector>

#include "Input.hpp"

class PlayerManager {

private:
    SDL_GameController * gamepad = nullptr;
    SDL_Texture * texture = nullptr;

    bool active = false;
public:
    PlayerManager();
    ~PlayerManager();

    bool isActive() {return this->active;}

    int getGamepadId();

    void handleInput(Input input);
    void update();
    void draw(SDL_Renderer * renderer, SDL_Rect * dst_rect);
};

#endif // PLAYERCONTROLLER_HPP
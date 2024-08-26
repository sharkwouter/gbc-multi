#ifndef PLAYERCONTROLLER_HPP
#define PLAYERCONTROLLER_HPP

#include <SDL.h>
#include <vector>

#include "Input.hpp"
#include "constants.hpp"

class PlayerManager {

private:
    SDL_GameController * gamepad = nullptr;
    SDL_Texture * texture = nullptr;

    bool active = false;
    int gamepad_id = NO_INPUT;
public:
    PlayerManager();
    ~PlayerManager();

    void connect(SDL_JoystickID id);
    void disconnect();
    bool isConnected();
    bool isActive() {return this->active;}
    int getGamepadId();

    void handleInput(Input input);
    void update();
    void draw(SDL_Renderer * renderer, SDL_Rect * dst_rect);
};

#endif // PLAYERCONTROLLER_HPP
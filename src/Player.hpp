#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SDL.h>
#include <libgbc/machine.hpp>
#include <vector>

class Player {

private:
    SDL_GameController * gamepad = nullptr;
    gbc::Machine * machine = nullptr;
    SDL_Texture * texture = nullptr;

    SDL_JoystickID gamepad_id  = -1;
public:
    Player(SDL_JoystickID id, std::vector<uint8_t> * romdata);
    ~Player();

    int getPlayerIndex();
    SDL_JoystickID getGamepadId() {return gamepad_id;}
    bool isConnected();

    void update();
    void render(SDL_Renderer * renderer);
};

#endif // PLAYER_HPP
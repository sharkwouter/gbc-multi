#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SDL.h>
#include <libgbc/machine.hpp>
#include <vector>

class Player {

private:
    SDL_GameController * gamepad = nullptr;
    gbc::Machine * machine = nullptr;
    
    SDL_JoystickID joystick_id  = -1;
public:
    Player(SDL_JoystickID id, std::vector<uint8_t> * romdata);
    ~Player();

    int getIndex();
    SDL_JoystickID getId() {return joystick_id;}
    bool isConnected();

    void update();
    void render(SDL_Texture * target);
};

#endif // PLAYER_HPP
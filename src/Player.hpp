#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SDL.h>
#include <libgbc/machine.hpp>
#include <vector>

class Player {

private:
    SDL_GameController * gamepad = nullptr;
    std::vector<uint8_t> * romdata;
    gbc::Machine * machine = nullptr;
    SDL_Texture * texture = nullptr;

    SDL_JoystickID gamepad_id  = -1;
    uint8_t input = 0;

    void renderMachine(SDL_Renderer * renderer, SDL_Rect * dst_rect);
public:
    Player(SDL_JoystickID id, std::vector<uint8_t> * romdata);
    ~Player();

    int getPlayerIndex();
    SDL_JoystickID getGamepadId() {return gamepad_id;}
    bool isConnected();

    void add_input(uint8_t button);
    void remove_input(uint8_t button);

    void update();
    void render(SDL_Renderer * renderer, SDL_Rect * dst_rect);
};

#endif // PLAYER_HPP
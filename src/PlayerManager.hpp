#ifndef PLAYERCONTROLLER_HPP
#define PLAYERCONTROLLER_HPP

#include <SDL.h>
#include <vector>

#include "Input.hpp"
#include "constants.hpp"
#include "FontManager.hpp"
#include "ScreenType.hpp"
#include "screen/ScreenBase.hpp"

class PlayerManager {

private:
    SDL_GameController * gamepad = nullptr;
    SDL_Texture * texture = nullptr;
    FontManager * font_manager = nullptr;

    bool active = false;
    int gamepad_id = NO_INPUT;

    ScreenBase * screen = nullptr;
    ScreenType current_screen_type = ScreenType::NONE;
public:
    PlayerManager(FontManager * font_manager);
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
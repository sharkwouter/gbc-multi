#ifndef SCREENPLAY_HPP
#define SCREENPLAY_HPP

#include <SDL.h>
#include <string>

#include "ScreenBase.hpp"

class ScreenPlay : public ScreenBase {
private:

public:
    ScreenPlay(std::string rom);
    ~ScreenPlay();

    void handleInput(Input input);
    void update();
    void draw(SDL_Renderer * renderer, SDL_Rect * dst_rect);
};

#endif // SCREENPLAY_HPP
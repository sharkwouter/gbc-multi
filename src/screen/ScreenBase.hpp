#ifndef SCREENBASE_HPP
#define SCREENBASE_HPP

#include <SDL.h>

#include "../Input.hpp"

class ScreenBase {
public:
    virtual ~ScreenBase() {};

    virtual void handleInput(Input input) = 0;
    virtual void update() = 0;
    virtual void draw(SDL_Renderer * renderer, SDL_Rect * dst_rect) = 0;
};

#endif // SCREENBASE_HPP
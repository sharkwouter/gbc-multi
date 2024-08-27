#ifndef SCREENSELECTROM_HPP
#define SCREENSELECTROM_HPP

#include <SDL.h>
#include <vector>
#include <string>
#include <regex>

#include "ScreenBase.hpp"
#include "../Input.hpp"
#include "../FontManager.hpp"

class ScreenSelectRom : public ScreenBase {
private:
    FontManager * font_manager = nullptr;

    std::regex rom_regex{"^.*\\.gb[ac]?$"};

    std::vector<std::string> roms;
    std::vector<SDL_Texture*> texts;
    int selected = 0;

    SDL_Texture * no_roms_found_texture = nullptr;

    void resetTexts();
    void getRoms();
public:
    ScreenSelectRom(FontManager * font_manager);
    ~ScreenSelectRom();

    std::string getSelectedRom();

    void handleInput(Input input);
    void update();
    void draw(SDL_Renderer * renderer, SDL_Rect * dst_rect);
};

#endif // SCREENSELECTROM_HPP
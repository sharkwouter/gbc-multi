#include "ScreenSelectRom.hpp"

#include <filesystem>

#include "../utils.hpp"
#include "../InputType.hpp"

ScreenSelectRom::ScreenSelectRom(FontManager * font_manager) {
    this->font_manager = font_manager;
    this->getRoms();
}

ScreenSelectRom::~ScreenSelectRom() {

}

void ScreenSelectRom::handleInput(Input input) {
    switch (input.type)
    {
    case InputType::UP:
        selected -= 1;
        if (selected < 0) {
            selected = 0;
        }
        this->resetTexts();
        break;
    case InputType::DOWN:
        selected += 1;
        if (selected >= (int) this->roms.size()) {
            selected = (int) this->roms.size() -1;
        }
        this->resetTexts();
        break;
    default:
        break;
    }
}

void ScreenSelectRom::update() {

}

void ScreenSelectRom::draw(SDL_Renderer * renderer, SDL_Rect * dst_rect) {
    for(int i = this->selected; i < (int) this->roms.size(); i++) {
        if (this->texts[i] == nullptr) {
            if (this->selected == i) {
                this->texts[i] = this->font_manager->getTexture(renderer, this->roms[i], FontType::TITLE, {255, 0, 0, 255});
            } else {
                this->texts[i] = this->font_manager->getTexture(renderer, this->roms[i], FontType::TITLE, {0, 0, 0, 255});
            }
        }
        SDL_Rect text_rect;
        SDL_QueryTexture(this->texts[i], NULL, NULL, &text_rect.w, &text_rect.h);
        text_rect.x = dst_rect->x;
        text_rect.y = dst_rect->y + text_rect.h*(i-selected);
        text_rect.w = std::min(text_rect.w, dst_rect->w);
        if ((text_rect.y + text_rect.h) > (dst_rect->y + dst_rect->h)) {
            continue;
        }
        SDL_RenderCopy(renderer, this->texts[i], NULL, &text_rect);
    }
}

void ScreenSelectRom::resetTexts() {
    for(int i = 0; i < (int) this->texts.size(); i++) {
        if (this->texts[i] != nullptr) {
            SDL_DestroyTexture(this->texts[i]);
            this->texts[i] = nullptr;
        }
    }
}

void ScreenSelectRom::getRoms() {
    this->resetTexts();
    this->roms.clear();

    std::filesystem::directory_iterator rom_iterator(getRomPath());
    SDL_Log("Looking for roms");
    for (auto &rom : rom_iterator) {
        if (std::regex_match(rom.path().string(), rom_regex)) {
            std::string name = rom.path().stem().string();
            SDL_Log("Found rom: %s", name.c_str());
            this->roms.push_back(name);
            this->texts.push_back(nullptr);
        }
    }
}

std::string ScreenSelectRom::getSelectedRom() {
    return getRomPath() + "/" + this->roms[this->selected];
}

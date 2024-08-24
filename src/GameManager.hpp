#ifndef GAMEMANAGER_HPP
#define GAMEMANAGER_HPP

#include <SDL.h>

class GameManager {

private:
    SDL_Window * window = nullptr;
    SDL_Renderer * renderer = nullptr;

    void createRenderer();
    void createWindow();
public:
    GameManager();
    ~GameManager();

    void run();
};

#endif // GAMEMANAGER_HPP
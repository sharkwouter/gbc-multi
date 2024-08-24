#include "InputManager.hpp"

InputManager::InputManager() {

}

InputManager::~InputManager() {

}

void InputManager::getInputs(std::vector<Input> * inputs) {
    inputs->clear();
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                this->quit_triggered = true;
                break;
            case SDL_CONTROLLERDEVICEADDED:
                this->controller_event_triggered = true;
                break;
            case SDL_CONTROLLERDEVICEREMOVED:
                this->controller_event_triggered = true;
                break;
            case SDL_CONTROLLERBUTTONDOWN:
                {
                    Input input = this->getGamepadInput(event.cbutton, false);
                    if (input.type != InputType::NONE) {
                        inputs->push_back(input);
                    }
                }
                break;
            case SDL_CONTROLLERBUTTONUP:
                {
                    Input input = this->getGamepadInput(event.cbutton, true);
                    if (input.type != InputType::NONE) {
                        inputs->push_back(input);
                    }
                }
                break;
            case SDL_KEYDOWN:
                {
                    Input input = this->getKeyboardInput(event.key.keysym.sym, false);
                    if (input.type != InputType::NONE) {
                        inputs->push_back(input);
                    }
                }
                break;
            case SDL_KEYUP:
                {
                    Input input = this->getKeyboardInput(event.key.keysym.sym, true);
                    if (input.type != InputType::NONE) {
                        inputs->push_back(input);
                    }
                }
                break;
        }
    }
}

Input InputManager::getGamepadInput(SDL_ControllerButtonEvent event, bool released) {
    Input input;
    input.gamepad = event.which;
    input.released = released;

    switch (event.button)
    {
        case SDL_CONTROLLER_BUTTON_BACK:
            input.type = InputType::SELECT;
            break;
        case SDL_CONTROLLER_BUTTON_START:
            input.type = InputType::START;
            break;
        case SDL_CONTROLLER_BUTTON_A:
            input.type = InputType::A;
            break;
        case SDL_CONTROLLER_BUTTON_B:
            input.type = InputType::B;
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_UP:
            input.type = InputType::UP;
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
            input.type = InputType::DOWN;
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
            input.type = InputType::LEFT;
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
            input.type = InputType::RIGHT;
            break;
        default:
            input.type = InputType::NONE;
    }

    return input;
}

Input InputManager::getKeyboardInput(SDL_Keycode button, bool released) {
    Input input;
    input.gamepad = -1;
    input.released = released;

    switch (button)
    {
        case SDLK_ESCAPE:
            this->quit_triggered = true;
            input.type = InputType::NONE;
            break;
        case SDLK_BACKSPACE:
            input.type = InputType::SELECT;
            break;
        case SDLK_RETURN:
            input.type = InputType::START;
            break;
        case SDLK_z:
            input.type = InputType::A;
            break;
        case SDLK_x:
            input.type = InputType::B;
            break;
        case SDLK_UP:
            input.type = InputType::UP;
            break;
        case SDLK_DOWN:
            input.type = InputType::DOWN;
            break;
        case SDLK_LEFT:
            input.type = InputType::LEFT;
            break;
        case SDLK_RIGHT:
            input.type = InputType::RIGHT;
            break;
        default:
            input.type = InputType::NONE;
    }

    return input;
}


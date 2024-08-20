#include "Player.hpp"

Player::Player(SDL_JoystickID id, std::vector<uint8_t> * romdata) {
    if (SDL_IsGameController(id)) {
        this->gamepad = SDL_GameControllerOpen(id);
        SDL_Joystick * joystick = SDL_GameControllerGetJoystick(this->gamepad);
        this->gamepad_id = SDL_JoystickInstanceID(joystick);
        SDL_Log("Added player %i with controller %i named %s", this->getPlayerIndex(), this->gamepad_id, SDL_GameControllerName(this->gamepad));
    }
    this->machine = new gbc::Machine(*romdata);
}

Player::~Player() {
    SDL_Log("Cleanup for player with index %i", this->getPlayerIndex());
    SDL_GameControllerClose(this->gamepad);
    SDL_DestroyTexture(this->texture);
    free(this->machine);
}

void Player::update() {
    this->machine->set_inputs(this->input);
}

void Player::render(SDL_Renderer * renderer, SDL_Rect * dst_rect) {
    static SDL_Rect texture_rect = {0, 0, 160, 144};
    if (this->texture == nullptr) {
        this->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_BGR555, SDL_TEXTUREACCESS_STREAMING, 160, 144);
    }
    if(this->machine->is_running()) {
        this->machine->simulate_one_frame();
    }
    if (this->machine->is_cgb()) { 
        // This is a color rom, just copy the pixels from the gpu into the texture
        SDL_UpdateTexture(this->texture, &texture_rect, &this->machine->gpu.pixels()[0], 320); // The 320 is the width of a gameboy screen (160) times the pixel size in bytes (2)
    } else {
        uint16_t pixels[160*144];
        for(int i = 0; i < 160*144; i++) {
              pixels[i] = this->machine->gpu.getpal(i*2) | (this->machine->gpu.getpal(i*2+1) << 8);
        }
        SDL_UpdateTexture(this->texture, &texture_rect, pixels, 320); // The 320 is the width of a gameboy screen (160) times the pixel size in bytes (2)
    }
    SDL_RenderCopy(renderer, this->texture, &texture_rect, dst_rect);
}

void Player::add_input(uint8_t button) {
    this->input |= button;
}

void Player::remove_input(uint8_t button) {
    if (this->input | button) {
        this->input ^= button;
    }
}

int Player::getPlayerIndex() {
    if(this->gamepad) {
        int player_index = SDL_GameControllerGetPlayerIndex(this->gamepad);
        if (player_index >= 0) {
            return player_index + 1;
        } else {
            return player_index;
        }
    } else {
        // A keyboard player also has to have an index
        return 0;
    }
}

bool Player::isConnected() {
    return (SDL_GameControllerGetAttached(this->gamepad) == SDL_TRUE);
}


#include "./input.h"

input_t *create_input() {
    input_t *input = (input_t *)malloc(sizeof(input_t));
    input->quit = false;
    input->keystate = SDL_GetKeyboardState(NULL);
    return input;
}

void destroy_input(input_t *input) { free(input); }

void poll_input(input_t *input) {
    while (SDL_PollEvent(&input->event)) {
        switch (input->event.type) {
        case SDL_EVENT_QUIT:
        case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
            input->quit = true;
            break;
        default:
            break;
        }
    }
}

bool is_keydown_input(input_t *input, SDL_Keycode code) {
    SDL_Scancode scancode = SDL_GetScancodeFromKey(code);
    return input->keystate[scancode];
}

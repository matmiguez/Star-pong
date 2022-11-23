#include "InputModule.h"
#include "StructsDef.h"

void onKeyUp(int keyCode, InputState& input) {
    switch (keyCode) {
    case SDLK_UP:
        input.up = false;
        break;
    case SDLK_DOWN:
        input.down = false;
        break;
    case SDLK_w:
        input.up1 = false;
        break;
    case SDLK_s:
        input.down1 = false;
        break;
    case SDLK_SPACE:
        input.fire = false;
        break;
    case SDLK_BACKSPACE:
        input.stop = false;
        break;
    case SDLK_1:
        input.one = false;
        break;
    case SDLK_2:
        input.two = false;
        break;
    default:
        break;
    }
}

void onKeyDown(int keyCode, InputState& input) {
    switch (keyCode) {
    case SDLK_UP:
        input.up = true;
        break;
    case SDLK_DOWN:
        input.down = true;
        break;
    case SDLK_w:
        input.up1 = true;
        break;
    case SDLK_s:
        input.down1 = true;
        break;
    case SDLK_SPACE:
        input.fire = true;
        break;
    case SDLK_BACKSPACE:
        input.stop = true;
        break;
    case SDLK_1:
        input.one = true;
        break;
    case SDLK_2:
        input.two = true;
        break;
    default:
        break;
    }
}
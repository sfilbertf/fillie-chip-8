#include "Application.h"

void Application::setKeyState(SDL_Event event) {
    // A key is pressed
    if(event.type == SDL_KEYDOWN) {
        switch(event.key.keysym.sym) {				
            case SDLK_x:
                emu.keypad[0] = 1;
                break;

            case SDLK_1:
                emu.keypad[1] = 1;
                break;

            case SDLK_2:
                emu.keypad[2] = 1;
                break;

            case SDLK_3:
                emu.keypad[3] = 1;
                break;

            case SDLK_q:
                emu.keypad[4] = 1;
                break;

            case SDLK_w:
                emu.keypad[5] = 1;
                break;

            case SDLK_e:
                emu.keypad[6] = 1;
                break;

            case SDLK_a:
                emu.keypad[7] = 1;
                break;

            case SDLK_s:
                emu.keypad[8] = 1;
                break;

            case SDLK_d:
                emu.keypad[9] = 1;
                break;

            case SDLK_y:
                emu.keypad[0xa] = 1;
                break; 

            case SDLK_c:
                emu.keypad[0xb] = 1;
                break;

            case SDLK_4:
                emu.keypad[0xc] = 1;
                break;

            case SDLK_r:
                emu.keypad[0xd] = 1;
                break;

            case SDLK_f:
                emu.keypad[0xe] = 1;
                break;

            case SDLK_v:
                emu.keypad[0xf] = 1;
                break;
        }
    }

    // A key stops being pressed
    else if(event.type == SDL_KEYUP) {
        switch(event.key.keysym.sym) {
            case SDLK_x:
                emu.keypad[0] = 0;
                break;

            case SDLK_1:
                emu.keypad[1] = 0;
                break;

            case SDLK_2:
                emu.keypad[2] = 0;
                break;

            case SDLK_3:
                emu.keypad[3] = 0;
                break;

            case SDLK_q:
                emu.keypad[4] = 0;
                break;

            case SDLK_w:
                emu.keypad[5] = 0;
                break;

            case SDLK_e:
                emu.keypad[6] = 0;
                break;

            case SDLK_a:
                emu.keypad[7] = 0;
                break;

            case SDLK_s:
                emu.keypad[8] = 0;
                break;

            case SDLK_d:
                emu.keypad[9] = 0;
                break;

            case SDLK_y:
                emu.keypad[0xa] = 0;
                break; 

            case SDLK_c:
                emu.keypad[0xb] = 0;
                break;

            case SDLK_4:
                emu.keypad[0xc] = 0;
                break;

            case SDLK_r:
                emu.keypad[0xd] = 0;
                break;

            case SDLK_f:
                emu.keypad[0xe] = 0;
                break;

            case SDLK_v:
                emu.keypad[0xf] = 0;
                break;
        }
    }
}

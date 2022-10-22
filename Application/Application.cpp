#include "Application.h"

void Application::run() {
    startUp();

    bool quit = false;
    while(!quit) {
        quit = eventHandler();

        setUpNewFrame();

        update();

        render();
    }
}

void Application::startUp() {
    emu.resetChip8();
    emu.initOpcodeTable();

    initTextureDisplay();
    initTextureSprite();

    /**
     * Test code
     * 
     */

    emu.loadRom("invaders.ch8");
}

void Application::update() {
    /**
     * Test code
     * 
     */

    int opcodesPerFrame = 9;
    for(int i = 0; i < opcodesPerFrame; i++) {
        emu.fetch();
        emu.decode();
        emu.execute();
        if(emu.firstNibble == 0xd) getSpriteData();
    }
    if(emu.delayTimer > 0) emu.delayTimer--;
    if(emu.soundTimer > 0) emu.soundTimer--;

    ImGui::PushFont(quinqueFive);
    ImGui::PushStyleColor(ImGuiCol_Text, fgColorf);
    // ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    display();
    registers();
    status();
    sprite();
    execution();
    stack();
    
    // ImGui::PopStyleVar();
    ImGui::PopStyleColor();
    ImGui::PopFont();
}

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

bool Application::eventHandler() {
    bool quit = false;

    SDL_Event event;

    while(SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);
        
        setKeyState(event);

        if(event.type == SDL_QUIT) 
            quit = true;
        

        else if(event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
            quit = true;
    }
    
    return quit;  
}

int main() {
    Application app;
    app.run();
}


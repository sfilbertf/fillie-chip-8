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
}

void Application::update() {
    display();
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


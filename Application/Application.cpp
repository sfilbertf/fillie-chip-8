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

}

void Application::update() {

}

bool Application::eventHandler() {
    
}

int main() {
    Application app;
    app.run();
}


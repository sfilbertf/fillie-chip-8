#include "Application.h"

void Application::initTexture() {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    uint8_t data[64 * 32 * 3];
    memset(data, 0, 64 * 32 * 3);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, 64, 32, 0, GL_RGB, GL_UNSIGNED_BYTE, (void*) data);
}

void Application::updateTexture() {
    uint8_t data[64 * 32 * 3];

    int j;
    for(int i = 0; i < 64 * 32; i++) {
        j = i * 3;

        if(emu.disMem[i] != 0) {
            data[j + 0] = fgColoru[0];
            data[j + 1] = fgColoru[1];
            data[j + 2] = fgColoru[2];
        } else {
            data[j + 0] = bgColoru[0];
            data[j + 1] = bgColoru[1];
            data[j + 2] = bgColoru[2];
        }
    }

    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 64, 32, GL_RGB, GL_UNSIGNED_BYTE, (void*) data);
}

void Application::display() {
    updateTexture();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    // ImGuiWindowFlags_NoMove, ImGuiWindowFlags_NoResize
    ImGuiIO& io = ImGui::GetIO();
    ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.35f), ImGuiCond_Always, ImVec2(0.5f,0.5f));
    ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x * 0.5125f, io.DisplaySize.y * 0.625f), ImGuiCond_Always);
    ImGui::Begin("Display", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize); 
    
    ImGui::Image((void*)(intptr_t) texture, ImGui::GetWindowSize());

    ImGui::End();
    ImGui::PopStyleVar();
}
#include "Application.h"

void Application::initTextureDisplay() {
    glGenTextures(1, &textureDisplay);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureDisplay);

    uint8_t data[64 * 32 * 3];
    memset(data, 0, 64 * 32 * 3);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, 64, 32, 0, GL_RGB, GL_UNSIGNED_BYTE, (void*) data);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Application::updateTextureDisplay() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureDisplay);

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

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Application::display() {
    updateTextureDisplay();

    ImGuiIO& io = ImGui::GetIO();
    ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5375f, io.DisplaySize.y * 0.35f), ImGuiCond_Always, ImVec2(0.5f,0.5f));
    ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x * 0.5525f, io.DisplaySize.y * 0.625f), ImGuiCond_Always);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

    ImGui::Begin("Display", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);     
    ImGui::Image((void*)(intptr_t) textureDisplay, ImGui::GetWindowSize());
    ImGui::End();

    ImGui::PopStyleVar();
}
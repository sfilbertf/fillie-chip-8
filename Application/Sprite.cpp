#include "Application.h"

void Application::getSpriteData() {
    memset(spriteData, 0, 8 * 15);

    for(int i = 0; i < emu.n; i++) {
        uint8_t sprite = emu.mem[i + emu.idx];
        for(int j = 0; j < 8; j++) {
            uint8_t currBit = 0x01 & (sprite >> (8 - j- 1));
            spriteData[i * 8 + j] = currBit;
        }
    }
}

void Application::initTextureSprite() {
    glGenTextures(1, &textureSprite);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureSprite);

    uint8_t data[8 * 15 * 3];
    memset(data, 0, 8 * 15 * 3);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, 8, 15, 0, GL_RGB, GL_UNSIGNED_BYTE, (void*) data);

    glBindTexture(GL_TEXTURE_2D, 0);    
}

void Application::updateTextureSprite() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureSprite);

    uint8_t data[8 * 15 * 3];
    
    int j;
    for(int i = 0; i < 8 * 15; i++) {
        j = i * 3;

        if(spriteData[i] != 0) {
            data[j + 0] = fgColoru[0];
            data[j + 1] = fgColoru[1];
            data[j + 2] = fgColoru[2];
        } else {
            data[j + 0] = bgColoru[0];
            data[j + 1] = bgColoru[1];
            data[j + 2] = bgColoru[2];
        }
    }

    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 8, 15, GL_RGB, GL_UNSIGNED_BYTE, (void*) data);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Application::sprite() {
    updateTextureSprite();

    ImGuiIO& io = ImGui::GetIO();
    ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.875f, io.DisplaySize.y * 0.19125), ImGuiCond_Always, ImVec2(0.5f,0.5f));
    ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x * 0.125f, io.DisplaySize.y * 0.25f), ImGuiCond_Always);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

    ImGui::Begin("Sprite", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
    ImGui::Image((void*)(intptr_t) textureSprite, ImGui::GetWindowSize());
    ImGui::End();

    ImGui::PopStyleVar();
}
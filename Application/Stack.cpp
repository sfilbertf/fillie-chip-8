#include "Application.h"

void Application::stack() {
    ImGuiIO& io = ImGui::GetIO();
    ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.9175f, io.DisplaySize.y * 0.6775f), ImGuiCond_Always, ImVec2(0.5f,0.5f));
    ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x * 0.15f, io.DisplaySize.y * 0.625f), ImGuiCond_Always);



    ImGui::PushStyleColor(ImGuiCol_WindowBg, bgColorf);

    ImGui::Begin("Stack", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize); 

    ImGui::TextWrapped("SP:%02X", emu.sp);
    ImGui::Text("\n");
    for(int i = 0; i < 16; i++) {
        if(i == emu.sp) {
            ImGui::PushTextWrapPos();
            ImGui::TextColored(tertColorf, "[%01X] - %04X", i, emu.stack[i]);
            ImGui::PopTextWrapPos();
        } else ImGui::TextWrapped("[%01X] - %04X", i, emu.stack[i]);
    }

    ImGui::End();

    ImGui::PopStyleColor();
}
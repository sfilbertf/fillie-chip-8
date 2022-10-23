#include "Application.h"

void Application::status() {
    ImGuiIO& io = ImGui::GetIO();
    ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.1375f, io.DisplaySize.y * 0.225f), ImGuiCond_Always, ImVec2(0.5f,0.5f));
    ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x * 0.22575f, io.DisplaySize.y * 0.3f), ImGuiCond_Always);


    ImGui::PushStyleColor(ImGuiCol_WindowBg, bgColorf);
    ImGui::PushFont(quinqueFiveSubtext);

    ImGui::Begin("Status", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize); 

    ImGui::TextWrapped("%s", rom);
    ImGui::Dummy(ImVec2(0.0f, 20.0f));
    ImGui::TextWrapped("SIZ: %d BYTES", romSize);
    ImGui::Dummy(ImVec2(0.0f, 20.0f));
    ImGui::TextWrapped("OPF: %d", opcodesPerFrame);
    ImGui::Dummy(ImVec2(0.0f, 20.0f));
    ImGui::TextWrapped("CYC: %llu", cycles);
    ImGui::Dummy(ImVec2(0.0f, 20.0f));
    ImGui::TextWrapped("FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::Dummy(ImVec2(0.0f, 20.0f));

    ImGui::End();

    ImGui::PopFont();
    ImGui::PopStyleColor();
}
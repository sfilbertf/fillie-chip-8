#include "Application.h"

void Application::stack() {
    // ImGuiWindowFlags_NoMove, ImGuiWindowFlags_NoResize
    ImGuiIO& io = ImGui::GetIO();
    ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.875f, io.DisplaySize.y * 0.78125f), ImGuiCond_Always, ImVec2(0.5f,0.5f));
    ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x * 0.175f, io.DisplaySize.y * 0.375f), ImGuiCond_Always);
    ImGui::Begin("Stack", NULL, ImGuiWindowFlags_NoTitleBar); 
    ImGui::Text("Test");
    ImGui::End();
}
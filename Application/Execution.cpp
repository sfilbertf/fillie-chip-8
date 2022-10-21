#include "Application.h"

void Application::execution() {
    // ImGuiWindowFlags_NoMove, ImGuiWindowFlags_NoResize
    ImGuiIO& io = ImGui::GetIO();
    ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.875f, io.DisplaySize.y * 0.3f), ImGuiCond_Always, ImVec2(0.5f,0.5f));
    ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x * 0.225f, io.DisplaySize.y * 0.5f), ImGuiCond_Always);
    ImGui::Begin("Execution", NULL, ImGuiWindowFlags_NoTitleBar); 
    ImGui::Text("Test");
    ImGui::End();
}
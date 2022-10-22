#include "Application.h"

void Application::registers() {    
    ImGuiIO& io = ImGui::GetIO();
    ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.8275f), ImGuiCond_Always, ImVec2(0.5f,0.5f));
    ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.275f), ImGuiCond_Always);
    
    ImGui::PushStyleColor(ImGuiCol_WindowBg, bgColorf);
    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(2.75f, 10.0f));

    ImGui::Begin("Registers", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize); 
    ImGui::BeginTable("Registers", 5);
    for(int i = 0; i < 4; i++) {
        ImGui::TableNextRow(ImGuiTableRowFlags_None, 55.0f);
        ImGui::TableNextColumn();
        ImGui::Text("V%01X:%02X", i + 0, emu.v[i + 0]);
        ImGui::TableNextColumn();
        ImGui::Text("V%01X:%02X", i + 4, emu.v[i + 4]);
        ImGui::TableNextColumn();
        ImGui::Text("V%01X:%02X", i + 8, emu.v[i + 8]);
        ImGui::TableNextColumn();
        ImGui::Text("V%01X:%02X", i + 12, emu.v[i + 12]);

        ImGui::TableNextColumn();
        if(i == 0) { ImGui::Text("PC:%04X", emu.pc); }
        else if(i == 1) { ImGui::Text("I: %04X", emu.idx); }
        else if(i == 2) { ImGui::Text("DT:%02X", emu.delayTimer); }
        else { ImGui::Text("ST:%02X", emu.soundTimer); }
    }  
    ImGui::EndTable();
    ImGui::End();

    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
}
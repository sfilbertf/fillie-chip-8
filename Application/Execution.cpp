#include "Application.h"

void Application::writeInstrToBuf(uint16_t addr) {
    uint8_t bufSize = BUF_SIZE;
    memset(buf, 0, bufSize);

    uint16_t opcode = (emu.mem[addr] << 8) | emu.mem[addr + 1];

    uint8_t x = (opcode & 0x0f00) >> 8;
    uint8_t y = (opcode & 0x00f0) >> 4;
    
    uint8_t n = opcode & 0x000f;
    uint8_t kk = opcode & 0x00ff;
    uint16_t nnn = opcode & 0x0fff;

    uint8_t firstNibble = (opcode & 0xf000) >> 12;
    uint8_t thirdNibble = (opcode & 0x00f0) >> 4;
    uint8_t fourthNibble = (opcode & 0x000f) >> 0;

    switch(emu.firstNibble) {
        case 0x0:
        {
            if(fourthNibble == 0x0) { snprintf(buf, bufSize, "%04X - CLS", addr); }
            else if(fourthNibble == 0xe) { snprintf(buf, bufSize, "%04X - RET", addr); }
            else { snprintf(buf, bufSize, "%04X - ???", addr); }
            break;
        }   

        case 0x8:
        {
            if(fourthNibble == 0x0) { snprintf(buf, bufSize, "%04X - LD V%01X V%01X", addr, x, y); }
            else if(fourthNibble == 0x1) { snprintf(buf, bufSize, "%04X - OR V%01X V%01X", addr, x, y); }  
            else if(fourthNibble == 0x2) { snprintf(buf, bufSize, "%04X - AND V%01X V%01X", addr, x, y); } 
            else if(fourthNibble == 0x3) { snprintf(buf, bufSize, "%04X - XOR V%01X V%01X", addr, x, y); } 
            else if(fourthNibble == 0x4) { snprintf(buf, bufSize, "%04X - ADD V%01X V%01X", addr, x, y); } 
            else if(fourthNibble == 0x5) { snprintf(buf, bufSize, "%04X - SUB V%01X V%01X", addr, x, y); } 
            else if(fourthNibble == 0x6) { snprintf(buf, bufSize, "%04X - SHR V%01X {V%01X}", addr, x, y); } 
            else if(fourthNibble == 0x7) { snprintf(buf, bufSize, "%04X - SUBN V%01X V%01X", addr, x, y); } 
            else if(fourthNibble == 0xe) { snprintf(buf, bufSize, "%04X - SHL V%01X {V%01X}", addr, x, y); } 
            else { snprintf(buf, 15, "%04X - ???", addr); }
            break;
        }

        case 0xe:
        {
            if(fourthNibble == 0xe) { snprintf(buf, bufSize, "%04X - SKP V%01X", addr, x); }
            else if(fourthNibble == 0x1) { snprintf(buf, bufSize, "%04X - SKNP V%01X", addr, x); }
            else { snprintf(buf, 15, "%04X - ???", addr); }
            break;
        }

        case 0xf:
        {
            uint8_t index = (thirdNibble << 4) | fourthNibble;
            if(index == 0x07) { snprintf(buf, bufSize, "%04X - LD V%01X DT", addr, x); }
            else if(index == 0x0a) { snprintf(buf, bufSize, "%04X - LD V%01X K", addr, x); }
            else if(index == 0x15) { snprintf(buf, bufSize, "%04X - LD DT V%01X", addr, x); }
            else if(index == 0x18) { snprintf(buf, bufSize, "%04X - LD ST V%01X", addr, x); }
            else if(index == 0x1e) { snprintf(buf, bufSize, "%04X - ADD I V%01X", addr, x); }
            else if(index == 0x29) { snprintf(buf, bufSize, "%04X - LD F V%01X", addr, x); }
            else if(index == 0x33) { snprintf(buf, bufSize, "%04X - LD B V%01X", addr, x); }
            else if(index == 0x55) { snprintf(buf, bufSize, "%04X - LD [I] V%01X", addr, x); }
            else if(index == 0x65) { snprintf(buf, bufSize, "%04X - LD V%01X [I]", addr, x); }
            else { snprintf(buf, 15, "%04X - ???", addr); }
            break;
        }

        default:
        {
            if(firstNibble == 0x1) { snprintf(buf, bufSize, "%04X - JP %03X", addr, nnn); }
            else if(firstNibble == 0x2) { snprintf(buf, bufSize, "%04X - CALL %03X", addr, nnn); }
            else if(firstNibble == 0x3) { snprintf(buf, bufSize, "%04X - SE V%01X %02X", addr, x, kk); }
            else if(firstNibble == 0x4) { snprintf(buf, bufSize, "%04X - SNE V%01X %02X", addr, x, kk); }
            else if(firstNibble == 0x5) { snprintf(buf, bufSize, "%04X - SE V%01X V%01X", addr, x, y); }
            else if(firstNibble == 0x6) { snprintf(buf, bufSize, "%04X - LD V%01X %02X", addr, x, kk); }
            else if(firstNibble == 0x7) { snprintf(buf, bufSize, "%04X - ADD V%01X %02X", addr, x, kk); }
            else if(firstNibble == 0x9) { snprintf(buf, bufSize, "%04X - SNE V%01X V%01X", addr, x, y); }
            else if(firstNibble == 0xa) { snprintf(buf, bufSize, "%04X - LD I %03X", addr, nnn); }
            else if(firstNibble == 0xb) { snprintf(buf, bufSize, "%04X - JP V0 %03X", addr, nnn); }
            else if(firstNibble == 0xc) { snprintf(buf, bufSize, "%04X - RND V%01X %02X", addr, x, kk); }
            else if(firstNibble == 0xd) { snprintf(buf, bufSize, "%04X - DRW V%01X V%01X %01X", addr, x, y, n); }
        }
    }
}

void Application::execution() {
    ImGuiIO& io = ImGui::GetIO();
    ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.13425f, io.DisplaySize.y * 0.70125f), ImGuiCond_Always, ImVec2(0.5f,0.5f));
    ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x * 0.24f, io.DisplaySize.y * 0.545f), ImGuiCond_Always);

    ImGui::PushFont(quinqueFiveSubtext);
    ImGui::PushStyleColor(ImGuiCol_Text, fgColorf);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, bgColorf);

    ImGui::Begin("Execution", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize); 

    for(int i = -16; i < 18; i+= 2) {
        writeInstrToBuf(emu.pc + i);
        if(i == 0) {
            ImGui::PushTextWrapPos();
            ImGui::TextColored(tertColorf, buf);
            ImGui::PopTextWrapPos();
        } else ImGui::Text(buf);
    }

    ImGui::End();

    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopFont();
}
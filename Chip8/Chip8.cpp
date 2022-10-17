#include "Chip8.h"

void Chip8::loadRom(std::string romFile) {
    // Construct an ifstream object and open file in binary mode for reading
    std::ifstream rom (romFile, std::ifstream::in | std::ifstream::binary);
    int romSize;

    // Get the size of the ROM in bytes
    if(rom.is_open()) {
        rom.seekg(0, rom.end);
        romSize = rom.tellg();
        rom.seekg(0, rom.beg);
    }

    // Read the ROM into a buffer
    char buffer[romSize];
    // std::cout << "Reading ROM, " << (romSize)/2 << " instructions found" << std::endl;
    rom.read(buffer, romSize);

    // Load the ROM into the main memory starting from address 0x200
    for(int i = 0; i < romSize; i++) {
        mem[PROGRAM_START + i] = buffer[i];
    }

    // Close the ROM file
    rom.close();
}

void Chip8::resetChip8() {
    idx = 0;
    pc = 0x200;

    sp = 0;

    delayTimer = 0;
    soundTimer = 0;

    memset(v, 0, 16);
    memset(stack, 0, 16 * 2);
    memset(keypad, 0, 16);

    memset(mem, 0, 4 * 1024);
    memset(disMem, 0, 64 * 32);

    for(int i = 0; i < 80; i++) {
        mem[FONTSET_START + i] = fontset[i];
    }
}

void Chip8::initOpcodeTable() {
    table00e[0x0] = &Chip8::i00e0;
    table00e[0xe] = &Chip8::i00ee;
    
    tableUnq[0x1] = &Chip8::i1nnn;
    tableUnq[0x2] = &Chip8::i2nnn;
    tableUnq[0x3] = &Chip8::i3xkk;
    tableUnq[0x4] = &Chip8::i4xkk;
    tableUnq[0x5] = &Chip8::i5xy0;
    tableUnq[0x6] = &Chip8::i6xkk;
    tableUnq[0x7] = &Chip8::i7xkk;
    tableUnq[0x9] = &Chip8::i9xy0;
    tableUnq[0xa] = &Chip8::iannn;
    tableUnq[0xb] = &Chip8::ibnnn;
    tableUnq[0xc] = &Chip8::icxkk;
    tableUnq[0xd] = &Chip8::idxyn;
    
    table8xy[0x0] = &Chip8::i8xy0;
    table8xy[0x1] = &Chip8::i8xy1;
    table8xy[0x2] = &Chip8::i8xy2;
    table8xy[0x3] = &Chip8::i8xy3;
    table8xy[0x4] = &Chip8::i8xy4;
    table8xy[0x5] = &Chip8::i8xy5;
    table8xy[0x6] = &Chip8::i8xy6;
    table8xy[0x7] = &Chip8::i8xy7;
    table8xy[0xe] = &Chip8::i8xye;
    
    tableex[0xE] = &Chip8::iex9e;
    tableex[0x1] = &Chip8::iexa1;
    
    tablefx[0x07] = &Chip8::ifx07;
    tablefx[0x0a] = &Chip8::ifx0a;
    tablefx[0x15] = &Chip8::ifx15;
    tablefx[0x18] = &Chip8::ifx18;
    tablefx[0x1e] = &Chip8::ifx1e;
    tablefx[0x29] = &Chip8::ifx29;
    tablefx[0x33] = &Chip8::ifx33;
    tablefx[0x55] = &Chip8::ifx55;
    tablefx[0x65] = &Chip8::ifx65;
}

void Chip8::fetch() {
    opcode = mem[pc] << 8 | mem[pc + 1];
    pc += 2;
}

void Chip8::decode() {
    x = opcode & 0x0f00 >> 8;
    y = opcode & 0x00f0 >> 4;
    
    n = opcode & 0x000f;
    kk = opcode & 0x00ff;
    nnn = opcode & 0x0fff;

    firstNibble = opcode & 0xf000 >> 12;
    secondNibble = opcode & 0x0f00 >> 8;
    thirdNibble = opcode & 0x00f0 >> 4;
    fourthNibble = opcode & 0x000f;
}

void Chip8::execute() {
    if(firstNibble == 0x0) (this->*table00e[fourthNibble])();

    else if(firstNibble == 0x8) (this->*table8xy[fourthNibble])();

    else if(firstNibble == 0xe) (this->*tableex[fourthNibble])();

    else if(firstNibble == 0xf) (this->*tablefx[thirdNibble << 4 | fourthNibble])();

    else if(firstNibble > 0x0 || firstNibble < 0xe) (this->*tableUnq[firstNibble])();
    
    else inop();
}
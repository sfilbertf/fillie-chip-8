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
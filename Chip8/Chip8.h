#include <cstdint>

#include <fstream>
#include <iostream>

#include <string>
#include <cstring>

#include <time.h>
#include <cstdlib>

#define PROGRAM_START 0x200 // Starting address of a Chip-8 program in the memory
#define FONTSET_START 0x50 // Starting address of the Chip-8 fontset in memory

class Chip8 {
    public:
        uint8_t v[16];
        uint8_t mem[4 * 1024];
        uint8_t disMem[64 * 32];
        uint8_t keypad[16];

        uint16_t idx;
        uint16_t pc;

        uint16_t stack[16];
        uint8_t sp;

        uint8_t delayTimer;
        uint8_t soundTimer;

    public:
        uint8_t x;
        uint8_t y;
        uint8_t kk;
        uint8_t n;
        uint8_t nnn;

    public:
        Chip8() {
            
        }

        ~Chip8() {
        
        }

    public:
        void loadRom(std::string romFile); // Load the contents of the ROM to the main memory
        void resetChip8(); // Reset the Chip-8 emulator

    // Chip-8 instruction set
    public:
        void i00e0();
        void i00ee();
        void i1nnn();
        void i2nnn();
        void i3xkk();
        void i4xkk();
        void i5xy0();
        void i6xkk();
        void i7xkk();
        void i8xy0();
        void i8xy1();
        void i8xy2();
        void i8xy3();
        void i8xy4();
        void i8xy5();
        void i8xy6();
        void i8xy7();
        void i8xye();
        void i9xy0();
        void iannn();
        void ibnnn();
        void icxkk();
        void idxyn();
        void iex9e();
        void iexa1();
        void ifx07();
        void ifx0a();
        void ifx15();
        void ifx18();
        void ifx1e();
        void ifx29();
        void ifx33();
        void ifx55();
        void ifx65();
};

// Chip-8 fontset
uint8_t fontset[80] =
{
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

// Chip-8 instruction set opcode table
void (*table00e[0xf])();
void (*tableUnq[0xe])();
void (*table8xy[0xf])();
void (*tableex[0xf])();
void (*tablefx[0x66])();


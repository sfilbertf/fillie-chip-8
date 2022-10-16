#include "Chip8.h"

void Chip8::resetChip8() {
    idx = 0;
    pc = 0x200;

    sp = 0;

    delayTimer = 0;
    soundTimer = 0;

    memset(v, 0, 16);
    memset(stack, 0, 16);
    memset(keypad, 0, 16);

    memset(mem, 0, 4 * 1024);
    memset(disMem, 0, 64 * 32);

    for(int i = 0; i < 80; i++) {
        mem[FONTSET_START + i] = fontset[i];
    }
}
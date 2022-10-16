#include "Chip8.h"

/**
 * Descriptions for these instructions are
 * taken from "Cowgod's Chip-8 Technical Reference v1.0"
 * http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#3.1
 */

/**
 * 00E0 - CLS
 * Clear the display.
 */
void Chip8::i00e0() {
    memset(disMem, 0, 64 * 32);
}

/**
 * 00EE - RET
 * Return from a subroutine.
 
 * The interpreter sets the program counter to the address at the top of the stack, then subtracts 1 from the stack pointer.
 */
void Chip8::i00ee() {
    pc = stack[--sp];
}

/**
 * 1nnn - JP addr
 * Jump to location nnn.
 
 * The interpreter sets the program counter to nnn.
 */
void Chip8::i1nnn() {
    pc = nnn;
}

/**
 * 2nnn - CALL addr
 * Call subroutine at nnn.

 * The interpreter increments the stack pointer, then puts the current PC on the top of the stack. The PC is then set to nnn.
 */
void Chip8::i2nnn() {
    stack[sp++] = pc;
    pc = nnn;
}

/**
 * 3xkk - SE Vx, byte
 * Skip next instruction if Vx = kk.

 * The interpreter compares register Vx to kk, and if they are equal, increments the program counter by 2.
 */
void Chip8::i3xkk() {
    if(v[x] == kk) pc += 2;
}

/**
 * 4xkk - SNE Vx, byte
 * Skip next instruction if Vx != kk.

 * The interpreter compares register Vx to kk, and if they are not equal, increments the program counter by 2.
 */
void Chip8::i4xkk() {
    if(v[x] != kk) pc += 2;
}

/**
 * 5xy0 - SE Vx, Vy
 * Skip next instruction if Vx = Vy.

 * The interpreter compares register Vx to register Vy, and if they are equal, increments the program counter by 2.
 */
void Chip8::i5xy0() {
    if(v[x] == v[y]) pc += 2;
}

/**
 * 6xkk - LD Vx, byte
 * Set Vx = kk.

 * The interpreter puts the value kk into register Vx.
 */
void Chip8::i6xkk() {
    v[x] = kk;
}

/**
 * 7xkk - ADD Vx, byte
 * Set Vx = Vx + kk.

 * Adds the value kk to the value of register Vx, then stores the result in Vx.
 */
void Chip8::i7xkk() {
    v[x] += kk;
}

/**
 * 8xy0 - LD Vx, Vy
 * Set Vx = Vy.

 * Stores the value of register Vy in register Vx. 
 */
void Chip8::i8xy0() {
    v[x] = v[y];
}

/**
 * 8xy1 - OR Vx, Vy
 * Set Vx = Vx OR Vy.

 * Performs a bitwise OR on the values of Vx and Vy, then stores the result in Vx. 
 * A bitwise OR compares the corrseponding bits from two values, and if either bit is 1, 
 * then the same bit in the result is also 1. Otherwise, it is 0.
 */
void Chip8::i8xy1() {
    v[x] |= v[y];
}

/**
 * 8xy2 - AND Vx, Vy
 * Set Vx = Vx AND Vy.

 * Performs a bitwise AND on the values of Vx and Vy, then stores the result in Vx. 
 * A bitwise AND compares the corrseponding bits from two values, and if both bits are 1, 
 * then the same bit in the result is also 1. Otherwise, it is 0.
 */
void Chip8::i8xy2() {
    v[x] &= v[y];
}

/**
 * 8xy3 - XOR Vx, Vy
 * Set Vx = Vx XOR Vy.

 * Performs a bitwise exclusive OR on the values of Vx and Vy, 
 * then stores the result in Vx. An exclusive OR compares the corrseponding bits from two values, and if the bits are not both the same, 
 * then the corresponding bit in the result is set to 1. Otherwise, it is 0.
 */
void Chip8::i8xy3() {
    v[x] ^= v[y];
}

/**
 * 8xy4 - ADD Vx, Vy
 * Set Vx = Vx + Vy, set VF = carry.

 * The values of Vx and Vy are added together. If the result is greater than 8 bits (i.e., > 255,) VF is set to 1, otherwise 0. 
 * Only the lowest 8 bits of the result are kept, and stored in Vx.
 */
void Chip8::i8xy4() {
    uint16_t temp = v[x] + v[y];
    
    uint8_t lo = temp & 0x00ff;
    uint8_t hi = temp & 0xff00 >> 8;

    v[x] = lo;

    if(hi) v[0xf] = 1;
    else v[0xf] = 0;
}

/**
 * 8xy5 - SUB Vx, Vy
 * Set Vx = Vx - Vy, set VF = NOT borrow.

 * If Vx > Vy, then VF is set to 1, otherwise 0. Then Vy is subtracted from Vx, and the results stored in Vx.
 */
void Chip8::i8xy5() {
    if(v[x] > v[y]) v[0xf] = 1;
    else v[0xf] = 0;

    v[x] -= v[y];
}

/**
 * 8xy6 - SHR Vx {, Vy}
 * Set Vx = Vx SHR 1.

 * If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0. Then Vx is divided by 2.
 */
void Chip8::i8xy6() {
    if(v[x] & 0x01 == 1) v[0xf] = 1;
    else v[0xf] = 0;

    v[x] >>= 1;
}

/**
 * 8xy7 - SUBN Vx, Vy
 * Set Vx = Vy - Vx, set VF = NOT borrow.

 * If Vy > Vx, then VF is set to 1, otherwise 0. Then Vx is subtracted from Vy, and the results stored in Vx.
 */
void Chip8::i8xy7() {
    if(v[y] > v[x]) v[0xf] = 1;
    else v[0xf] = 0;

    v[x] = v[y] - v[x];
}

/**
 * 8xyE - SHL Vx {, Vy}
 * Set Vx = Vx SHL 1.

 * If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0. Then Vx is multiplied by 2.
 */
void Chip8::i8xye() {
    if(v[x] >> 7 == 1) v[0xf] = 1;
    else v[0xf] = 0;

    v[x] <<= 1;
}

/**
 * 9xy0 - SNE Vx, Vy
 * Skip next instruction if Vx != Vy.

 * The values of Vx and Vy are compared, and if they are not equal, the program counter is increased by 2.
 */
void Chip8::i9xy0() {
    if(v[x] != v[y]) pc += 2;
}

/**
 * Annn - LD I, addr
 * Set I = nnn.
 
 * The value of register I is set to nnn. 
 */
void Chip8::iannn() {
    idx = nnn;
}

/**
 * Bnnn - JP V0, addr
 * Jump to location nnn + V0.

 * The program counter is set to nnn plus the value of V0.
 */
void Chip8::ibnnn() {
    pc = nnn + v[0];
}

/**
 * Cxkk - RND Vx, byte
 * Set Vx = random byte AND kk.

 * The interpreter generates a random number from 0 to 255, which is then ANDed with the value kk. 
 * The results are stored in Vx. See instruction 8xy2 for more information on AND.
 */
void Chip8::icxkk() {
    srand(time(0));

    v[x] = (rand() % 255) & kk;
}


/**
 * Dxyn - DRW Vx, Vy, nibble
 * Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.

 * The interpreter reads n bytes from memory, starting at the address stored in I. 
 * These bytes are then displayed as sprites on screen at coordinates (Vx, Vy). Sprites are XORed onto the existing screen. 
 * If this causes any pixels to be erased, VF is set to 1, otherwise it is set to 0. 
 * If the sprite is positioned so part of it is outside the coordinates of the display, it wraps around to the opposite side of the screen. 
 * See instruction 8xy3 for more information on XOR, and section 2.4, Display, for more information on the Chip-8 screen and sprites.
 */
void Chip8::idxyn() {
    v[0xf] = 0;

    uint8_t xCoordinate = v[x] % 64;
    uint8_t yCoordinate = v[y] % 32;

    for(int i = 0; i < n; i++) {
        uint8_t sprite = mem[i + idx];
        uint8_t yCurr = yCoordinate + i;

        if(yCurr > 31) break;

        for(int j = 0; j < 8; j++) {
            uint8_t currBit = 0x01 & (sprite >> (8 - j - 1));
            uint8_t xCurr = xCoordinate + j;
            
            if(xCurr > 63) break;

            if(!v[0xf]) {
                if( (currBit == 1) & (disMem[yCurr * 64 + xCurr] == 1) ) {
                    v[0xf] = 1;
                }
            }

            disMem[yCurr * 64 + xCurr] ^= currBit;
        }
    }
}

/**
 * Ex9E - SKP Vx
 * Skip next instruction if key with the value of Vx is pressed.

 * Checks the keyboard, and if the key corresponding to the value of Vx is currently in the down position, PC is increased by 2.
 */
void Chip8::iex9e() {
    if(keypad[v[x]]) {
        pc += 2;
    }
}

/**
 * ExA1 - SKNP Vx
 * Skip next instruction if key with the value of Vx is not pressed.

 * Checks the keyboard, and if the key corresponding to the value of Vx is currently in the up position, PC is increased by 2.
 */
void Chip8::iexa1() {
    if(!keypad[v[x]]) {
        pc += 2;
    }
}

/**
 * Fx07 - LD Vx, DT
 * Set Vx = delay timer value.

 * The value of DT is placed into Vx
 */
void Chip8::ifx07() {
    v[x] = delayTimer;
}

/**
 * Fx0A - LD Vx, K
 * Wait for a key press, store the value of the key in Vx.

 * All execution stops until a key is pressed, then the value of that key is stored in Vx.
 */
void Chip8::ifx0a() {
    if(keypad[0])        v[x] = 0;
    else if(keypad[1])   v[x] = 1;
    else if(keypad[2])   v[x] = 2;
    else if(keypad[3])   v[x] = 3;
    else if(keypad[4])   v[x] = 4;
    else if(keypad[5])   v[x] = 5;
    else if(keypad[6])   v[x] = 6;
    else if(keypad[7])   v[x] = 7;
    else if(keypad[8])   v[x] = 8;
    else if(keypad[9])   v[x] = 9;
    else if(keypad[0xa]) v[x] = 0xa;
    else if(keypad[0xb]) v[x] = 0xb;
    else if(keypad[0xc]) v[x] = 0xc;
    else if(keypad[0xd]) v[x] = 0xd;
    else if(keypad[0xe]) v[x] = 0xe;
    else if(keypad[0xf]) v[x] = 0xf;
    else pc -= 2;
}

/**
 * Fx15 - LD DT, Vx
 * Set delay timer = Vx.

 * DT is set equal to the value of Vx.
 */
void Chip8::ifx15() {
    delayTimer = v[x];
}

/**
 * Fx18 - LD ST, Vx
 * Set sound timer = Vx.

 * ST is set equal to the value of Vx.
 */
void Chip8::ifx18() {
    soundTimer = v[x];
}

/**
 * Fx1E - ADD I, Vx
 * Set I = I + Vx.

 * The values of I and Vx are added, and the results are stored in I.
 */
void Chip8::ifx1e() {
    idx += v[x];
}

/**
 * Fx29 - LD F, Vx
 * Set I = location of sprite for digit Vx.

 * The value of I is set to the location for the hexadecimal sprite corresponding to the value of Vx. 
 * See section 2.4, Display, for more information on the Chip-8 hexadecimal font.
 */
void Chip8::ifx29() {
    idx = FONTSET_START + (v[x] * 5);
}

/**
 * Fx33 - LD B, Vx
 * Store BCD representation of Vx in memory locations I, I+1, and I+2.

 * The interpreter takes the decimal value of Vx, and places the hundreds digit in memory at location in I, 
 * the tens digit at location I+1, and the ones digit at location I+2.
 */
void Chip8::ifx33() {
    mem[idx] = v[x] / 100;
    mem[idx + 1] = (v[x] % 100) / 10;
    mem[idx + 2] = (v[x] % 100) % 10;
}

/**
 * Fx55 - LD [I], Vx
 * Store registers V0 through Vx in memory starting at location I.

 * The interpreter copies the values of registers V0 through Vx into memory, starting at the address in I.
 */
void Chip8::ifx55() {
    for(int i = 0; i <= x; i++) {
        mem[idx + i] = v[i];
    }
}

/**
 * Fx65 - LD Vx, [I]
 * Read registers V0 through Vx from memory starting at location I.

 * The interpreter reads values from memory starting at location I into registers V0 through Vx.
 */
void Chip8::ifx65() {
    for(int i = 0; i <= x; i++) {
        v[i] = mem[idx + i];
    }
}

/**
 * ???? - NOP
 * Does nothing. 
 
 * The nop (no operation) opcode safeguards 
 * the interpreter (to a certain degree) against nonexistent/illegal opcodes.
 */
void Chip8::inop() { }
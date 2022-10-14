#include <cstdint>

#include <fstream>
#include <iostream>

#include <cstring>
#include <string>

#include <SDL2/SDL.h>

#include <unistd.h>

#include <chrono>

uint8_t V[16];
uint8_t MEM[4096];
uint8_t DIS_MEM[2048];
uint8_t KEYPAD[16];

uint16_t I;
uint16_t PC;

uint16_t STACK[16];
uint8_t SP;

uint8_t DELAY_TIMER;
uint8_t SOUND_TIMER;

uint8_t FONTSET[80] =
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

uint16_t OPCODE;

uint8_t HB_1;
uint8_t HB_2;
uint8_t HB_3;
uint8_t HB_4;

uint8_t  X;
uint8_t  Y;
uint8_t  KK;
uint8_t  N;
uint16_t NNN;

uint8_t CNT;

void load_ROM(std::string ROM_FILENAME) {
    // Construct an ifstream object and open file in binary mode for reading
    std::ifstream ROM (ROM_FILENAME, std::ifstream::in | std::ifstream::binary);
    int rom_size;

    // Get the size of the ROM in bytes
    if(ROM.is_open()) {
        ROM.seekg(0, ROM.end);
        rom_size = ROM.tellg();
        ROM.seekg(0, ROM.beg);
    }

    // Read the ROM into a buffer
    char* buffer = new char[rom_size];
    std::cout << "Reading ROM, " << (rom_size)/2 << " instructions found" << std::endl;

    ROM.read(buffer, rom_size);

    // Load the ROM into the main memory starting from address 0x200
    uint16_t START = 0x200;

    for(int i = 0; i < rom_size; i++) {
        MEM[START + i] = buffer[i];
    }

    // Close the ROM file and deallocate memory for the buffer
    ROM.close();

    delete[] buffer;
}

void init_CHIP8() {
    I = 0;
    PC = 0x200;

    SP = 0;

    DELAY_TIMER = 0;
    SOUND_TIMER = 0;

    CNT = 0;

    memset(V, 0, 16);
    memset(STACK, 0, 16);
    memset(KEYPAD, 0, 16);

    memset(MEM, 0, 4096);
    memset(DIS_MEM, 0, 2048);

    for(int i = 0; i < 80; i++) {
        MEM[i] = FONTSET[i];
    }
}

void fetch() {
    OPCODE = MEM[PC] << 8 | MEM[PC+1];
    PC += 2;    
}

void decode() {
    HB_1 = (OPCODE & 0xF000u) >> 12;
    HB_2 = (OPCODE & 0x0F00u) >> 8;
    HB_3 = (OPCODE & 0x00F0u) >> 4;
    HB_4 = (OPCODE & 0x000Fu) >> 0;

    X = (OPCODE & 0x0F00u) >> 8; 
    Y = (OPCODE & 0x00F0u) >> 4;

    N   = OPCODE & 0x000Fu; 
    KK  = OPCODE & 0x00FFu;
    NNN = OPCODE & 0x0FFFu;
}

/* * * * * * * * * OPCODE LOOKUP TABLE * * * * * * * */
void (*TABLE_00E[0x0Fu])();
void (*TABLE_UNQ[0X0Eu])();
void (*TABLE_8XY[0x0Fu])();
void (*TABLE_EX[0x0Fu])();
void (*TABLE_FX[0x66u])();

// 00E0 - CLS
// Clear the display
void I_00E0() {
    memset(DIS_MEM, 0, 2048);
    printf("$%x: CLS\n", PC-2);
}

// 00EE - RET
// Return from a subroutine
void I_00EE() {
    SP--;
    PC = STACK[SP];
    printf("$%x: RET\n", PC-2);
} 

// 1NNN - JP addr
// Jump to location NNN
void I_1NNN() {
    PC = NNN;
    printf("$%x: JP %x\n", PC-2, NNN);
}

// 2NNN - CALL addr
// Call subroutine at NNN
void I_2NNN() {
    STACK[SP] = PC;
    SP++;
    PC = NNN;
    printf("$%x: CALL %x\n", PC-2, NNN);
}

// 3XKK - SE VX, byte
// Skip next instruction if VX = KK
void I_3XKK() {
    if(V[X] == KK) {
        PC += 2;
    }
    printf("$%x: SE V%x, %x\n", PC-2, X, KK);
}

// SNE VX, byte
// Skip next instruction if VX != KK
void I_4XKK() {
    if(V[X] != KK) {
        PC += 2;
    }
    printf("$%x: SNE V%x, %x\n", PC-2, X, KK);
}

// SE VX, VY
// Skip next instruction if VX = VY
void I_5XY0() {
    if(V[X] == V[Y]) {
        PC += 2;
    }
    printf("$%x: SE V%x, V%x\n", PC-2, X, Y);
}

// LD VX, byte
// Set VX = KK
void I_6XKK() {
    V[X] = KK;
    printf("$%x: LD V%x, %x\n", PC-2, X, KK);
}

// ADD VX, byte
// Set VX = VX + KK
void I_7XKK() {
    V[X] += KK;
    printf("$%x: ADD V%x, %x\n", PC-2, X, KK);
}

// SNE VX, VY
// Skip next instruction if VX != VY
void I_9XY0() {
    if(V[X] != V[Y]) {
        PC += 2;
    }
    printf("$%x: SNE V%x, V%x\n", PC-2, X, Y); 
}

// LD I, addr
// Set I = NNN
void I_ANNN() {
    I = NNN;
    printf("$%x: LD I, %x\n", PC-2, NNN);
}

// JP V0, addr
// Jump to location nnn + V0
void I_BNNN() {
    PC = NNN + V[0];
    printf("$%x: JP V0, %x\n", PC-2, NNN);
}

// RND VX, byte
// Set VX = random byte AND KK
void I_CXKK() {
    V[X] = (rand() % 255) & KK;
    printf("$%x: RND V%x, %x\n", PC-2, X, KK);
}

// DRW VX, VY, nibble
// Display n-byte sprite starting at memory location IDX at (VX,VY), set VF = collision
void I_DXYN() {
    uint8_t X_COOR = V[X] % 64;
    uint8_t Y_COOR = V[Y] % 32;

    V[0xFu] = 0;

    for(int ROW = 0; ROW < N; ROW++) {
        uint8_t SPRITE = MEM[I + ROW];
        for(int COL = 0; COL < 8; COL++) {
            uint8_t SPRITE_BIT = (SPRITE >> (8 - COL - 1)) & 0x1u;
            uint16_t ADDR = (X_COOR + COL) + (Y_COOR + ROW) * 64;

            if(SPRITE_BIT) {
                if(DIS_MEM[ADDR] == 1) V[0xFu] = 1;
                DIS_MEM[ADDR] ^= SPRITE_BIT;
            }
        }
    }
    printf("$%x: DRW V%x, V%x, %x\n", PC-2, X, Y, N);
}

// LD VX, VY
// Set VX = VY
void I_8XY0() {
    V[X] = V[Y];
    printf("$%x: LD V%x, V%x\n", PC-2, X, Y);
}

// OR VX, VY
// Set VX = VX OR VY
void I_8XY1() {
    V[X] |= V[Y];
    printf("$%x: OR V%x, V%x\n", PC-2, X, Y);
}

// AND VX, VY
// Set VX = VX AND VY
void I_8XY2() {
    V[X] &= V[Y];
    printf("$%x: AND V%x, V%x\n", PC-2, X, Y);
}

// XOR VX, VY
// Set VX = VX XOR VY
void I_8XY3() {
    V[X] ^= V[Y];
    printf("$%x: XOR V%x, V%x\n", PC-2, X, Y);
}

// ADD VX, VY
// Set VX = VX + VY, set VF = carry
void I_8XY4() {
    uint16_t res = V[X] + V[Y];
    
    if(res > 255) { V[0xFu] = 1; } 
    else { V[0xFu] = 0; }

    V[X] = res & 0x00FF;
    printf("$%x: ADD V%x, V%x\n", PC-2, X, Y);
}

// SUB VX, VY 
// Set VX = VX - VY, setBF = NOT borrow
void I_8XY5() {
    if(V[X] > V[Y]) { V[0xFu] = 1; }
    else { V[0xFu] = 0; }

    V[X] = V[X] - V[Y];
    printf("$%x: SUB V%x, V%x\n", PC-2, X, Y);
}

// SHR VX {, VY}
// Set VX = VX SHR 1 
void I_8XY6() {
    if((V[X] & 0x1u) == 1) { V[0xFu] = 1; } 
    else { V[0xFu] = 0; } 

    V[X] >>= 1;
    printf("$%x: SHR V%x {, V%x}\n", PC-2, X, Y);
}

// SUBN VX, VY
// Set VX = VY - VX, set VF = NOT borrow
void I_8XY7() {
    if(V[Y] > V[X]) { V[0xFu] = 1; } 
    else { V[0xFu] = 0; }

    V[X] = V[Y] - V[X];
    printf("$%x: SUBN V%x, V%x\n", PC-2, X, Y);
}

// SHL Vx {, Vy}
// Set VX = VX SHL 1
void I_8XYE() {
    uint8_t temp = V[X] & 0x80u >> 7;
    if(temp == 1) { V[0xFu] = 1; }
    else { V[0xFu] = 0; }

    V[X] <<= 1;
    printf("$%x: SHL V%x {, V%x}\n", PC-2, X, Y);
}

// SKP VX
void I_EX9E() {
    if(KEYPAD[V[X]] == 1) PC += 2;
    printf("$%x: SKP V%x\n", PC-2, X);
}

// SKNP VX
void I_EXA1() {
    if(KEYPAD[V[X]] == 0) PC += 2; 
    printf("$%x: SKNP V%x\n", PC-2, X);
}

// LD VX, DT
// Set VX = delay timer value
void I_FX07() {
    V[X] = DELAY_TIMER;
    printf("$%x: LD V%x, DT\n", PC-2, X);
}

// LD VX, K
void I_FX0A() {
    if(KEYPAD[0]) V[X] = 0;
    else if(KEYPAD[1]) V[X] = 1;
    else if(KEYPAD[2]) V[X] = 2;
    else if(KEYPAD[3]) V[X] = 3;
    else if(KEYPAD[4]) V[X] = 4;
    else if(KEYPAD[5]) V[X] = 5;
    else if(KEYPAD[6]) V[X] = 6;
    else if(KEYPAD[7]) V[X] = 7;
    else if(KEYPAD[8]) V[X] = 8;
    else if(KEYPAD[9]) V[X] = 9;
    else if(KEYPAD[0xA]) V[X] = 0xAu;
    else if(KEYPAD[0xB]) V[X] = 0xBu;
    else if(KEYPAD[0xC]) V[X] = 0xCu;
    else if(KEYPAD[0xD]) V[X] = 0xDu;
    else if(KEYPAD[0xE]) V[X] = 0xEu;
    else if(KEYPAD[0xF]) V[X] = 0xFu;
    else PC -= 2;
    printf("$%x: LD V%x, K\n", PC-2, X);
}

// LD DT, VX
void I_FX15() {
    DELAY_TIMER = V[X];
    printf("$%x: LD DT, V%x\n", PC-2, X);
}

// LD ST, VX
void I_FX18() {
    SOUND_TIMER = V[X];
    printf("$%x: LD ST, V%x\n", PC-2, X);
}

// ADD I, VX
// Set I = I + VX
void I_FX1E() {
    I += V[X];
    printf("$%x: ADD I, V%x\n", PC-2, X);
}

// LD F, VX
// Set I = location of sprite for digit VX
void I_FX29() {
    I = MEM[V[X] * 5];
    printf("$%x: LD F, V%x\n", PC-2, X);
}

// LD B, VX
void I_FX33() {
    MEM[I] = V[X] / 100;
    MEM[I + 1] = (V[X] % 100) / 10;
    MEM[I + 2] = (V[X] % 100) % 10;
    printf("$%x: LD B, V%x\n", PC-2, X);
}

// LD [I], VX
void I_FX55() {
    for(int i = 0; i <= X; i++) {
        MEM[I + i] = V[i];
    }
    printf("$%x: LD [I], V%x\n", PC-2, X);
}

// LD VX, [I]
void I_FX65() {
    for(int i = 0; i <= X; i++) {
        V[i] = MEM[I + i];
    }
    printf("$%x: LD V%x, [I]\n", PC-2, X);
}

void init_OLT() {
    TABLE_00E[0x0] = &I_00E0;
    TABLE_00E[0xE] = &I_00EE;
    
    TABLE_UNQ[0x1] = &I_1NNN;
    TABLE_UNQ[0x2] = &I_2NNN;
    TABLE_UNQ[0x3] = &I_3XKK;
    TABLE_UNQ[0x4] = &I_4XKK;
    TABLE_UNQ[0x5] = &I_5XY0;
    TABLE_UNQ[0x6] = &I_6XKK;
    TABLE_UNQ[0x7] = &I_7XKK;
    TABLE_UNQ[0x9] = &I_9XY0;
    TABLE_UNQ[0xA] = &I_ANNN;
    TABLE_UNQ[0xB] = &I_BNNN;
    TABLE_UNQ[0xC] = &I_CXKK;
    TABLE_UNQ[0xD] = &I_DXYN;
    
    TABLE_8XY[0x0] = &I_8XY0;
    TABLE_8XY[0x1] = &I_8XY1;
    TABLE_8XY[0x2] = &I_8XY2;
    TABLE_8XY[0x3] = &I_8XY3;
    TABLE_8XY[0x4] = &I_8XY4;
    TABLE_8XY[0x5] = &I_8XY5;
    TABLE_8XY[0x6] = &I_8XY6;
    TABLE_8XY[0x7] = &I_8XY7;
    TABLE_8XY[0xE] = &I_8XYE;
    
    TABLE_EX[0xE] = &I_EX9E;
    TABLE_EX[0x1] = &I_EXA1;
    
    TABLE_FX[0x07] = &I_FX07;
    TABLE_FX[0x0A] = &I_FX0A;
    TABLE_FX[0x15] = &I_FX15;
    TABLE_FX[0x18] = &I_FX18;
    TABLE_FX[0x1E] = &I_FX1E;
    TABLE_FX[0x29] = &I_FX29;
    TABLE_FX[0x33] = &I_FX33;
    TABLE_FX[0x55] = &I_FX55;
    TABLE_FX[0x65] = &I_FX65;
}
/* * * * * * * * * OPCODE LOOKUP TABLE * * * * * * * */

void execute() {
    switch(HB_1) {
    	case 0x8u:
    	{
    	    (*TABLE_8XY[HB_4])(); break;
    	}
    	case 0x0u:
    	{
    	    (*TABLE_00E[HB_4])(); break;
    	}
    	case 0xEu:
    	{
    	    (*TABLE_EX[HB_4])(); break;
    	}
    	case 0xFu:
    	{
    	    uint8_t fpt_idx = (HB_3 << 4) | (HB_4); 
    	    (*TABLE_FX[fpt_idx])(); break;
    	}
    	default:
    	{
    	    (*TABLE_UNQ[HB_1])();
    	}
    }
}

void cycle() {
    fetch();
    decode();
    execute();
}

class SDL_GFX {
    public:
        int SCREEN_WIDTH = 640;
        int SCREEN_HEIGHT = 320;
        int SCALE = 10;

        SDL_Window* WIN;
        SDL_Renderer* REN;

        void init_SDL_GFX() {
            SDL_Init(SDL_INIT_VIDEO);

            WIN = SDL_CreateWindow("CHIP-8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                    SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

            REN = SDL_CreateRenderer(WIN, -1, 0);

            SDL_RenderSetScale(REN, SCALE, SCALE);
            SDL_SetRenderDrawColor(REN, 0, 0, 0, 255);
            SDL_RenderClear(REN);
            SDL_RenderPresent(REN);
        } 

        void free_SDL_GFX() {
            SDL_DestroyRenderer(REN);
            SDL_DestroyWindow(WIN);
            SDL_Quit();
        }

        void update_WIN() {
            for(int ROW = 0; ROW < 32; ROW++) {
                for(int COL = 0; COL < 64; COL++) {
                    uint16_t ADDR = (COL) + (ROW) * 64;
                    if(DIS_MEM[ADDR] == 0) { SDL_SetRenderDrawColor(REN, 0, 0, 0, 255); }
                    else if(DIS_MEM[ADDR] == 1) { SDL_SetRenderDrawColor(REN, 255, 255, 255, 255); }
                    SDL_RenderDrawPoint(REN, COL, ROW);
                }
            }
            SDL_RenderPresent(REN);
        }

        bool input() {
            bool quit = false;

            SDL_Event event;

            while(SDL_PollEvent(&event))
            {
                // A key is pressed
                if(event.type == SDL_KEYDOWN) {
                    switch(event.key.keysym.sym) {				
                        case SDLK_ESCAPE:
                            quit = true;
                            break;

                        case SDLK_x:
                            KEYPAD[0] = 1;
                            break;

                        case SDLK_1:
                            KEYPAD[1] = 1;
                            break;

                        case SDLK_2:
                            KEYPAD[2] = 1;
                            break;

                        case SDLK_3:
                            KEYPAD[3] = 1;
                            break;

                        case SDLK_q:
                            KEYPAD[4] = 1;
                            break;

                        case SDLK_w:
                            KEYPAD[5] = 1;
                            break;

                        case SDLK_e:
                            KEYPAD[6] = 1;
                            break;

                        case SDLK_a:
                            KEYPAD[7] = 1;
                            break;

                        case SDLK_s:
                            KEYPAD[8] = 1;
                            break;

                        case SDLK_d:
                            KEYPAD[9] = 1;
                            break;

                        case SDLK_y:
                            KEYPAD[0xAu] = 1;
                            break; 

                        case SDLK_c:
                            KEYPAD[0xBu] = 1;
                            break;

                        case SDLK_4:
                            KEYPAD[0xCu] = 1;
                            break;

                        case SDLK_r:
                            KEYPAD[0xDu] = 1;
                            break;

                        case SDLK_f:
                            KEYPAD[0xEu] = 1;
                            break;

                        case SDLK_v:
                            KEYPAD[0xFu] = 1;
                            break;
                    }
                }

                // A key stops being pressed
                else if(event.type == SDL_KEYUP) {
                    switch(event.key.keysym.sym) {
                        case SDLK_x:
                            KEYPAD[0] = 1;
                            break;

                        case SDLK_1:
                            KEYPAD[1] = 0;
                            break;

                        case SDLK_2:
                            KEYPAD[2] = 0;
                            break;

                        case SDLK_3:
                            KEYPAD[3] = 0;
                            break;

                        case SDLK_q:
                            KEYPAD[4] = 0;
                            break;

                        case SDLK_w:
                            KEYPAD[5] = 0;
                            break;

                        case SDLK_e:
                            KEYPAD[6] = 0;
                            break;

                        case SDLK_a:
                            KEYPAD[7] = 0;
                            break;

                        case SDLK_s:
                            KEYPAD[8] = 0;
                            break;

                        case SDLK_d:
                            KEYPAD[9] = 0;
                            break;

                        case SDLK_y:
                            KEYPAD[0xAu] = 0;
                            break; 

                        case SDLK_c:
                            KEYPAD[0xBu] = 0;
                            break;

                        case SDLK_4:
                            KEYPAD[0xCu] = 0;
                            break;

                        case SDLK_r:
                            KEYPAD[0xDu] = 0;
                            break;

                        case SDLK_f:
                            KEYPAD[0xEu] = 0;
                            break;

                        case SDLK_v:
                            KEYPAD[0xFu] = 0;
                            break;
                    }
                }

                else if(event.type == SDL_QUIT) {
                    quit = true;
                }
            }
            return quit;
        }
};

int main(int argc, char** argv) {
    SDL_GFX S;

    char* ROM_FILE = argv[1];
    float FREQ = std::stof(argv[2]);
    float TDELAY_MS = (1/FREQ) * 1000;

    init_CHIP8();
    init_OLT();
    S.init_SDL_GFX();

    load_ROM(ROM_FILE);

    bool LOOP = true;
    SDL_Event e;

    auto CYCLE_START = std::chrono::high_resolution_clock::now();

    while(LOOP) {
        LOOP = !S.input();

        auto CYCLE_END = std::chrono::high_resolution_clock::now();
        double TDIFF_MS = std::chrono::duration<double, std::milli>(CYCLE_END-CYCLE_START).count();

        if(TDIFF_MS > TDELAY_MS) {
            CYCLE_START = CYCLE_END;
            cycle();
            S.update_WIN();
        }

        CNT++;

        if(CNT == (int) FREQ/60) {
            CNT = 0;
            if(DELAY_TIMER > 0) {
                DELAY_TIMER--;
            }
            if(SOUND_TIMER > 0) {
                SOUND_TIMER--;
            }
        }
    }

    S.free_SDL_GFX();
    return 0;
}
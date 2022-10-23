#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include <stdio.h>

#include <SDL.h>

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif

#include "../Chip8/Chip8.h"

#define BUF_SIZE 20

class Application {
    public:
        Application() {
            // Initialize SDL subsystems
            if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == -1)
            {
                printf("SDL_Error: %s\n", SDL_GetError());
                std::exit(1); // Indicate unsuccessful program termination
            }

            // Decide GL+GLSL versions
            #if defined(IMGUI_IMPL_OPENGL_ES2)
            // GL ES 2.0 + GLSL 100
            const char* glsl_version = "#version 100";
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
            #elif defined(__APPLE__)
            // GL 3.2 Core + GLSL 150
            const char* glsl_version = "#version 150";
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
            #else
            // GL 3.0 + GLSL 130
            const char* glsl_version = "#version 130";
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
            #endif

            // Create window with graphics context
            SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
            SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
            SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
            SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

            SDL_DisplayMode displayMode;
            SDL_GetCurrentDisplayMode(0, &displayMode);
            auto displayWidth = displayMode.w;
            auto displayHeight = displayMode.h;

            window = SDL_CreateWindow("FillieChip8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, displayWidth, displayHeight, window_flags);
            gl_context = SDL_GL_CreateContext(window);
            SDL_GL_MakeCurrent(window, gl_context);
            SDL_GL_SetSwapInterval(1); // Enable vsync

            // Setup Dear ImGui context
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO(); (void)io;

            // Setup Dear ImGui style
            ImGui::StyleColorsDark();

            // Setup Platform/Renderer backends
            ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
            ImGui_ImplOpenGL3_Init(glsl_version);

            // Load fonts
            quinqueFiveText = io.Fonts->AddFontFromFileTTF("quinquefive.ttf", 25.0f);
            quinqueFiveSubtext = io.Fonts->AddFontFromFileTTF("quinquefive.ttf", 21.25f);
        }

        ~Application() {
            // Cleanup
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplSDL2_Shutdown();
            ImGui::DestroyContext();

            SDL_GL_DeleteContext(gl_context);
            SDL_DestroyWindow(window);
            SDL_Quit();
        }

        void setUpNewFrame() {
            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL2_NewFrame();
            ImGui::NewFrame();
        }

        void render() {
            // Rendering
            ImGui::Render();
            ImGuiIO& io = ImGui::GetIO(); (void)io; // Access the Dear ImGui IO structure
            glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
            glClearColor(clearColor.x * clearColor.w, clearColor.y * clearColor.w, clearColor.z * clearColor.w, clearColor.w);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            SDL_GL_SwapWindow(window);
        }

    public:
        void run();
        void startUp();
        void update();
        
        void setKeyState(SDL_Event event);
        bool eventHandler();

    // Chip-8
    public:
        Chip8 emu;
        char* rom;
        int romSize;
        uint8_t opcodesPerFrame;
        unsigned long long int cycles = 0;
        
    // Emulator
    public:
        SDL_Window* window;
        SDL_GLContext gl_context;
        ImFont* quinqueFiveText;
        ImFont* quinqueFiveSubtext;
        
    // Display
    public:
        uint8_t bgColoru[3] = {0x26, 0x4b, 0x59};
        uint8_t fgColoru[3] = {0x99, 0xb0, 0xb6};

        ImVec4 bgColorf = ImVec4(bgColoru[0]/255.0f, bgColoru[1]/255.0f, bgColoru[2]/255.0f, 1.0f);
        ImVec4 fgColorf = ImVec4(fgColoru[0]/255.0f, fgColoru[1]/255.0f, fgColoru[2]/255.0f, 1.0f);

        ImVec4 tertColorf = ImVec4(0xf1/255.0f, 0x50/255.0f, 0x25/255.0f, 1.0f);

        ImVec4 clearColor = bgColorf;
        
        GLuint textureDisplay;
        void initTextureDisplay();
        void updateTextureDisplay();

        void display();
    
    // Registers
    public:
        void registers();

    // Status
    public:
        void status();

    // Sprite
    public:
        uint8_t spriteData[8 * 15];
        void getSpriteData();

        GLuint textureSprite;
        void initTextureSprite();
        void updateTextureSprite();

        void sprite();

    // Execution
    public:;
        char buf[BUF_SIZE];
        void writeInstrToBuf(uint16_t addr); 

        void execution();

    // Stack
    public:
        void stack();
};
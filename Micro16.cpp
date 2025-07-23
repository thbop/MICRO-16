/*
* Copyright © 2025 Micro-16 Team
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the “Software”), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#include <iostream>
#include <fstream>

#include "stdio.h"

#define TITLE             "Micro-16"
#define SCREEN_RESOLUTION 128
#define WINDOW_RATIO      6
#define WINDOW_RESOLUTION ( SCREEN_RESOLUTION * WINDOW_RATIO )

// Switches
// Ignore development package
// #define RUNTIME

// #define BTP_DEBUG

// #define PGU_DEBUG

#include "MiDi16/MicroDisplay16.hpp"
#include "bob3000/Bob.hpp"
#include "btp6000/Btp.hpp"
#include "pgu7000/Pgu.hpp"
#include "cartlink/CartLink.hpp"

// Main class
class Micro16 {
public:
    // Initialize everything
    Micro16() {
        // CPU
        cpu.Reset();
        cpu.SetMemory( &memory );

        // Display
        window =
            new MiDi16::Window( TITLE, WINDOW_RESOLUTION, WINDOW_RESOLUTION );
        screen = new MiDi16::Surface( SCREEN_RESOLUTION, SCREEN_RESOLUTION );

        // GPU
        gpu = new pgu::PixelGraphicsUnit( screen );
        gpu->SetMemory( &memory );

        #ifndef RUNTIME
        editor = new Editor( window, screen );
        #endif
    }

    // Destroy resources
    ~Micro16() {
        delete screen;
        delete window;
        delete gpu;

        #ifndef RUNTIME
        delete editor;
        #endif

        #ifdef BTP_DEBUG
        cpu.DumpMemory( "build/memory.bin" );
        #endif
    }

    // Update loop
    void Update();

    // Draw loop
    void Draw();

    // Main loop
    void Run();

    // Loads a cartridge into memory
    void LoadCart( const std::string &cartName );

private:
    Bob3k memory;
    btp::BetterThanPico cpu;
    pgu::PixelGraphicsUnit *gpu;

    MiDi16::Window *window;
    MiDi16::Surface *screen;

    #ifndef RUNTIME
    Editor *editor;

    enum State {
        GAME,
        EDITOR,
    };
    int state = EDITOR;
    #endif
};

// Loads a cartridge into memory
void Micro16::LoadCart( const std::string &cartName ) {
    std::ifstream file( cartName, std::ios::binary );
    if ( !file.is_open() ) {
        std::cout << "Could not open cartridge \"" << cartName << "\"!\n";
        return;
    }

    // Calculate file size (to make sure it is a valid cartridge)
    std::streamsize fileSize = file.tellg();
    file.seekg( 0, std::ios::end );
    fileSize = file.tellg() - fileSize;
    if ( fileSize != BOB3K_SIZE ) {
        std::cout << "Invalid cartridge size! Must be exactly 64K!\n";
        file.close();
        return;
    }

    // Seek back and read
    file.seekg( 0, std::ios::beg );
    file.read( (char*)memory.data(), BOB3K_SIZE );

    file.close();
}


// Update loop
void Micro16::Update() {
    // if ( window->IsKeyPressed( MiDi16::KEY_F11 ) )
    cpu.Execute();
}

// Draw loop
void Micro16::Draw() {
    // if ( window->IsKeyPressed( MiDi16::KEY_F11 ) )
    gpu->Run();
}

// Main loop
void Micro16::Run() {
    // Hardcoded load cartridge
    LoadCart( "build/pong/pong.cart" );

    while ( window->IsRunning() ) {
        window->PollEvents();
        #ifdef RUNTIME
        Update();
        #else
        // Basic state management
        if ( window->IsKeyPressed( MiDi16::KEY_F5 ) ) {
            cpu.Reset();
            cpu.CS = 0x800; // Hardcode the code segment
            state = GAME;
        }
        else if ( window->IsKeyPressed( MiDi16::KEY_ESC ) ) {
            state = EDITOR;
        }


        switch ( state ) {
            case GAME:   Update();         break;
            case EDITOR: editor->Update(); break;
        }
        #endif

        screen->Clear();
        #ifdef RUNTIME
        Draw();
        #else
        switch ( state ) {
            case GAME:   Draw();         break;
            case EDITOR: editor->Draw(); break;
        }
        #endif

        screen->BlitFill( window );
        window->Flip();
    }
}


int main() {
    Micro16 micro16;
    micro16.Run();

    return 0;
}
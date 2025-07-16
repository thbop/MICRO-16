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

#include "stdio.h"

#define TITLE             "Micro-16"
#define SCREEN_RESOLUTION 128
#define WINDOW_RATIO      6
#define WINDOW_RESOLUTION ( SCREEN_RESOLUTION * WINDOW_RATIO )

// Ignore development package
// #define RUNTIME

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


// Update loop
void Micro16::Update() {
    // if ( window->IsKeyPressed( MiDi16::KEY_F11 ) )
    cpu.Execute();
}

// Draw loop
void Micro16::Draw() {
    gpu->RenderSprite( 0, 0, 10, 10 );
}

// Main loop
void Micro16::Run() {
    // Load hardcoded program into memory
    uint8_t program[] = {
        0xCA, 0x65, 0x52, 0x5E, 0x52, 0x50, 0xA0, 0xC0, 0x03, 0xAE, 0xA0, 0x00,
        0x00, 0xA7, 0x00, 0xA0, 0x08, 0x00, 0xA7, 0x01, 0xA0, 0x0A, 0x00, 0xA7,
        0x02, 0xA0, 0x02, 0x00, 0xA7, 0x03, 0xA0, 0x00, 0x03, 0xAE, 0xA0, 0x00,
        0x00, 0xA7, 0x00, 0xA0, 0x2C, 0x00, 0xA7, 0x01, 0xA0, 0x7E, 0x00, 0xA7,
        0x02, 0xA7, 0x03, 0xA7, 0x04, 0xA7, 0x05, 0xA7, 0x06, 0xA0, 0x3C, 0x00,
        0xA7, 0x07, 0xA0, 0x18, 0x00, 0xA7, 0x08, 0xA0, 0x10, 0x00, 0xA7, 0x09,
        0xA0, 0x08, 0x00, 0xA7, 0x0B, 0xA7, 0x0C, 0xA0, 0x10, 0x00, 0xA7, 0x0D,
        0x51, 0x53, 0xBE, 0x53, 0x5B, 0x50, 0x52, 0xA0, 0x0A, 0x00, 0xB0, 0x05,
        0x00, 0xC0, 0xC8, 0xF7, 0x51, 0x53, 0x5B, 0xA0, 0x00, 0x02, 0xAC, 0x5A,
        0x02, 0x00, 0x5A, 0x59, 0x00, 0xCA, 0xFE,
    };
    memcpy( memory.data() + 0x8000, program, sizeof(program) );

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
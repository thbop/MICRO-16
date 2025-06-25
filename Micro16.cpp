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
#define WINDOW_RATIO      4
#define WINDOW_RESOLUTION ( SCREEN_RESOLUTION * WINDOW_RATIO )

#include "MiDi16/MicroDisplay16.hpp"
#include "bob3000/Bob.hpp"
#include "btp6000/Btp.hpp"
#include "pgu7000/Pgu.hpp"

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
    }

    // Destroy resources
    ~Micro16() {
        delete screen;
        delete window;
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
};


// Update loop
void Micro16::Update() {

}

// Draw loop
void Micro16::Draw() {
    gpu->RenderSprite( 0, 0, 10, 10 );
}

// Main loop
void Micro16::Run() {
    while ( window->IsRunning() ) {
        window->PollEvents();
        Update();

        screen->Clear();
        Draw();

        screen->BlitFill( window );
        window->Flip();
    }
}


int main() {
    Micro16 micro16;
    micro16.Run();

    return 0;
}
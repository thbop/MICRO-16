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

#ifndef RUNTIME
#ifndef CARTLINK_HPP
#define CARTLINK_HPP

// Essentially the editor + linker

#include "../MiDi16/MicroDisplay16.hpp"

// Cartlink editor
class Editor {
public:
    // Empty constructor
    Editor() {}
    // Constructor
    Editor( MiDi16::Surface *screen ) : screen( screen ) {
        font = MiDi16::Surface::FromImage( "resources/font.png" );
    }

    // Clean up resources
    ~Editor() {
        delete font;
    }

    // Update loop
    void Update() {}

    // Draw loop
    void Draw() {
        screen->Blit( font, (MiDi16::Rect){ 15, 0, 3, 5 }, (MiDi16::Rect){ 0, 0, 3, 5 } );
    }

private:
    MiDi16::Surface *screen; // Managed by Micro16 class
    MiDi16::Surface *font;
};

#endif
#endif
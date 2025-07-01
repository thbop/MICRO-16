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
#include "Gui.hpp"

// Cartlink editor
class Editor {
public:
    // Empty constructor
    Editor() {}
    // Constructor
    Editor( MiDi16::Window *window, MiDi16::Surface *screen )
        : window( window ), screen( screen ) {
        textbox = new gui::TextBox( window, screen, { 1, 8, 126, 120 } );

    }

    // Clean up resources
    ~Editor() {
        delete textbox;
    }

    // Update loop
    void Update() {}

    // Draw loop
    void Draw() {
        textbox->Run();
    }

private:
    MiDi16::Window *window;  // Managed by Micro16 class
    MiDi16::Surface *screen; // Managed by Micro16 class
    gui::TextBox *textbox;
};

#endif
#endif
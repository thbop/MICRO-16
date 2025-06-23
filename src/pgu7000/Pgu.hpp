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

#ifndef PGU_HPP
#define PGU_HPP

#include "bob3000/Bob.hpp"
#include "MiDi16/MicroDisplay16.hpp"

// Pixel Graphics Unit

// PGU namespace
namespace pgu {

enum Colors {
    BLACK, GRAY, WHITE, PEACH,
    MAGENTA, PURPLE, RED, ORANGE,
    YELLOW, LIME, GREEN, DARK_BLUE,
    BLUE, LIGHT_BLUE, BROWN, CHOCOLATE,
    COLOR_COUNT,
};

// PGU class
class PixelGraphicsUnit {
public:
    // Empty constructor
    PixelGraphicsUnit() {}

    // Constructor
    PixelGraphicsUnit( MiDi16::Surface *screen ) : screen( screen ) {}

    // Sets the memory just like the CPU
    void SetMemory( Bob3k *memory ) {
        bob = memory;
    }

private:
    MiDi16::Surface *screen;
    Bob3k *bob;

    // https://lospec.com/palette-list/anb16
    const MiDi16::Color colors[COLOR_COUNT] = {
        0x0A080D00, 0x69759400, 0xDFE9F500, 0xF7AAA800,
        0xD4689A00, 0x782C9600, 0xE8356200, 0xF2825C00,
        0xFFC76E00, 0x88C44D00, 0x3F9E5900, 0x37346100,
        0x4854A800, 0x7199D900, 0x9E525200, 0x4D253600,
    };

    // Sets a color at a given position
    void SetPixel( uint8_t color, int x, int y ) {
        screen->Set( x, y, colors[color] );
    }
};

}


#endif
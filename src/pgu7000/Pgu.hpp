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

#include "stdint.h"

#include "bob3000/Bob.hpp"
#include "MiDi16/MicroDisplay16.hpp"

// Pixel Graphics Unit

// PGU namespace
namespace pgu {

constexpr int
    // Spritesheet stuff
    SPRITE_WIDTH             = 8, // Sprites are square
    SPRITE_SIZE              = 16, // Sprite size in bytes
    SPRITESHEET_SPRITE_COUNT = ( 16 * 8 ), // 16x8 sprites

    // Nametable stuff
    NAMETABLE_WIDTH          = SCREEN_RESOLUTION / SPRITE_WIDTH,
    NAMETABLE_ENTRY_COUNT    = NAMETABLE_WIDTH * NAMETABLE_WIDTH,
    NAMETABLE_ENTRY_SIZE     = 2, // 2 bytes
    NAMETABLE_SIZE           = NAMETABLE_ENTRY_SIZE * NAMETABLE_ENTRY_COUNT,

    // Palette stuff
    PALETTE_ENTRY_SIZE       = 3, // 3 colors
    PALETTE_ENTRY_COUNT      = 16,
    PALETTE_SIZE             = 1 + PALETTE_ENTRY_SIZE * PALETTE_ENTRY_COUNT,

    // Memory locations
    MEMORY_START             = 0x3000,
    SPRITESHEET              = MEMORY_START,
    NAMETABLE0               =
        SPRITESHEET + SPRITESHEET_SPRITE_COUNT * SPRITE_SIZE,
    NAMETABLE1               = NAMETABLE0 + NAMETABLE_SIZE,
    PALETTE                  = NAMETABLE1 + NAMETABLE_SIZE,
    
    // Registers
    REGISTER_MASK            = PALETTE + PALETTE_SIZE + 1, // Byte
    REGISTER_CONTROL         = REGISTER_MASK + 1,          // Byte
    REGISTER_DATA            = REGISTER_MASK + 2;          // Dword


// Colors
enum Colors {
    BLACK, GRAY, WHITE, PEACH,
    MAGENTA, PURPLE, RED, ORANGE,
    YELLOW, LIME, GREEN, DARK_BLUE,
    BLUE, LIGHT_BLUE, BROWN, CHOCOLATE,
    COLOR_COUNT,
};

// Registers
// Mask register
enum {
    MASK_NAMETABLE0 = 0x01,
    MASK_NAMETABLE1 = 0x02,
};

enum {
    CONTROL_DRAW_SPRITE     = 0x01,
    CONTROL_DRAW_BACKGROUND = 0x02,
    CONTROL_SET_SCROLL      = 0x04,
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
        
        mask = bob->data() + REGISTER_MASK;
        control = bob->data() + REGISTER_CONTROL;
        data = (uint32_t*)( bob->data() + REGISTER_DATA );
    }

    // Processes registers and draws
    void Run();

private:
    MiDi16::Surface *screen;
    Bob3k *bob;

    // Internal memory
    uint16_t
        scrollX = 0,
        scrollY = 0;
    
    // Registers
    uint8_t *mask;
    uint8_t *control;
    uint32_t *data;

    // https://lospec.com/palette-list/anb16
    const MiDi16::Color colors[COLOR_COUNT] = {
        { 0x0A, 0x08, 0x0D, 0xFF }, { 0x69, 0x75, 0x94, 0xFF },
        { 0xDF, 0xE9, 0xF5, 0xFF }, { 0xF7, 0xAA, 0xA8, 0xFF },
        { 0xD4, 0x68, 0x9A, 0xFF }, { 0x78, 0x2C, 0x96, 0xFF },
        { 0xE8, 0x35, 0x62, 0xFF }, { 0xF2, 0x82, 0x5C, 0xFF },
        { 0xFF, 0xC7, 0x6E, 0xFF }, { 0x88, 0xC4, 0x4D, 0xFF },
        { 0x3F, 0x9E, 0x59, 0xFF }, { 0x37, 0x34, 0x61, 0xFF },
        { 0x48, 0x54, 0xA8, 0xFF }, { 0x71, 0x99, 0xD9, 0xFF },
        { 0x9E, 0x52, 0x52, 0xFF }, { 0x4D, 0x25, 0x36, 0xFF },
    };

    // Sets a color at a given position
    void SetPixel( uint8_t color, int x, int y ) {
        screen->Set( x, y, colors[color] );
    }

    // Given sprite coordinates and a palette, renders a sprite at the given
    // position
    void DrawSprite( uint8_t sprite, uint8_t palette, int x, int y );
};


// Given sprite coordinates and a palette, renders a sprite at the given
// position
void PixelGraphicsUnit::DrawSprite(
    uint8_t sprite,
    uint8_t palette,
    int x, int y
) {
    uint16_t spriteAddress = SPRITESHEET + sprite * SPRITE_SIZE;
    uint16_t paletteAddress = PALETTE + 1 + palette * PALETTE_ENTRY_SIZE;

    for ( int j = 0; j < SPRITE_WIDTH; j++ ) {
        uint16_t rowAddress = spriteAddress + j;
        uint8_t lsb = bob->Read( rowAddress ); // Least significant bit
        uint8_t msb = bob->Read( rowAddress + SPRITE_WIDTH );

        // Iterate through the bits
        for ( int i = SPRITE_WIDTH - 1; i > -1; i-- ) {
            uint8_t paletteIndex = ( ( msb & 1 ) << 1 ) | ( lsb & 1 );
            lsb >>= 1;
            msb >>= 1;

            uint8_t colorIndex;
            if ( paletteIndex == 0 )
                colorIndex = bob->Read( PALETTE );
            else
                colorIndex = bob->Read( paletteAddress + paletteIndex - 1 );

            SetPixel( colorIndex, x + i, y + j );
        }
    }
}

// Processes registers and draws
void PixelGraphicsUnit::Run() {
    #ifdef PGU_DEBUG
    printf(
        "Mask: %02X Control: %02X Data: %04X\n",
        *mask, *control, *data
    );
    #endif


    switch ( *mask ) {}

    switch ( *control ) {
        case CONTROL_DRAW_SPRITE:
            DrawSprite(
                *data & 0xFF,             // Sprite
                ( (*data) >> 8 ) & 0xFF,  // Palette
                ( (*data) >> 24 ) & 0xFF, // X
                ( (*data) >> 16 ) & 0xFF  // Y
            );
            break;
    }
}

}


#endif
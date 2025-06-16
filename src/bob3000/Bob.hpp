/*
* Copyright © 2025 Micro-16 Team
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy of
* this software and associated documentation files (the “Software”), to deal in
* the Software without restriction, including without limitation the rights to
* use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
* of the Software, and to permit persons to whom the Software is furnished to do
* so, subject to the following conditions:
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

#ifndef BOB_HPP
#define BOB_HPP

#include "stdint.h"

#define BOB3K_SIZE 0x10000

// Buffer of Bytes 3000
// A class to manage memory
class Bob3k {
public:
    // Empty constructor
    Bob3k() {}

    // Getter
    // Similar to x86 memory segmentation:
    //     0xFFF segments
    //     Each segment is 0xFFF bytes wide
    //     Segments are separated by 0x10 bytes
    //
    // For example:
    //     FFF:00F = FFFF
    //     FF0:0FF = FFFF
    //     FFF:FFF = 0FEF
    uint8_t Read( uint16_t segment, uint16_t offset ) const {
        return buffer[CalculateIndex( segment, offset )];
    }

    // Returns a word
    uint16_t Read16( uint16_t segment, uint16_t offset ) const {
        return *(uint16_t*)( buffer + CalculateIndex( segment, offset ) );
    }

    // Setter
    void Write( uint16_t segment, uint16_t offset, uint8_t value ) {
        buffer[CalculateIndex( segment, offset )] = value;
    }

    // Writes a word
    void Write16( uint16_t segment, uint16_t offset, uint16_t value ) {
        *(uint16_t*)( buffer + CalculateIndex( segment, offset ) ) = value;
    }

private:
    uint8_t buffer[BOB3K_SIZE];

    uint16_t CalculateIndex( uint16_t segment, uint16_t offset ) const {
        return ( segment << 4 ) + offset;
    }
};

// Thbop Hex Printer
// Quick little namespace for returning printable streams of bytes and words
namespace thex {
    // Byte
    std::ostream& Byte( std::ostream &os ) {
        return os << std::uppercase << std::hex << std::setw( 2 )
            << std::setfill('0');
    }

    // Word
    std::ostream& Word( std::ostream &os ) {
        return os << std::uppercase << std::hex << std::setw( 4 )
            << std::setfill('0');
    }
}


#endif
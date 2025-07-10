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
    uint8_t Read( uint16_t address ) const {
        return buffer[address];
    }

    // Returns a word
    uint16_t Read16( uint16_t address ) const {
        return *(uint16_t*)( buffer + address );
    }

    // Setter
    void Write( uint16_t address, uint8_t value ) {
        buffer[address] = value;
    }

    // Writes a word
    void Write16( uint16_t address, uint16_t value ) {
        *(uint16_t*)( buffer + address ) = value;
    }

    // Raw data access
    uint8_t *data() const {
        return (uint8_t*)buffer;
    }

private:
    uint8_t buffer[BOB3K_SIZE];

};




#endif
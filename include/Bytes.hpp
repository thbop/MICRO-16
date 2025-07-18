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

#ifndef BYTES_HPP
#define BYTES_HPP

#include <vector>
#include <string>

#include "stdint.h"

// Handy bytes class

// A byte storage class
class Bytes {
public:
    // Empty constructor
    Bytes() {}

    // Appends a byte to the buffer
    void Append( uint8_t byte ) {
        data.push_back( byte );
        // PrintBytes();
    }

    // Appends a word to the buffer
    void Append( uint16_t word ) {
        data.push_back( (uint8_t)( word & 0xFF ) );
        data.push_back( (uint8_t)( word >> 8 ) );
        // PrintBytes();
    }

    // Appends a string of bytes to the buffer
    void Append( uint8_t *bytes, size_t size ) {
        size_t oldSize = data.size();
        data.resize( oldSize + size );

        memcpy( data.data() + oldSize, bytes, size );
    }

    // Appends a Bytes object to the buffer
    void Append( Bytes *bytes ) {
        Append( (uint8_t*)bytes->buffer(), bytes->size() );
    }

    // Appends a null-terminated string to the buffer
    void Append( std::string &str ) {
        Append( (uint8_t*)str.c_str(), str.size() + 1 );
    }

    // Returns the raw byte buffer
    uint8_t *buffer() const {
        return (uint8_t*)data.data();
    }

    // Returns the buffer size
    size_t size() const {
        return data.size();
    }

    // Resizes the buffer
    void resize( size_t newSize ) {
        data.resize( newSize );
    }

    #ifdef DEBUG

    // Prints a Bytes object's bytes
    void PrintBytes() {
        std::cout << "Bytes( ";
        for ( auto byte : data )
            printf("%02X ", byte);
        
        std::cout << ")\n";
    }

    #endif

private:
    std::vector<uint8_t> data;
};

#endif
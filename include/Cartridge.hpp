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

#ifndef CARTRIDGE_HPP
#define CARTRIDGE_HPP

// A simple cartridge storage class

#include <iostream>
#include <fstream>

#include "Bytes.hpp"
#include "Object.hpp"


#define CART_SIZE 0x10000

// Cartridge class
class Cartridge : public Bytes {
public:
    // Constructor
    Cartridge() {
        resize( CART_SIZE );
    }

    // Loads all input object files and constructs the cartridge
    // Returns false on error
    bool Load( const std::vector<std::string> &inputFiles );

    // Saves the resulting cartridge in the specified file
    void Dump( const std::string &outputFile );
};

// Loads all input object files and constructs the cartridge
// Returns false on error
bool Cartridge::Load( const std::vector<std::string> &inputFiles ) {
    // Iterate through object files
    for ( auto file : inputFiles ) {
        obj::Object object( file );

        memcpy(
            buffer() + object.header->origin,
            object.code->buffer(),
            object.code->size()
        );
    }

    return true;
}

// Saves the resulting cartridge in the specified file
void Cartridge::Dump( const std::string &outputFile ) {
    // Attempt to open write binary file
    std::ofstream file( outputFile, std::ios::binary );
    if ( !file.is_open() ) {
        std::cout << "Unable to save cartridge file \""
            << outputFile << "\"!\n";
        return;
    }

    // Write
    file.write( (const char*)buffer(), size() );

    // Close
    file.close();
}


#endif
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

#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#include <fstream>
#include "settings.hpp"

// Takes raw assembly code and makes it managable for the parser
class Lexer {
public:
    // Default constructor
    Lexer() {}

    // Accept the settings and open the file for lexing
    Lexer( Settings *settings ) {
        file.open( settings->inputFile, std::ios::in );

        if ( !file.is_open() ) {
            std::cout << "File \"" << settings->inputFile << "\" either does "
                "not exist or cannot be opened.\n";
            error = true;
        }
    }

    // Close the file
    ~Lexer() {
        file.close();
    }

private:
    std::ofstream file;
    bool error = false; // Prevent further action if an error has occured
};

#endif
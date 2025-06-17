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

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "stdint.h"

#include "stringextra.hpp"

#include "settings.hpp"

// Lexer namespace
namespace lex {

// A single lexed line
class Line {
public:
    // Basic line type
    enum Type {
        LINKER_DIRECTIVE, // e.g. .org
        MACRO,            // e.g. #define EE 23
        LABEL,            // e.g. main:
        INSTRUCTION,      // e.g. lda 34
        STATEMENT,        // e.g. db "Hello World!"
    };
    enum AddressingMode {
        IMMEDIATE,
        STACK_OFFSET,
        STACK_POINTER_OFFSET,
        DATA_OFFSET,
        DATA_POINTER_OFFSET,
    };

    std::string rawLine;

    int type;
    std::string label;  // If a label is used
    std::string string; // If a string is defined
    uint16_t number;    // If an immediate number is required
    int addressingMode; // If accessing memory

    Line() {}
    Line( std::string &line ) : rawLine( line ) {
        Lex();
    }

private:
    // Lexes a line
    void Lex();
};

// Lexes a line
void Line::Lex() {
    std::cout << rawLine << "\n";
}

// A container of Lines that defines a scope
class Scope : public Line {
public:
    // Constructors
    Scope() {}
    
    // Inherit Line constructor
    Scope( std::string line ) : Line( line ) {}

    // Cleanup
    ~Scope() {
        for ( auto it : lines )
            delete it;
    }

    // Allocate and add a line to the list
    void Add( std::string &line ) {
        // TODO: Probably something to test if the line is really a scope
        lines.push_back( new Line( line ) );
    }

private:
    std::vector<Line*> lines; // or scopes
};

// Takes raw assembly code and makes it managable for the parser
class Lexer {
public:
    Scope scope;

    // Default constructor
    Lexer() {}

    // Accept the settings and open the file for lexing
    Lexer( Settings *settings ) {
        file.open( settings->inputFile );

        if ( !file.is_open() )
            std::cout << "File \"" << settings->inputFile << "\" either does "
                "not exist or cannot be opened.\n";
    }

    // Begin lexing
    void Evaluate();

    // Close the file
    ~Lexer() {
        file.close();
    }

private:
    std::ifstream file;
};

// Begin lexing
void Lexer::Evaluate() {
    std::string line;
    while ( std::getline( file, line ) ) {
        line = stringextra::strip( line );
        if ( !line.empty() )
            scope.Add( line );
    }
}

}

#endif
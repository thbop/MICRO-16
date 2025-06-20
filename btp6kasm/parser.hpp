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

#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>

#include "stdint.h"

#include "lexer.hpp"

// Parser namespace
namespace parser {

class Bytes {
public:
    // Empty constructor
    Bytes() {}

    // Appends a byte to the buffer
    void Append( uint8_t byte ) {
        data.push_back( byte );
    }

    // Appends a string of bytes to the buffer
    void Append( uint8_t *bytes, size_t size ) {
        for ( size_t i = 0; i < size; i++ )
            Append( bytes[i] );
    }

    // Returns the raw byte buffer
    const uint8_t *buffer() const {
        return data.data();
    }

    // Returns the buffer size
    size_t size() const {
        return data.size();
    }

private:
    std::vector<uint8_t> data;
};

// Parses the lexed data and generated the object
class Parser {
public:
    Parser() {}
    Parser( lex::Lexer *lexer ) : lexer( lexer ) {}

    #ifdef DEBUG
    // Debug prints the lexed structure
    void PrintStructure() {
        PrintScope( &lexer->scope, 0 );
    }
    #endif

private:
    lex::Lexer *lexer;

    #ifdef DEBUG
    // Prints a scope from the lexer
    void PrintScope( lex::Scope *scope, int indent );

    // Prints a non-scope line from the lexer
    void PrintLine( lex::Line *line, int indent );
    #endif
};

#ifdef DEBUG
// Prints a non-scope line from the lexer
void Parser::PrintLine( lex::Line *line, int indent ) {
    std::cout << "Line." << line->number << ":\n";
    line->tokenStack.top()->Print( indent );
};

// Prints a scope from the lexer
void Parser::PrintScope( lex::Scope *scope, int indent ) {
    for ( auto it : scope->lines ) {
        lex::Scope *childScope = dynamic_cast<lex::Scope*>(it);
        if ( childScope )
            PrintScope( childScope, indent + 1 );
        else {
            PrintLine( it, indent + 1 );
        }
    }
}
#endif

}

#endif
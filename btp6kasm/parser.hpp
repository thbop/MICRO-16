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

#define PARSER_OBJECT_METADATA_END 0xFF


#include <vector>

#include "stdint.h"

#include "settings.hpp"
#include "lexer.hpp"

// Parser namespace
namespace parser {

// An inefficient but simple byte storage class
class Bytes {
public:
    // Empty constructor
    Bytes() {}

    // Appends a byte to the buffer
    void Append( uint8_t byte ) {
        data.push_back( byte );
    }

    // Appends a word to the buffer
    void Append( uint16_t word ) {
        data.push_back( word & 0xFF );
        data.push_back( word >> 8 );
    }

    // Appends a string of bytes to the buffer
    void Append( uint8_t *bytes, size_t size ) {
        for ( size_t i = 0; i < size; i++ )
            Append( bytes[i] );
    }

    // Appends a Bytes object to the buffer
    void Append( Bytes *bytes ) {
        Append( (uint8_t*)bytes->buffer(), bytes->size() );
    }

    // Empties the buffer
    void Clear() {

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

// Byte chunk storage class
class Chunk : public Bytes {
public:
    // Chunk type
    enum Type {
        HEADER,   // Basic information about the whole file for the linker
        CODE,     // Code blocks
        DATA,     // Data blocks
    };
    int type = HEADER;

    // Empty constructor
    Chunk() {}

};

// Basic information about the whole file for the linker
// Also includes external references
class Header : public Chunk {
public:

};

// Class to manage the generated object
class Object : public Bytes {
public:
    // Empty constructor
    Object() {}

    // Adds a chunk to the internal list
    void AddChunk( Chunk *chunk ) {
        chunks.push_back( chunk );
    }

    // Construct the chunks into the buffer with the correct metadata
    void Build();

private:
    std::vector<Chunk*> chunks;
};

// Construct the chunks into the buffer with the correct metadata
void Object::Build() {
    Clear();

    // Each metadata entry is 3 bytes + the 1 end byte
    // This is kinda like a very basic file system to guide the linker
    uint16_t pointer = chunks.size() * 3 + 1;
    for ( Chunk *chunk : chunks ) {
        Append( (uint8_t)chunk->type );
        Append( pointer );
        
        pointer += (uint16_t)chunk->size(); // Increment to the next chunk
    }
    Append( (uint8_t)PARSER_OBJECT_METADATA_END );

    // Actually add the data
    for ( Chunk *chunk : chunks ) {
        Append( chunk );
    }
}

// Parses the lexed data and generated the object
class Parser {
public:
    // Constructors
    Parser() {}
    Parser( lex::Lexer *lexer, Settings *settings ) : lexer( lexer ) {
        file.open( settings->outputFile, std::ios::binary );
        if ( !file.is_open() )
            std::cout << "File \"" << settings->outputFile << "\" either does "
                "not exist or cannot be opened.\n";
        
    }

    // Close the file
    ~Parser() {
        if ( file.is_open() )
            file.close();
    }

    // Parse the lex structure and output the file
    void Parse();

    #ifdef DEBUG
    // Debug prints the lexed structure
    void PrintStructure() {
        PrintScope( &lexer->scope, 0 );
    }
    #endif

private:
    lex::Lexer *lexer;
    Object output;
    std::ofstream file;

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

// Parse the lex structure and output the file
void Parser::Parse() {
    // Hardcoded chunk creation
    uint8_t data[] = "Hello World!";
    uint8_t data2[] = "Second chunk!";
    Chunk chunk, chunk2;
    chunk.Append( data, sizeof(data) );
    chunk2.Append( data2, sizeof(data2) );

    output.AddChunk( &chunk );
    output.AddChunk( &chunk2 );
    output.Build();

    file.write( (const char*)output.buffer(), output.size() );
    file.close();
}

}

#endif
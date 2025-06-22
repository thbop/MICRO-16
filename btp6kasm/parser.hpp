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

#define PARSER_OBJECT_METADATA_END          0xFF
#define PARSER_OBJECT_METADATA_ENTRY_LENGTH 3


#include <vector>
#include <unordered_map>

#include "stdint.h"

#include "../src/btp6000/Instructions.hpp"

#include "settings.hpp"
#include "lexer.hpp"
using namespace lex;

// Parser namespace
namespace parser {

// Instruction namespace
namespace ins {

enum AddressingMode {
    NONE = 0b00000, // None
    IM   = 0b00001, // Immediate
    SO   = 0b00010, // Stack offset
    SPO  = 0b00100, // Stack pointer offset
    DO   = 0b01000, // Data offset
    DPO  = 0b10000, // Data pointer offset
};

struct Info {
    uint8_t baseOpcode;
    uint8_t addressingModes;
};

// Instructions
const std::unordered_map<std::string, Info> instructions = {
    // Accumulator stuff
    { "lda",  { INS_LDA_IM, IM | SO | SPO | DO | DPO } },
    { "sta",  { INS_STA_SO - 1, SO | SPO | DO | DPO } },
    { "tab",  { INS_TAB, NONE } },
    { "tax",  { INS_TAX, NONE } },
    { "tay",  { INS_TAY, NONE } },
    { "tass", { INS_TASS, NONE } },
    { "tacs", { INS_TACS, NONE } },
    { "tads", { INS_TADS, NONE } },

    // Base stuff
    { "ldb",  { INS_LDB_IM, IM | SO | SPO | DO | DPO } },
    { "stb",  { INS_STB_SO - 1, SO | SPO | DO | DPO } },
    { "tba",  { INS_TBA, NONE } },
    { "tbx",  { INS_TBX, NONE } },
    { "tby",  { INS_TBY, NONE } },
    { "tbss", { INS_TBSS, NONE } },
    { "tbcs", { INS_TBCS, NONE } },
    { "tbds", { INS_TBDS, NONE } },

    // X-index stuff
    { "ldx",  { INS_LDX_IM, IM | SO | SPO | DO | DPO } },
    { "stx",  { INS_STX_SO - 1, SO | SPO | DO | DPO } },
    { "txa",  { INS_TXA, NONE } },
    { "txb",  { INS_TXB, NONE } },
    { "txy",  { INS_TXY, NONE } },
    { "txss", { INS_TXSS, NONE } },
    { "txcs", { INS_TXCS, NONE } },
    { "txds", { INS_TXDS, NONE } },

    // Y-pointer stuff
    { "ldy",  { INS_LDY_IM, IM | SO | SPO | DO | DPO } },
    { "sty",  { INS_STY_SO - 1, SO | SPO | DO | DPO } },
    { "tya",  { INS_TYA, NONE } },
    { "tyb",  { INS_TYB, NONE } },
    { "tyx",  { INS_TYX, NONE } },
    { "tyss", { INS_TYSS, NONE } },
    { "tycs", { INS_TYCS, NONE } },
    { "tyds", { INS_TYDS, NONE } },

};

}

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
        size_t oldSize = data.size();
        data.resize( oldSize + size );

        memcpy( data.data() + oldSize, bytes, size );
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

    // Construct the correct bytes for the particular chunk type
    virtual void Build() {}

};


// Basic information about the whole file for the linker
// Also includes external references
class Header : public Chunk {
public:
    uint16_t origin; // .org

    Header() {
        type = HEADER;
    }

    // Build the header
    void Build() override {
        Append( origin );
    }
};

// Instruction chunk
class Code : public Chunk {
public:
    Code() {
        type = CODE;
    }

    // Adds a label
    void AddLabel() {}

private:
    std::unordered_map<std::string, uint16_t> labels;
};

// Class to manage the generated object
class Object : public Bytes {
public:
    std::vector<Chunk*> chunks;
    Header *header;

    // Setup basic object
    Object() {
        header = new Header();
        chunks.push_back( header );
    }

    // Adds a chunk to the internal list
    void AddChunk( Chunk *chunk ) {
        chunks.push_back( chunk );
    }

    // Construct the chunks into the buffer with the correct metadata
    void Build();

};

// Construct the chunks into the buffer with the correct metadata
void Object::Build() {
    Clear();

    // Each metadata entry is 3 bytes + the 1 end byte
    // This is kinda like a very basic file system to guide the linker
    uint16_t pointer = chunks.size() * PARSER_OBJECT_METADATA_ENTRY_LENGTH + 1;
    for ( Chunk *chunk : chunks ) {
        Append( (uint8_t)chunk->type );
        Append( pointer );
        
        pointer += (uint16_t)chunk->size(); // Increment to the next chunk
    }
    Append( (uint8_t)PARSER_OBJECT_METADATA_END );

    // Actually add the data
    for ( Chunk *chunk : chunks ) {
        chunk->Build();
        Append( chunk );
    }
}

// Parses the lexed data and generated the object
class Parser {
public:
    // Constructors
    Parser() {}
    Parser( Lexer *lexer, Settings *settings ) : lexer( lexer ) {
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
    Lexer *lexer;
    Object output;
    std::ofstream file;

    #ifdef DEBUG
    // Prints a scope from the lexer
    void PrintScope( Scope *scope, int indent );

    // Prints a non-scope line from the lexer
    void PrintLine( Line *line, int indent );
    #endif

    // Parses a scope
    void ParseScope( Scope *scope );

    // Parses a line
    void ParseLine( Line *line );

    // Parses a linker directive
    void ParseLinkerDirective( token::LinkerDirective *token );
};

#ifdef DEBUG
// Prints a non-scope line from the lexer
void Parser::PrintLine( Line *line, int indent ) {
    std::cout << "Line." << line->number << ":\n";
    line->tokenStack.top()->Print( indent );
};

// Prints a scope from the lexer
void Parser::PrintScope( Scope *scope, int indent ) {
    for ( auto it : scope->lines ) {
        Scope *childScope = dynamic_cast<Scope*>(it);
        if ( childScope )
            PrintScope( childScope, indent + 1 );
        else {
            PrintLine( it, indent + 1 );
        }
    }
}
#endif

// Parses a linker directive
void Parser::ParseLinkerDirective( token::LinkerDirective *token ) {
    if ( token->value == ".org" ) {
        token::Number *number = (token::Number*)token->subTokens[0];
        output.header->origin = number->value;
    }
}

// Parses a line
void Parser::ParseLine( Line *line ) {
    token::Token *token = line->tokenStack.top();
    switch ( token->type ) {
        case token::LINKER_DIRECTIVE:
            ParseLinkerDirective( (token::LinkerDirective*)token );
            break;
    }
}

// Parses a scope
void Parser::ParseScope( Scope *scope ) {
    for ( auto it : scope->lines ) {
        Scope *subScope = dynamic_cast<Scope*>( it );
        if ( subScope != nullptr )
            ParseScope( subScope );
        else
            ParseLine( it );
    }
}

// Parse the lex structure and output the file
void Parser::Parse() {
    ParseScope( &lexer->scope );
    output.Build();

    file.write( (const char*)output.buffer(), output.size() );
    file.close();
}

}

#endif
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
    NONE  = 0b00000000000, // None
    IM    = 0b00000000001, // Immediate
    SO    = 0b00000000010, // Stack offset
    SOI   = 0b00000000100, // Stack offset immediate
    SPO   = 0b00000001000, // Stack pointer offset
    SPOI  = 0b00000010000, // Stack pointer offset immediate
    SPIO  = 0b00000100000, // Stack pointer immediate offset
    DO    = 0b00001000000, // Data offset
    DOI   = 0b00010000000, // Data offset immediate
    DPO   = 0b00100000000, // Data offset pointer
    DPOI  = 0b01000000000, // Data pointer offset immediate
    DPIO  = 0b10000000000, // Data pointer immediate offset
    SPIOI = SPOI | SPIO,   // Stack pointer immediate offset immediate
    DPIOI = DPOI | DPIO,   // Data pointer immediate offset immediate
};

const Line
    StackOffsetLine           ( "[bp+x]",       0 ),
    StackOffsetImLine         ( "[bp+42]",      0 ),
    StackPointerOffsetLine    ( "[[bp+x]+y]",   0 ),
    StackPointerOffsetImLine  ( "[[bp+42]+y]",  0 ),
    StackPointerImOffsetLine  ( "[[bp+x]+42]",  0 ),
    StackPointerImOffsetImLine( "[[bp+42]+42]", 0 ),
    DataOffsetLine            ( "[x]",          0 ),
    DataOffsetImLine          ( "[42]",         0 ),
    DataPointerOffsetLine     ( "[[x]+y]",      0 ),
    DataPointerOffsetImLine   ( "[[42]+y]",     0 ),
    DataPointerImOffsetLine   ( "[[x]+42]",     0 ),
    DataPointerImOffsetImLine ( "[[42]+42]",    0 );

struct Info {
    uint8_t baseOpcode;
    uint16_t addressingModes;
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
    { "ldb",  { INS_LDB_IM, IM | SOI | SPIOI | DOI | DPIOI } },
    { "stb",  { INS_STB_SO - 1, SOI | SPIOI | DOI | DPIOI } },
    { "tba",  { INS_TBA, NONE } },
    { "tbx",  { INS_TBX, NONE } },
    { "tby",  { INS_TBY, NONE } },
    { "tbss", { INS_TBSS, NONE } },
    { "tbcs", { INS_TBCS, NONE } },
    { "tbds", { INS_TBDS, NONE } },

    // X-index stuff
    { "ldx",  { INS_LDX_IM, IM | SOI | SPOI | DOI | DPOI } },
    { "stx",  { INS_STX_SO - 1, SOI | SPOI | DOI | DPOI } },
    { "txa",  { INS_TXA, NONE } },
    { "txb",  { INS_TXB, NONE } },
    { "txy",  { INS_TXY, NONE } },
    { "txss", { INS_TXSS, NONE } },
    { "txcs", { INS_TXCS, NONE } },
    { "txds", { INS_TXDS, NONE } },

    // Y-pointer stuff
    { "ldy",  { INS_LDY_IM, IM | SO | SPIO | DO | DPIO } },
    { "sty",  { INS_STY_SO - 1, SO | SPIO | DO | DPIO } },
    { "tya",  { INS_TYA, NONE } },
    { "tyb",  { INS_TYB, NONE } },
    { "tyx",  { INS_TYX, NONE } },
    { "tyss", { INS_TYSS, NONE } },
    { "tycs", { INS_TYCS, NONE } },
    { "tyds", { INS_TYDS, NONE } },

};

// Returns the addressing mode of an instruction given its token
uint16_t GetAddressingMode( token::Instruction *token ) {
    if ( token->subTokens.size() == 0 )               // None
        return NONE;
    if ( token->subTokens.size() > 1 )                // Invalid
        return -1;
    if ( token->subTokens[0]->type == token::NUMBER ) // Immediate
        return IM;
    
    // Bracket land
    if ( token->subTokens[0]->type == token::SEPARATOR ) {
        token::Separator *bracket = (token::Separator*)token->subTokens[0];

        if ( bracket->Equal( StackOffsetLine.tokenStack.top() ) )
            return SO;
        if ( bracket->Equal( StackOffsetImLine.tokenStack.top() ) )
            return SOI;
        if ( bracket->Equal( StackPointerOffsetLine.tokenStack.top() ) )
            return SPO;
        if ( bracket->Equal( StackPointerOffsetImLine.tokenStack.top() ) )
            return SPOI;
        if ( bracket->Equal( StackPointerImOffsetLine.tokenStack.top() ) )
            return SPIO;
        if ( bracket->Equal( StackPointerImOffsetImLine.tokenStack.top() ) )
            return SPIOI;
        if ( bracket->Equal( DataOffsetLine.tokenStack.top() ) )
            return DO;
        if ( bracket->Equal( DataOffsetImLine.tokenStack.top() ) )
            return DOI;
        if ( bracket->Equal( DataPointerOffsetLine.tokenStack.top() ) )
            return DPO;
        if ( bracket->Equal( DataPointerOffsetImLine.tokenStack.top() ) )
            return DPOI;
        if ( bracket->Equal( DataPointerImOffsetLine.tokenStack.top() ) )
            return DPIO;
        if ( bracket->Equal( DataPointerImOffsetImLine.tokenStack.top() ) )
            return DPIOI;
    }
    
    // Nothing valid happened
    return -1;
}

// Most instructions' addressing modes follow this offset pattern
int GetAddressingModeOffset( uint16_t mode ) {
    switch ( mode ) {
        case NONE:
        case IM:    return 0;
        case SO:
        case SOI:   return 1;
        case SPO:
        case SPOI:
        case SPIO:
        case SPIOI: return 2;
        case DO:
        case DOI:   return 3;
        case DPO:
        case DPOI:
        case DPIO:
        case DPIOI: return 4;
    }
    return 0;
}

// Gets an immediate offset [bp+x] or [x]
// Assumes token is valid
// TODO: Make this less bad
uint8_t GetImOffset( token::Instruction *token ) {
    if ( token->subTokens[0]->subTokens[0]->type == token::NUMBER ) {
        return ( (token::Number*)token->subTokens[0]->subTokens[0] )->value;
    }
    return ( (token::Number*)token->subTokens[0]->subTokens[2] )->value;
}

// Gets an immediate offset [[bp+x]+y] or [[x]+y]
// Assumes token is valid
// TODO: Make this less bad
uint8_t GetPointerOffsetIm( token::Instruction *token ) {
    if (
        token->subTokens[0]->subTokens[0]->subTokens[0]->type == token::NUMBER
    ) {
        return ( (token::Number*)token->subTokens[0]->
                    subTokens[0]->subTokens[0] )->value;
    }
    return ( (token::Number*)token->subTokens[0]->
                subTokens[0]->subTokens[2] )->value;
}

// Gets an immediate pointer [[bp+x]+y] or [[x]+y]
// Assumes token is valid
// TODO: Make this less bad
uint16_t GetPointerImOffset( token::Instruction *token ) {
    return ( (token::Number*)token->subTokens[0]->subTokens[2] )->value;
}

}

// A byte storage class
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

    // Virtual destructor
    virtual ~Chunk() = default;

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
    Code *code;

    // Setup basic object
    Object() {
        header = new Header();
        code   = new Code();

        chunks.push_back( header );
        chunks.push_back( code );
    }

    // Free up chunks when we're done building them
    ~Object() {
        for ( auto chunk : chunks )
            delete chunk;
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
    // Each metadata entry is 3 bytes + the 1 end byte
    // This is kinda like a very basic file system to guide the linker
    uint16_t pointer = chunks.size() * PARSER_OBJECT_METADATA_ENTRY_LENGTH + 1;
    for ( Chunk *chunk : chunks ) {
        Append( (uint8_t)chunk->type );
        Append( pointer );

        chunk->Build();
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
    bool error = false;

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

    // Parses line of code
    void ParseCode( token::Instruction *token );
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
    if ( !error ) {
        if ( token->value == ".org" ) {
            token::Number *number = (token::Number*)token->subTokens[0];
            output.header->origin = number->value;
        }
    }
}

// Parses line of code
void Parser::ParseCode( token::Instruction *token ) {
    if ( !error ) {
        // Get instruction info
        ins::Info info;
        try {
            info = ins::instructions.at( token->value );
        } catch (...) {
            std::cout << "ERROR: Unknown instruction \""
                << token->value << "\"\n";
            error = true;
            return;
        }

        // Get and check addressing mode
        uint16_t addressingMode = ins::GetAddressingMode( token );
        if (
            // Don't raise errors if there's no addressing mode
            info.addressingModes != ins::NONE &&
            // Check if the instruction supports the provided addressing mode
            ( !( addressingMode & info.addressingModes ) ||
            // Also check if the provided addressing mode is invalid
            addressingMode == -1 )
        ) {
            std::cout << "ERROR: Invalid addressing mode for instruction \""
                << token->value << "\"\n";
            error = true;
            return;
        }

        // Get opcode
        uint8_t opcode =
            info.baseOpcode + ins::GetAddressingModeOffset( addressingMode );
        
        output.code->Append( opcode );

        // Add immediate values
        if ( ( addressingMode & ( ins::SOI | ins::DOI ) ) )
            output.code->Append( ins::GetImOffset( token ) );
        else if ( ( addressingMode & ( ins::SPOI | ins::DPOI ) ) )
            output.code->Append( ins::GetPointerOffsetIm( token ) );
        if ( ( addressingMode & ( ins::SPIO | ins::DPIO ) ) )
            output.code->Append( ins::GetPointerImOffset( token ) );
    }
}

// Parses a line
void Parser::ParseLine( Line *line ) {
    if ( !error ) {
        token::Token *token = line->tokenStack.top();
        switch ( token->type ) {
            case token::LINKER_DIRECTIVE:
                ParseLinkerDirective( (token::LinkerDirective*)token );
                break;
            case token::INSTRUCTION:
                ParseCode( (token::Instruction*)token );
                break;
        }
    }
}

// Parses a scope
void Parser::ParseScope( Scope *scope ) {
    if ( !error ) {
        for ( auto it : scope->lines ) {
            Scope *subScope = dynamic_cast<Scope*>( it );
            if ( subScope != nullptr )
                ParseScope( subScope );
            else
                ParseLine( it );
        }
    }
}

// Parse the lex structure and output the file
void Parser::Parse() {
    ParseScope( &lexer->scope );
    if ( !error ) {
        output.Build();

        file.write( (const char*)output.buffer(), output.size() );
    }
    else
        std::cout << "\nExited on error.\n\n";

    file.close();
}

}

#endif
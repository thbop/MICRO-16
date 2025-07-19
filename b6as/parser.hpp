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


#include <fstream>
#include <vector>
#include <unordered_map>

#include "stdint.h"

#include "../src/btp6000/Instructions.hpp"
#include "../include/Object.hpp"

#include "settings.hpp"
#include "lexer.hpp"
using namespace lex;

// Parser namespace
namespace parser {

// Instruction namespace
namespace ins {

enum AddressingMode {
    NONE  = 0b0000000000001, // None
    IM8   = 0b0000000000010, // Immediate 8
    IM16  = 0b0000000000100, // Immediate 16
    SO    = 0b0000000001000, // Stack offset
    SOI   = 0b0000000010000, // Stack offset immediate
    SPO   = 0b0000000100000, // Stack pointer offset
    SPOI  = 0b0000001000000, // Stack pointer offset immediate
    SPIO  = 0b0000010000000, // Stack pointer immediate offset
    DO    = 0b0000100000000, // Data offset
    DOI   = 0b0001000000000, // Data offset immediate
    DPO   = 0b0010000000000, // Data offset pointer
    DPOI  = 0b0100000000000, // Data pointer offset immediate
    DPIO  = 0b1000000000000, // Data pointer immediate offset
    SPIOI = SPOI | SPIO,    // Stack pointer immediate offset immediate
    DPIOI = DPOI | DPIO,    // Data pointer immediate offset immediate
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
    // The fundamental "instruction"
    { "db",    { 0, IM8 | IM16 } },

    // Accumulator stuff
    { "lda",   { INS_LDA_IM, IM16 | SOI | SPIOI | DOI | DPIOI } },
    { "sta",   { INS_STA_SO - 1, SOI | SPIOI | DOI | DPIOI } },
    { "tab",   { INS_TAB, NONE } },
    { "tax",   { INS_TAX, NONE } },
    { "tay",   { INS_TAY, NONE } },
    { "tass",  { INS_TASS, NONE } },
    { "tacs",  { INS_TACS, NONE } },
    { "tads",  { INS_TADS, NONE } },

    // Base stuff
    { "ldb",   { INS_LDB_IM, IM16 | SO | SPO | DO | DPO } },
    { "stb",   { INS_STB_SO - 1, SO | SPO | DO | DPO } },
    { "tba",   { INS_TBA, NONE } },
    { "tbx",   { INS_TBX, NONE } },
    { "tby",   { INS_TBY, NONE } },
    { "tbss",  { INS_TBSS, NONE } },
    { "tbcs",  { INS_TBCS, NONE } },
    { "tbds",  { INS_TBDS, NONE } },

    // X-index stuff
    { "ldx",   { INS_LDX_IM, IM16 | SOI | SPOI | DOI | DPOI } },
    { "stx",   { INS_STX_SO - 1, SOI | SPOI | DOI | DPOI } },
    { "txa",   { INS_TXA, NONE } },
    { "txb",   { INS_TXB, NONE } },
    { "txy",   { INS_TXY, NONE } },
    { "txss",  { INS_TXSS, NONE } },
    { "txcs",  { INS_TXCS, NONE } },
    { "txds",  { INS_TXDS, NONE } },

    // Y-pointer stuff
    { "ldy",   { INS_LDY_IM, IM16 | SO | SPIO | DO | DPIO } },
    { "sty",   { INS_STY_SO - 1, SO | SPIO | DO | DPIO } },
    { "tya",   { INS_TYA, NONE } },
    { "tyb",   { INS_TYB, NONE } },
    { "tyx",   { INS_TYX, NONE } },
    { "tyss",  { INS_TYSS, NONE } },
    { "tycs",  { INS_TYCS, NONE } },
    { "tyds",  { INS_TYDS, NONE } },

    // Stack operations
    { "pusha", { INS_PUSHA, NONE } },
    { "popa",  { INS_POPA, NONE } },
    { "pushb", { INS_PUSHB, NONE } },
    { "popb",  { INS_POPB, NONE } },
    { "pushx", { INS_PUSHX, NONE } },
    { "popx",  { INS_POPX, NONE } },
    { "pushy", { INS_PUSHY, NONE } },
    { "popy",  { INS_POPY, NONE } },
    { "enter", { INS_ENTER, NONE } },
    { "leave", { INS_LEAVE, NONE } },
    { "call",  { INS_CALL, IM16 } },
    { "ret",   { INS_RET, NONE } },

    // Misc transfers
    { "tssa",  { INS_TSSA, NONE } },
    { "tcsa",  { INS_TCSA, NONE } },
    { "tdsa",  { INS_TDSA, NONE } },

    // Jump stuff
    { "cmp",   { INS_CMP, NONE | IM16 | SOI | SPIOI | DOI | DPIOI } },
    { "je",    { INS_JE, IM8 } },
    { "jne",   { INS_JNE, IM8 } },
    { "jg",    { INS_JG, IM8 } },
    { "jge",   { INS_JGE, IM8 } },
    { "jmp",   { INS_JMP, IM8 } },
    { "ljmp",  { INS_LJMP, IM16 } },

    // Interrupts
    { "int",   { INS_INT, IM8 } },
    { "rti",   { INS_RTI, NONE } },

    // Arithmetic
    { "add",   { INS_ADD, NONE | IM16 | SOI | SPIOI | DOI | DPIOI } },
    { "sub",   { INS_SUB, NONE | IM16 | SOI | SPIOI | DOI | DPIOI } },
    { "ina",   { INS_INA, NONE } },
    { "dea",   { INS_DEA, NONE } },
    { "inb",   { INS_INB, NONE } },
    { "deb",   { INS_DEB, NONE } },
    { "shr",   { INS_SHR, NONE | IM16 | SOI | SPIOI | DOI | DPIOI } },
    { "shl",   { INS_SHL, NONE | IM16 | SOI | SPIOI | DOI | DPIOI } },
    { "inx",   { INS_INX, NONE } },
    { "dex",   { INS_DEX, NONE } },
    { "iny",   { INS_INY, NONE } },
    { "dey",   { INS_DEY, NONE } },

    // Bit tests
    { "and",   { INS_AND, NONE | IM16 | SOI | SPIOI | DOI | DPIOI } },
    { "or",    { INS_OR,  NONE | IM16 | SOI | SPIOI | DOI | DPIOI } },
    { "xor",   { INS_XOR, NONE | IM16 | SOI | SPIOI | DOI | DPIOI } },
    { "sec",   { INS_SEC, NONE } },
    { "clc",   { INS_CLC, NONE } },
    { "clv",   { INS_CLV, NONE } },

};

// Returns the addressing mode of an instruction given its token
uint16_t GetAddressingMode( token::Instruction *token ) {
    if ( token->subTokens.size() == 0 )                // None
        return NONE;
    if ( token->subTokens.size() > 1 )                  // Invalid
        return -1;
    if ( token->subTokens[0]->type == token::NUMBER ) { // Immediate
        uint16_t number = ( (token::Number*)token->subTokens[0] )->value;
        if ( number > 0xFF )
            return IM16;
        return IM8;
    }
    if ( token->subTokens[0]->type == token::LABEL )    // Label
        return IM8; // The "safe one"
    
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
// TODO: Make this less bad
int GetAddressingModeOffset( uint16_t mode, Info *info ) {
    switch ( mode ) {
        case NONE:  return 0;
        case IM8:
        case IM16:  return ( info->addressingModes & NONE ) ? 1 : 0;
        case SO:
        case SOI:   return ( info->addressingModes & NONE ) ? 2 : 1;
        case SPO:
        case SPOI:
        case SPIO:
        case SPIOI: return ( info->addressingModes & NONE ) ? 3 : 2;
        case DO:
        case DOI:   return ( info->addressingModes & NONE ) ? 4 : 3;
        case DPO:
        case DPOI:
        case DPIO:
        case DPIOI: return ( info->addressingModes & NONE ) ? 5 : 4;
    }
    return 0;
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
        PrintScope( lexer->scopeStack.back(), 0 );
    }
    #endif

private:
    Lexer *lexer;
    obj::Object output;
    std::ofstream file;

    // Debug ------------------------------------------------------------------

    #ifdef DEBUG
    // Prints a scope from the lexer
    void PrintScope( Scope *scope, int indent );

    // Prints a non-scope line from the lexer
    void PrintLine( Line *line, int indent );
    #endif

    // Get immediate values ---------------------------------------------------

    // Gets an immediate value x
    uint16_t GetIm( token::Instruction *token, bool isByte );

    // Gets an immediate offset [bp+x] or [x]
    uint16_t GetOffsetIm( token::Instruction *token );

    // Gets an immediate offset [[bp+x]+y] or [[x]+y]
    uint8_t GetPointerOffsetIm( token::Instruction *token );

    // Gets an immediate pointer [[bp+x]+y] or [[x]+y]
    uint16_t GetPointerImOffset( token::Instruction *token );

    // Sends immediate label data to the appropriate label
    void SendImLabelData( token::Label *token, obj::ImLabelData data );

    // Parsing ----------------------------------------------------------------

    // Parses a scope
    void ParseScope( Scope *scope );

    // Parses a line
    void ParseLine( Line *line );

    // Parses a linker directive
    void ParseLinkerDirective( token::LinkerDirective *token );

    // Parses a label
    void ParseLabel( token::Label *token );

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
        PrintLine( it, indent + 1 );
        Scope *childScope = dynamic_cast<Scope*>(it);
        if ( childScope )
            PrintScope( childScope, indent + 2 );

    }
}
#endif

// Gets an immediate value x
// Assumes token is valid
uint16_t Parser::GetIm( token::Instruction *token, bool isByte ) {
    token::Token *t0 = token->subTokens[0];

    // If immediate is a label, its value to zero and fix it in post
    // (the linker)
    if ( t0->type == token::LABEL ) {
        SendImLabelData(
            (token::Label*)t0,
            {
                (uint8_t)(
                    isByte ?
                    obj::ImLabelData::OFFSET :
                    obj::ImLabelData::ABSOLUTE
                ),
                (uint16_t)output.code->size()
            }
        );
        return 0;
    }

    return ( (token::Number*)t0 )->value;
}

// Gets an immediate offset [bp+x] or [x]
// Assumes token is valid
uint16_t Parser::GetOffsetIm( token::Instruction *token ) {
    token::Token
        *t0 = token->subTokens[0]->subTokens[0], // Number?
        *s0 = NULL;                              // Sign?
    if ( t0->type == token::REGISTER ) {
        t0 = token->subTokens[0]->subTokens[2];
        s0 = token->subTokens[0]->subTokens[1];
    }

    // If immediate is a label, its value to zero and fix it in post
    // (the linker)
    if ( t0->type == token::LABEL ) {
        SendImLabelData(
            (token::Label*)t0,
            { obj::ImLabelData::OFFSET, (uint16_t)output.code->size() }
        );
        return 0;
    }
    if (
        s0 != NULL                   &&
        s0->type == token::SEPARATOR &&
        ( (token::Separator*)s0 )->value == '-'
    ) {
        return -( (token::Number*)t0 )->value;
    }

    return ( (token::Number*)t0 )->value;
}

// Gets an immediate offset [[bp+x]+y] or [[x]+y]
// Assumes token is valid
uint8_t Parser::GetPointerOffsetIm( token::Instruction *token ) {
    return GetOffsetIm( (token::Instruction*)token->subTokens[0] );
}

// Gets an immediate pointer [[bp+x]+y] or [[x]+y]
// Assumes token is valid
uint16_t Parser::GetPointerImOffset( token::Instruction *token ) {
    token::Token
        *t0 = token->subTokens[0]->subTokens[2],
        *s0 = token->subTokens[0]->subTokens[1];

    // If immediate is a label, its value to zero and fix it in post
    // (the linker)
    if ( t0->type == token::LABEL ) {
        SendImLabelData(
            (token::Label*)t0,
            { obj::ImLabelData::ABSOLUTE, (uint16_t)output.code->size() }
        );
        return 0;
    }

    if (
        s0->type == token::SEPARATOR &&
        ( (token::Separator*)s0 )->value == '-'
    )
        return -( (token::Number*)t0 )->value;

    return ( (token::Number*)t0 )->value;
}

// Parses a linker directive
void Parser::ParseLinkerDirective( token::LinkerDirective *token ) {
    token::Number *number =
        dynamic_cast<token::Number*>( token->subTokens[0] );
    
    token::Label *label = dynamic_cast<token::Label*>( token->subTokens[0] );

    if ( !error ) {
        if ( token->value == ".org" && number != nullptr ) {
            output.header->origin = number->value;
        }
        else if ( token->value == "extern" && label != nullptr ) {
            if ( label->value[0] != '.' )
                output.header->labels[label->value]->external = true;
            else
                std::cout << "ERROR: Cannot make sub-label \"" << label->value
                    << "\" external!\n";
        }
        // else if ( token->value == "db" && number != nullptr ) {
        //     if ( number->sizeHint == 1 )
        //         output.code->Append( (uint8_t)number->value );
        //     else
        //         output.code->Append( number->value );
        // }
    }
}

// Sends immediate label data to the appropriate label
void Parser::SendImLabelData( token::Label *token, obj::ImLabelData data ) {
    obj::Label *label =
        output.header->GetLabelFromToken( (token::Label*)token );
    
    label->AppendIm( data );
}

// Parses a label
void Parser::ParseLabel( token::Label *token ) {
    obj::Label *label =
        output.header->GetLabelFromToken( (token::Label*)token );

    // Set the position of the label
    label->position = (uint16_t)output.code->size();
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
        
        // Also check if we need to cast from a byte immediate to a word
        // immediate
        if (
            ( info.addressingModes & ins::IM16 ) &&
            ( addressingMode & ins::IM8 ) &&
            addressingMode != (uint16_t)-1
        )
            addressingMode = ins::IM16;

        if (
            // Don't raise errors if there's no addressing mode
            !( info.addressingModes & ins::NONE ) &&
            // Check if the instruction supports the provided addressing mode
            ( !( addressingMode & info.addressingModes ) ||
            // Also check if the provided addressing mode is invalid
            addressingMode == (uint16_t)-1 )
        ) {
            std::cout << "ERROR: Invalid addressing mode for instruction \""
                << token->value << "\"\n";
            error = true;
            return;
        }

        // Get opcode
        if ( token->value != "db" ) {
            uint8_t opcode =
                info.baseOpcode +
                ins::GetAddressingModeOffset( addressingMode, &info );
            
            output.code->Append( opcode );
        }

        // Add immediate values
        switch ( addressingMode ) {
            case ins::IM8:
                output.code->Append( (uint8_t)GetIm( token, true ) );
                break;
            case ins::IM16:
                output.code->Append( GetIm( token, false ) );
                break;
            case ins::SOI:
            case ins::DOI:
                output.code->Append( (uint8_t)GetOffsetIm( token ) );
                break;
        }
        if ( ( addressingMode & ( ins::SPOI | ins::DPOI ) ) )
            output.code->Append( GetPointerOffsetIm( token ) );
        if ( ( addressingMode & ( ins::SPIO | ins::DPIO ) ) )
            output.code->Append( GetPointerImOffset( token ) );
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
            case token::LABEL:
                ParseLabel( (token::Label*)token );
                break;
        }
    }
}

// Parses a scope
void Parser::ParseScope( Scope *scope ) {
    if ( !error ) {
        for ( auto it : scope->lines ) {
            ParseLine( it );

            Scope *subScope = dynamic_cast<Scope*>( it );
            if ( subScope != nullptr )
                ParseScope( subScope );
        }
    }
}

// Parse the lex structure and output the file
void Parser::Parse() {
    output.header = new obj::Header();
    output.code = new obj::Code();

    ParseScope( lexer->scopeStack.back() );
    if ( !error ) {
        output.AddChunk( output.header );
        output.AddChunk( output.code );

        output.Dump();

        file.write( (const char*)output.buffer(), output.size() );
    }
    else
        std::cout << "\nExited on error.\n\n";

    file.close();
}

}

#endif
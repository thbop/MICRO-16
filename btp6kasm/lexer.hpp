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
#include <cctype>
#include <fstream>
#include <vector>
#include <stack>

#include "stdint.h"

#include "../include/stringextra.hpp"

#include "settings.hpp"

// Lexer namespace
namespace lex {

// Namepsace for all token classes
namespace token {

enum Type {
    UNKNOWN,          // e.g. lda
    NUMBER,           // e.g. 45
    REGISTER,         // e.g. bp
    SEPARATOR,        // e.g. :
    STRING,           // e.g. Hello world!
    INSTRUCTION,      // e.g. lda
    LABEL,            // e.g. main
    STATEMENT,        // e.g. db
    LINKER_DIRECTIVE, // e.g. .org
    MACRO,            // e.g. #define
};

// String listings
// Instructions
std::vector<std::string> instructions = {
    "lda", "sta", "tab", "tax", "tay", "tass", "tacs", "tads",
    "ldb", "stb", "tba", "tbx", "tby", "tbss", "tbcs", "tbds",
    "ldx", "stx", "txa", "txb", "txy", "txss", "txcs", "txds",
    "ldy", "sty", "tya", "tyb", "tyx", "tyss", "tycs", "tyds",
    "pusha", "popa", "pushb", "popb", "pushx", "popx", "pushy", "popy",
    "enter", "leave", "call", "ret",
    "tssb", "tcsb", "tdsb",
    "cmp", "je", "jne", "jg", "jge", "jmp", "ljmp",
    "int", "rti",
};

// Registers
std::vector<std::string> registers = {
    "a", "b", "x", "y",
    "bp",
    "ss", "cs", "ds",
};

// Linker directives
std::vector<std::string> linkerDirectives = {
    ".org",
};

// Separators
std::string separators = "+[]";


// Abstract token class
class Token {
public:
    int type = UNKNOWN;
    std::vector<Token*> subTokens;

    // Clear subTokens
    virtual ~Token() {
        for ( auto it : subTokens )
            delete it;
    }

    // Used by the root token to validate arguments
    // Returns true if valid
    virtual bool Validate( int lineNumber ) {
        bool ok = true;
        for ( auto it : subTokens )
            ok &= it->Validate( lineNumber );
        
        return ok;
    };

    // Checks if a token and its subtokens are equal in type and in value (if
    // specified)
    virtual bool Equal( Token *other, bool inValue=true ) {
        bool equal = true;
        if ( subTokens.size() != other->subTokens.size() )
            return false;
        
        for ( int i = 0; i < (int)subTokens.size(); i++ ) {
            equal &= subTokens[i]->Equal( other->subTokens[i], inValue );
        }

        return equal;
    }

    #ifdef DEBUG
    // Debug print
    virtual void Print( int indent ) {
        for ( auto it : subTokens )
            it->Print( indent + 1 );
    }
    #endif

    // Adds a subToken to the list
    void AddSubToken( Token *subToken ) {
        subTokens.push_back( subToken );
    }

    // Prints a token error
    void PrintError( const char *message, int lineNumber) {
        std::cout << "TOKEN ERROR." << lineNumber << ": " << message << '\n';
    }
};


// Number token class
class Number : public Token {
public:
    uint16_t value;

    // Constructors
    Number() {
        type = NUMBER;
    }
    Number( uint16_t value ) : Number() {
        this->value = value;
    }
    // A number cannot be a validator
    bool Validate( int lineNumber ) override {
        PrintError( "A number cannot be a validator!", lineNumber );
        return false;
    }

    // Checks if a token and its subtokens are equal in type and in value (if
    // specified)
    bool Equal( Token *other, bool inValue=true ) override {
        bool equal = Token::Equal( other, inValue );
        if ( !equal ) return false;
        
        equal &= ( type == other->type );
        if ( !equal ) return false;
        
        // We'll just pretend this doesn't exist
        // if ( inValue )
        //     equal &= ( value == ( (Number*)other )->value );

        return equal;
    }

    #ifdef DEBUG
    // Prints the number and its children
    void Print( int indent ) override {
        for ( int i = 0; i < indent; i++ ) std::cout << "  ";
        std::cout << "Number(" << value << ")\n";
        Token::Print( indent + 1 );
    }
    #endif
};

// String token class
class String : public Token {
public:
    std::string value;

    // Constructors
    String() {
        type = STRING;
    }
    String( std::string &value ) : String() {
        this->value = value;
    }
    // A string cannot be a validator
    bool Validate( int lineNumber ) override {
        PrintError( "A string cannot be a validator!", lineNumber );
        return false;
    }

    // Checks if a token and its subtokens are equal in type and in value (if
    // specified)
    bool Equal( Token *other, bool inValue=true ) override {
        bool equal = Token::Equal( other, inValue );
        if ( !equal ) return false;
        
        equal &= ( type == other->type );
        if ( !equal ) return false;
        
        if ( inValue )
            equal &= ( value == ( (String*)other )->value );

        return equal;
    }

    #ifdef DEBUG
    // Prints the string and its children
    void Print( int indent ) override {
        for ( int i = 0; i < indent; i++ ) std::cout << "  ";
        std::cout << "String(" << value << ")\n";
        Token::Print( indent + 1 );
    }
    #endif
};

// Instruction token class
class Instruction : public Token {
public:
    std::string value;

    // Constructors
    Instruction() {
        type = INSTRUCTION;
    }
    Instruction( std::string &value ) : Instruction() {
        this->value = value;
    }
    // Instruction validator
    bool Validate( int lineNumber ) override {
        if ( subTokens.size() > 1 ) {
            PrintError( "Instruction has too many arguments!", lineNumber );
            return false;
        }
        if (
            subTokens.size() &&
            !( subTokens[0]->type == NUMBER ||
               subTokens[0]->type == SEPARATOR )
        ) {
            PrintError( "Instruction has invalid argument!", lineNumber );
            return false;
        }

        return true;
    }

    // Checks if a token and its subtokens are equal in type and in value (if
    // specified)
    bool Equal( Token *other, bool inValue=true ) override {
        bool equal = Token::Equal( other, inValue );
        if ( !equal ) return false;
        
        equal &= ( type == other->type );
        if ( !equal ) return false;
        
        if ( inValue )
            equal &= ( value == ( (Instruction*)other )->value );

        return equal;
    }

    #ifdef DEBUG
    // Prints the instruction and its children
    void Print( int indent ) override {
        for ( int i = 0; i < indent; i++ ) std::cout << "  ";
        std::cout << "Instruction(" << value << ")\n";
        Token::Print( indent + 1 );
    }
    #endif
};

// Register token class
class Register : public Token {
public:
    std::string value;

    // Constructors
    Register() {
        type = REGISTER;
    }
    Register( std::string &value ) : Register() {
        this->value = value;
    }
    // A register cannot be a validator
    bool Validate( int lineNumber ) override {
        PrintError( "A register cannot be a validator!", lineNumber );
        return false;
    }

    // Checks if a token and its subtokens are equal in type and in value (if
    // specified)
    bool Equal( Token *other, bool inValue=true ) override {
        bool equal = Token::Equal( other, inValue );
        if ( !equal ) return false;
        
        equal &= ( type == other->type );
        if ( !equal ) return false;
        
        if ( inValue )
            equal &= ( value == ( (Register*)other )->value );

        return equal;
    }

    #ifdef DEBUG
    // Prints the register and its children
    void Print( int indent ) override {
        for ( int i = 0; i < indent; i++ ) std::cout << "  ";
        std::cout << "Register(" << value << ")\n";
        Token::Print( indent + 1 );
    }
    #endif
};

// Separator token class
class Separator : public Token {
public:
    char value;

    // Constructors
    Separator() {
        type = SEPARATOR;
    }
    Separator( char value ) : Separator() {
        this->value = value;
    }
    // Validate separator
    bool Validate( int lineNumber ) override {
        if ( value == '[' ) {
            Separator *endBracket = dynamic_cast<Separator*>(
                subTokens[subTokens.size() - 1]
            );
            if ( endBracket == nullptr || endBracket->value != ']' ) {
                PrintError( "Missing end bracket!", lineNumber );
                return false;
            }
        }
        return true;
    }

    // Checks if a token and its subtokens are equal in type and in value (if
    // specified)
    bool Equal( Token *other, bool inValue=true ) override {
        bool equal = Token::Equal( other, inValue );
        if ( !equal ) return false;
        
        equal &= ( type == other->type );
        if ( !equal ) return false;
        
        if ( inValue )
            equal &= ( value == ( (Separator*)other )->value );

        return equal;
    }

    #ifdef DEBUG
    // Prints the separator and its children
    void Print( int indent ) override {
        for ( int i = 0; i < indent; i++ ) std::cout << "  ";
        std::cout << "Separator(" << value << ")\n";
        Token::Print( indent + 1 );
    }
    #endif
};

// LinkerDirective token class
class LinkerDirective : public Token {
public:
    std::string value;

    // Constructors
    LinkerDirective() {
        type = LINKER_DIRECTIVE;
    }
    LinkerDirective( std::string &value ) : LinkerDirective() {
        this->value = value;
    }
    // Instruction validator
    bool Validate( int lineNumber ) override {
        if ( subTokens.size() != 1 && subTokens[0]->type != NUMBER ) {
            PrintError( "Linker directive has invalid argument!", lineNumber );
            return false;
        }

        return true;
    }

    // Checks if a token and its subtokens are equal in type and in value (if
    // specified)
    bool Equal( Token *other, bool inValue=true ) override {
        bool equal = Token::Equal( other, inValue );
        if ( !equal ) return false;
        
        equal &= ( type == other->type );
        if ( !equal ) return false;
        
        if ( inValue )
            equal &= ( value == ( (LinkerDirective*)other )->value );

        return equal;
    }

    #ifdef DEBUG
    // Prints the linker directive and its children
    void Print( int indent ) override {
        for ( int i = 0; i < indent; i++ ) std::cout << "  ";
        std::cout << "LinkerDirective(" << value << ")\n";
        Token::Print( indent + 1 );
    }
    #endif
};


}

// A single lexed line
class Line {
public:
    int number; // line number
    std::string rawLine;
    std::stack<token::Token*> tokenStack;
    
    // Default constructor
    Line() {}

    // Constructor that lexes automatically
    Line( const std::string &line, int lineNumber ) {
        rawLine = line;
        number = lineNumber;

        Lex();
    }

    // Free the master token
    virtual ~Line() {
        if ( tokenStack.size() > 1 )
            std::cout << "DEBUG ERROR: Something failed with the token "
                "stack! Missing bracket?\n";
        else if ( tokenStack.size() == 1 )
            delete tokenStack.top();
    }


private:
    // Lexes a line
    void Lex();

    // Creates a new token given the raw token
    token::Token *NewToken( std::string &rawToken );

    // Evaluates a raw token string and adds a new token
    token::Token *AddToken( std::string &rawToken );
};

// Creates a new token given the raw token
token::Token *Line::NewToken( std::string &rawToken ) {
    rawToken = stringextra::tolower( rawToken );

    // Instruction ------------------------------------------------------------
    if (
        stringextra::find_str_in_list( rawToken, token::instructions ) != -1
    )
        return new token::Instruction( rawToken );
    // Register ---------------------------------------------------------------
    if ( stringextra::find_str_in_list( rawToken, token::registers ) != -1 )
        return new token::Register( rawToken );
    // Linker Directive -------------------------------------------------------
    if (
        stringextra::find_str_in_list(
            rawToken, token::linkerDirectives
        ) != -1
    )
        return new token::LinkerDirective( rawToken );
    // Number -----------------------------------------------------------------
    if ( stringextra::isint( rawToken ) )
        return new token::Number( stringextra::str_to_int( rawToken ) );
    // Separator --------------------------------------------------------------
    if (
        rawToken.size() == 1 &&
        token::separators.find( rawToken[0] ) != std::string::npos
    )
        return new token::Separator( rawToken[0] );
    
    std::cout << "Unknown: " << rawToken << "\n";

    // Unknown
    return new token::Token();
}

// Evaluates a raw token string and adds a new token
token::Token *Line::AddToken( std::string &rawToken ) {
    if ( rawToken == "" ) {
        return nullptr;
    }

    token::Token *token = NewToken( rawToken );

    // If the master token does not exist, create it
    if ( tokenStack.size() == 0 ) 
        tokenStack.push( token );
    // Otherwise, add to the master token
    else
        tokenStack.top()->AddSubToken( token );
    
    return token;
}

// Lexes a line
void Line::Lex() {
    std::string rawToken;

    for ( int i = 0; i < (int)rawLine.size(); i++ ) {
        char character = rawLine[i];
        // If space -----------------------------------------------------------
        if ( std::isspace( character ) ) {
            AddToken( rawToken );
            rawToken = std::string();
            continue;
        // If separator -------------------------------------------------------
        } else if (
            token::separators.find( character ) != std::string::npos
        ) {
            AddToken( rawToken );
            rawToken = std::string() + character;
            token::Token *separatorToken =
                AddToken( rawToken );

            switch ( character ) {
                case '[': tokenStack.push( separatorToken ); break;
                case ']': tokenStack.pop(); break;
            }
            
            rawToken = std::string();
        // If end -------------------------------------------------------------
        } else if ( i == (int)rawLine.size() - 1 ) {
            rawToken += character;
            AddToken( rawToken );
            rawToken = std::string();
            continue;
        }
        else
            rawToken += character;
    }

    #ifdef DEBUG
    if ( tokenStack.size() != 1 )
        std::cout << "DEBUG ERROR: Something failed with the token stack! "
            "Missing bracket?\n";
    else
    #endif
        tokenStack.top()->Validate( number );
}

// A container of Lines that defines a scope
class Scope : public Line {
public:
    std::vector<Line*> lines; // or scopes

    // Constructors
    Scope() {}
    
    // Inherit Line constructor
    Scope( std::string line, int lineNumber ) : Line( line, lineNumber ) {}

    // Cleanup
    ~Scope() {
        for ( auto it : lines )
            delete it;
    }

    // Allocate and add a line to the list
    void Add( Line *line ) {
        // TODO: Probably something to test if the line is really a scope
        lines.push_back( line );
    }
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
        if ( file.is_open() )
            file.close();
    }

private:
    std::ifstream file;
};

// Begin lexing
void Lexer::Evaluate() {
    std::string line;
    int lineNumber = 0;
    while ( std::getline( file, line ) ) {
        lineNumber++;

        line = stringextra::strip( line );
        if ( !line.empty() )
            scope.Add( new Line( line, lineNumber ) );
    }

    file.close();
}

}

#endif
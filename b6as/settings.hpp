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


#ifndef SETTINGS_H
#define SETTINGS_H

#include <iostream>
#include <string>

const char settingsHelpMessage[] =
"BetterThanPico 6000 Assembler\n"
"Arguments:\n"
"    -h - Help message\n"
"    -o - Output file\n"
"    default - Input file\n"
"\n\nExample:\n"
"    b6as test.asm -o test.o\n";

// Parses and stores assembler settings from command line arguments
class Settings {
public:
    std::string inputFile;
    std::string outputFile;

    Settings() {}

    // Parses the arguments
    bool Parse( int argc, char **argv );

private:
    enum {
        NEXT_ARG_INPUT,
        NEXT_ARG_OUTPUT,
    };

    // Sets the next argument
    // For example, if the last argument was -o then the next argument would be
    // NEXT_ARG_OUTPUT
    bool SetNextArgument( char *arg, int &nextArgument );
};

// Sets the next argument
// For example, if the last argument was -o then the next argument would be
// NEXT_ARG_OUTPUT
bool Settings::SetNextArgument( char *arg, int &nextArgument ) {
    std::string sArg = arg;

    if ( sArg == "-o" ) {
        nextArgument = NEXT_ARG_OUTPUT;
        return true;
    } else if ( sArg == "-h" ) {
        std::cout << settingsHelpMessage;
        return false;
    } else if ( sArg[0] == '-' ) {
        std::cout << "Argument \"" << sArg << "\" does not exist!\n";
        return false;
    } else {
        nextArgument = NEXT_ARG_INPUT;
        return true;
    }
}

// Parses the arguments
bool Settings::Parse( int argc, char **argv ) {
    if ( argc < 4 ) {
        std::cout << settingsHelpMessage;
        return false;
    }

    int nextArgument = NEXT_ARG_INPUT;
    int wait = 0;

    for ( int i = 0; i < argc; i++ ) {
        if ( wait > 0 ) {
            switch ( nextArgument ) {
                case NEXT_ARG_INPUT:
                    inputFile = argv[i];
                    wait = 0;
                    break;
                case NEXT_ARG_OUTPUT:
                    outputFile = argv[i];
                    wait = 0;
                    break;
            }
        }
        else {
            if ( !SetNextArgument( argv[i], nextArgument ) )
                return false;
            wait++;
        }
    }

    return true;
}

#endif
#ifndef SETTINGS_H
#define SETTINGS_H

#include <iostream>
#include <string>

const char settingsHelpMessage[] =
"Better Than Pico 6000 Assembler\n"
"Arguments:\n"
"    -h - Help message\n"
"    -o - Output file\n"
"    default - Fnput file\n"
"\n\nExample:\n"
"    btp6kasm test.asm -o test.o\n";

// Parses and stores assembler settings from command line arguments
class Settings {
    std::string inputFile;
    std::string outputFile;

public:
    Settings() {}

    // Parses the arguments
    bool Parse( int argc, char **args );

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
bool Settings::Parse( int argc, char **args ) {
    if ( argc < 4 ) {
        std::cout << settingsHelpMessage;
        return false;
    }

    int nextArgument = NEXT_ARG_INPUT;
    int wait = 0;

    for ( int i = 0; i < argc; i++ ) {
        if ( !SetNextArgument( args[i], nextArgument ) )
            return false;
        
        if ( wait > 0 ) {
            switch ( nextArgument ) {
                case NEXT_ARG_INPUT:
                    inputFile = args[i];
                    wait = 0;
                    break;
                case NEXT_ARG_OUTPUT:
                    outputFile = args[i];
                    wait = 0;
                    break;
            }
        }
        else
            wait++;
    }

    return true;
}

#endif
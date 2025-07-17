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
#include <vector>

const char settingsHelpMessage[] =
"\n"
"BetterThanPico 6000 Linker\n\n"
"Options:\n"
"    none      - Input object files\n"
"    -h        - Help message\n"
"    -o        - Output file\n"
"    -nostdlib - Do not include the standard library\n\n"
"Example:\n"
"    b6ld test.o -o test.cart\n"
"\n";

// Parses and stores linker settings from command line arguments
class Settings {
public:
    std::vector<std::string> inputFiles;
    std::string outputFile;

    bool includeStandardLibrary = true;

    Settings() {}

    // Parses the arguments
    bool Parse( int argc, char **argv );

private:
    enum {
        NONE,
        EXPECT_OUTPUT_FILE,
    };

    // Parses options (e.g. -o) and returns the proper expect
    // If something went wrong, returns -1
    int ParseOptions( const std::string option );

    // Includes the stdlib as input file arguments
    // Returns true if successful
    bool IncludeStandardLibrary( std::string ldLocation );
};

// Parses options (e.g. -o) and returns the proper expect
// If something went wrong, returns -1
int Settings::ParseOptions( const std::string option ) {
    if ( option == "-o" )
        return EXPECT_OUTPUT_FILE;
    if ( option == "-nostdlib" ) {
        includeStandardLibrary = false;
        return NONE;
    }
    if ( option == "-h" ) {
        std::cout << settingsHelpMessage;
        return -1;
    }

    std::cout << "Invalid option \"" << option << "\"!\n";
    std::cout << settingsHelpMessage;
    return -1;
}

// Includes the stdlib as input file arguments
// Returns true if successful
bool Settings::IncludeStandardLibrary( std::string ldLocation ) {
    // Find last slash
    int slash = -1;

    for ( int i = (int)ldLocation.size() - 1; i > -1; i-- ) {
        if ( ldLocation[i] == '/' || ldLocation[i] == '\\' ) {
            slash = i;
            break;
        }
    }

    if ( slash == -1 ) {
        std::cout << "Something went wrong when including standard library!\n";
        return false;
    }

    // Standard Library Path
    std::string stdlibPath = ldLocation.substr( 0, slash ) + "/stdlib";

    // Include stuff now
    inputFiles.push_back( stdlibPath + "/interrupts.o" );

    return true;
}

// Parses the arguments
// Returns true on success
bool Settings::Parse( int argc, char **argv ) {
    if ( argc < 4 ) {
        std::cout << settingsHelpMessage;
        return false;
    }

    int expect = NONE;

    for ( int i = 1; i < argc; i++ ) {
        switch ( expect ) {
            case NONE:
                if ( argv[i][0] == '-' ) {
                    expect = ParseOptions( argv[i] );
                    if ( expect == -1 ) // Something went wrong
                        return false;
                }
                else
                    inputFiles.push_back( argv[i] );
                break;
            case EXPECT_OUTPUT_FILE:
                outputFile = argv[i];
                expect = NONE;
                break;
        }
    }

    if ( includeStandardLibrary )
        if ( !IncludeStandardLibrary( argv[0] ) )
            return false;
    
    return true;
}

#endif
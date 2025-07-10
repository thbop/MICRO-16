/*
* Copyright © 2025 Thbop
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

#ifndef STRINGEXTRA_HPP
#define STRINGEXTRA_HPP

#include <string>
#include <algorithm>
#include <cctype>

// String Extra namespace
namespace stringextra {

// Strips off leading and trailing whitespaces/newlines
std::string strip( const std::string &str ) {
    auto start = std::find_if_not( str.begin(), str.end(), ::isspace );
    auto end = std::find_if_not( str.rbegin(), str.rend(), ::isspace ).base();

    return ( start < end ) ? std::string( start, end ) : std::string();

}

// Counts the number of instances of a certain character
int count_char( const std::string &str, char character ) {
    int count = 0;
    for ( char c : str ) {
        count += ( character == c );
    }
    return count;
}

// Returns true if the string contains only a base10 integer
bool isint10( const std::string &str ) {
    if ( str == "" ) return false;

    for ( char c : str )
        if ( !std::isdigit( c ) )
            return false;

    return true;
}

// Returns true if the string contains only a base16 integer
// No headers like: 0xBEEF or DEADh
bool isint16( const std::string &str ) {
    if ( str == "" ) return false;

    for ( char c : str )
        if ( !std::isxdigit( c ) )
            return false;

    return true;
}

// Strips strings like "0xDEAD" to "DEAD" or "BEEFh" to "BEEF"
std::string strip_int16_headers( const std::string &str ) {
    if ( str.substr(0, 2) == "0x" ) {
        return std::string( str.begin() + 2, str.end() );
    } else if ( str[str.size() - 1] == 'h' ) {
        return std::string( str.begin(), str.end() - 1 );
    }
    else return std::string();
}

// Returns true if the string contains only a base16 integer with appropriate
// hex headers like: 0xBEEF or DEADh
bool isint16_h( const std::string &str ) {
    return isint16( strip_int16_headers( str ) );
}

// If str is a valid hex (with headers) or base10 integer
bool isint( const std::string &str ) {
    return isint10( str ) || isint16_h( str );
}

// Accepts base10 and base16
int str_to_int( const std::string &str ) {
    if ( isint16_h( str ) )
        return std::stoi(
            strip_int16_headers( str ),
            nullptr, 16
        );
    return std::stoi( str );
}

// Replaces all uppercase letters with lowercase ones
std::string tolower( std::string str ) {
    for ( char &c : str )
        c = std::tolower( c );
    return str;
}

// Finds a string in a list of char buffers
// Returns the index or -1 if not found
int find_str_in_list( const std::string &str, std::vector<std::string> &list ) {
    for ( int i = 0; i < (int)list.size(); i++ )
        if ( str == list[i] )
            return i;
    return -1;
}


}

#endif
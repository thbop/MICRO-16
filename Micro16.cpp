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

#include <iostream>
#include <iomanip>

#include "bob3000/Bob.hpp"

int main() {
    Bob3k bob;
    
    // Test endianness
    bob.Write16( 0x200, 0x000, 0xBEEF );
    std::cout << thex::Byte << (int)bob.Read( 0x200, 0x000 ) << '\n';
    std::cout << thex::Word << (int)bob.Read16( 0x200, 0x000 ) << '\n';

    // Test if segments address properly
    bob.Write( 0xFFF, 0x00F, 0xFF );
    std::cout << thex::Byte << (int)bob.Read( 0xFF0, 0x0FF );

    return 0;
}
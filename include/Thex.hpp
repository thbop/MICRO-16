#ifndef THEX_HPP
#define THEX_HPP

#include <iostream>
#include <iomanip>


// Thbop Hex Printer
// Quick little namespace for returning printable streams of bytes and words
namespace thex {
    // Byte
    std::ostream& Byte( std::ostream &os ) {
        return os << std::uppercase << std::hex << std::setw( 2 )
            << std::setfill('0');
    }

    // Word
    std::ostream& Word( std::ostream &os ) {
        return os << std::uppercase << std::hex << std::setw( 4 )
            << std::setfill('0');
    }
}

#endif
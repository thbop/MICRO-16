#ifndef BTP_HPP
#define BTP_HPP

#include "stdint.h"

// The Better Than Pico 6000 namespace
namespace btp {
    // General purpose register
    union Register {
        uint16_t value;
        struct {
            uint8_t low, high;
        };
    };

    // CPU class
    class BetterThanPico {
    public:
        // General purpose registers
        //     Accumulator
        //     Base register
        //     X-index register
        Register A, B, X;

        // Stack registers
        //     Stack pointer
        //     Base pointer
        uint16_t SP, BP;

        // Segment registers
        //     Stack segment
        //     Code segment
        //     Data segment
        uint16_t SS, CS, DS;

        // Resets the CPU
        BetterThanPico() {
            // Set everything to zero
            A.value = B.value = X.value = 
            SP = BP = SS = CS = DS = 0;
        }
    };

}

#endif
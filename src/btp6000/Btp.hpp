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

#ifndef BTP_HPP
#define BTP_HPP

#include "stdint.h"

#include "../bob3000/Bob.hpp"

// The Better Than Pico 6000 namespace
namespace btp {
    #include "Instructions.hpp"


    // General purpose register
    union Register {
        uint16_t value;
        struct {
            uint8_t low, high;
        };
    };

    // Flags
    union Flag {
        uint8_t value;
        struct {
            uint8_t
                C : 1, // Carry flag
                Z : 1, // Zero flag
                V : 1, // Overflow flag
                N : 1; // Negative flag
        };
    };

    // CPU class
    class BetterThanPico {
    public:
        // General purpose registers
        //     Accumulator
        //     Base register
        Register A, B;

        // Index registers
        //     X-index register
        //     Y-pointer register
        uint16_t X, Y;

        // Pointer registers
        //     Instruction pointer
        //     Stack pointer
        //     Base pointer
        uint16_t IP, SP, BP;

        // Segment registers
        //     Stack segment
        //     Code segment
        //     Data segment
        uint16_t SS, CS, DS;

        // Default constructor
        BetterThanPico() {}

        // Sets up the memory
        void SetMemory( Bob3k *memory ) {
            this->memory = memory;
        }

        void Reset() {
            // Set everything to zero
            A.value = B.value = X = Y = 
            IP = SP = BP = SS = CS = DS = 0;
        }

        // Executes one instruction
        void Execute();

    private:
        Bob3k *memory;

        // Calculates an address from a segment and an offset
        // Similar to x86 memory segmentation:
        //     0xFFF segments
        //     Each segment is 0xFFF bytes wide
        //     Segments are separated by 0x10 bytes
        //
        // For example:
        //     FFF:00F = FFFF
        //     FF0:0FF = FFFF
        //     FFF:FFF = 0FEF
        uint16_t CalculateAddress( uint16_t segment, uint16_t offset ) {
            return ( segment << 4 ) + ( offset & 0xFFF );
        }

        // Read a byte given a segment and an offset
        uint8_t Read( uint16_t segment, uint16_t offset ) {
            return memory->Read( CalculateAddress( segment, offset ) );
        }

        // Read a word given a segment and an offset
        uint16_t Read16( uint16_t segment, uint16_t offset ) {
            return memory->Read16( CalculateAddress( segment, offset ) );
        }

        // Write a byte given a segment and an offset
        void Write( uint16_t segment, uint16_t offset, uint8_t value ) {
            memory->Write( CalculateAddress( segment, offset ), value );
        }

        // Write a word given a segment and an offset
        void Write16( uint16_t segment, uint16_t offset, uint16_t value ) {
            memory->Write16( CalculateAddress( segment, offset ), value );
        }

        // Fetches a byte from CS:IP and increments IP
        uint8_t Fetch() {
            uint8_t value = Read( CS, IP );
            IP++;
            return value;
        }

        // Fetches a word from CS:IP and increments IP
        uint16_t Fetch16() {
            uint16_t value = Read16( CS, IP );
            IP += 2;
            return value;
        }
    };

}

#endif
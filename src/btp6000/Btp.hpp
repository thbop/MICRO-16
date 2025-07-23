/*
* Copyright © 2025 Micro-16 Team
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the “Software”), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
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

// #define BTP_DEBUG

#define BTP_INTERRUPT_COUNT 16

// Evil little macro to make repeat arithmetic statements easier
// Args
//     result = first argument and the result of the operation
//     value  = second argument
//     op     = operation (e.g. +, -, etc)
#define BTP_MATH_OP( result, value, op ) {\
    uint16_t v = value;\
    uint32_t r = result op v;\
    MathFlagSet( result, v, r );\
    result = r; \
}

#include <fstream>

#include "stdio.h"
#include "stdint.h"

#include "../bob3000/Bob.hpp"

// The Better Than Pico 6000 namespace
namespace btp {
    #include "Instructions.hpp"

    // Flags
    union Flags {
        uint8_t value;
        struct {
            uint8_t
                C : 1, // Carry flag
                Z : 1, // Zero flag
                V : 1, // Overflow flag
                N : 1, // Negative flag
                I : 1; // Interrupt flag
        };
    };

    // CPU class
    class BetterThanPico {
    public:
        // General purpose registers
        //     Accumulator
        //     Base register
        uint16_t A, B;

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

        // Flags
        Flags flags;

        // Default constructor
        BetterThanPico() {}

        // Sets up the memory
        void SetMemory( Bob3k *memory ) {
            this->memory = memory;
        }

        void Reset() {
            // Set everything to zero
            A = B = X = Y = 
            IP = SP = BP = SS = CS = DS = 
            flags.value = 0;
        }

        // Executes one instruction
        void Execute();

        #ifdef BTP_DEBUG
        // Dumps all the memory to a file
        void DumpMemory( const char *outputFile );

        #endif

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
            return ( segment << 4 ) + offset;
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

        // Most AB register operations will run this generic function
        void GenericFlagSet( uint16_t value ) {
            flags.Z = ( value == 0 );
            flags.N = ( value >> 15 );
        }

        // Sets frags for arithmetic
        void MathFlagSet( uint16_t value0, uint16_t value1, uint32_t result ) {
            flags.C = ( result > 0xFFFF );

            // Borrowed from this abomination:
            // https://github.com/bfirsh/jsnes/blob/master/src/cpu.js#L293
            flags.V =
                !( ( value0 ^ value1 ) & 0x8000 ) &&
                ( ( value0 ^ result ) & 0x8000 );

            GenericFlagSet( result );
        }

        // Sets flags appropriately for a CMP instruction
        void Compare( uint16_t a, uint16_t b ) {
            flags.Z = a == b;
            flags.C = a > b;
            flags.N = a >= b;
        }

        // Load Immediate value
        uint16_t LoadImmediate() {
            uint16_t value = Fetch16();
            GenericFlagSet( value );
            return value;
        }

        // Load offset value
        uint16_t LoadOffset( uint16_t segment, uint16_t offset ) {
            uint16_t value = Read16( segment, offset );
            GenericFlagSet( value );
            return value;
        }

        // Load offset with immediate
        uint16_t LoadOffsetIm( uint16_t segment, uint16_t offset ) {
            return LoadOffset( segment, offset + Fetch() );
        }

        // Load pointer offset value
        uint16_t LoadPointerOffset(
            uint16_t segment,
            uint16_t offset,
            uint16_t pointer
        ) {
            uint16_t address = Read16( segment, offset );
            uint16_t value = Read16( SS, address + pointer );
            GenericFlagSet( value );
            return value;
        }

        // Load pointer offset value with an immediate offset
        uint16_t LoadPointerOffsetIm(
            uint16_t segment,
            uint16_t offset,
            uint16_t pointer
        ) {
            return LoadPointerOffset( segment, offset + Fetch(), pointer );
        }

        // Load pointer offset value with an immediate pointer
        uint16_t LoadPointerImOffset(
            uint16_t segment,
            uint16_t offset,
            uint16_t pointer
        ) {
            return LoadPointerOffset( segment, offset, pointer + Fetch16() );
        }

        // Load pointer offset value with an immediate offset and pointer
        uint16_t LoadPointerImOffsetIm(
            uint16_t segment,
            uint16_t offset,
            uint16_t pointer
        ) {
            return LoadPointerOffset(
                segment,
                offset + Fetch(),
                pointer + Fetch16()
            );
        }

        // Store offset value in memory
        void StoreOffset( uint16_t segment, uint16_t offset, uint16_t value ) {
            Write16( segment, offset, value );
        }

        // Store offset value in memory with an immediate offset
        void StoreOffsetIm(
            uint16_t segment,
            uint16_t offset,
            uint16_t value
        ) {
            StoreOffset( segment, offset + Fetch(), value );
        }

        // Store pointer offset value in memory
        void StorePointerOffset(
            uint16_t segment,
            uint16_t offset,
            uint16_t pointer,
            uint16_t value
        ) {
            uint16_t address = Read16( segment, offset );
            Write16( SS, address + pointer, value );
        }

        // Store pointer offset value in memory with an immediate offset
        void StorePointerOffsetIm(
            uint16_t segment,
            uint16_t offset,
            uint16_t pointer,
            uint16_t value
        ) {
            StorePointerOffset( segment, offset + Fetch(), pointer, value );
        }

        // Store pointer offset value in memory with an immediate pointer
        void StorePointerImOffset(
            uint16_t segment,
            uint16_t offset,
            uint16_t pointer,
            uint16_t value
        ) {
            StorePointerOffset( segment, offset, pointer + Fetch16(), value );
        }

        // Store pointer offset value in memory with an immediate offset and
        // pointer
        void StorePointerImOffsetIm(
            uint16_t segment,
            uint16_t offset,
            uint16_t pointer,
            uint16_t value
        ) {
            StorePointerOffset(
                segment,
                offset + Fetch(),
                pointer + Fetch16(),
                value
            );
        }

        // Pushes a byte onto the stack
        void Push( uint8_t value ) {
            Write( SS, --SP, value );
        }

        // Pushes a word onto the stack
        void Push16( uint16_t value ) {
            SP -= 2;
            Write16( SS, SP, value );
        }

        // Pops a byte off the stack
        uint8_t Pop() {
            return Read( SS, SP++ );
        }

        // Pop a word off the stack
        uint16_t Pop16() {
            uint16_t value = Read16( SS, SP );
            SP += 2;
            return value;
        }

        // Control flow + jumps
        // Adds a signed immediate byte to the instruction pointer
        void Jump() {
            IP += (int8_t)Fetch();
        }

        // Jumps if the given condition is true
        void JumpCondition( uint8_t condition ) {
            if ( condition )
                Jump();
            else
                IP++; // Otherwise, skip past the argument
        }

        // Sets the instruction pointer to the given word
        void LongJump() {
            IP = Fetch16();
        }

        // Calls the appropriate interrupt
        void Interrupt( uint8_t id ) {
            if ( id < BTP_INTERRUPT_COUNT ) {
                // Save current flags and IP
                Push( flags.value );
                Push16( CS );
                Push16( IP );

                // Set interrupt flag and jump
                flags.I = 1;
                CS = 0x0000;
                IP = Read16( 0, id << 1 );
            }
        }

        // Returns from an interrupt
        void ReturnFromInterrupt() {
            IP = Pop16();
            CS = Pop16();
            flags.value = Pop();
        }
    };

}

#endif
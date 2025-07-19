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

#ifndef INSTRUCTIONS_HPP
#define INSTRUCTIONS_HPP

enum Instructions {
    // LDA - Load Accumulator
    INS_LDA_IM  = 0xA0, // Immediate
    INS_LDA_SO  = 0xA1, // Stack offset [SS:BP+im] (im = immediate)
    INS_LDA_SPO = 0xA2, // Stack pointer offset [[SS:BP+im]+im]
    INS_LDA_DO  = 0xA3, // Data offset [DS:im]
    INS_LDA_DPO = 0xA4, // Data pointer offset [[DS:im]+im]

    // STA - Store Accumulator
    INS_STA_SO  = 0xA5, // [SS:BP+im]      = A
    INS_STA_SPO = 0xA6, // [[SS:BP+im]+im] = A
    INS_STA_DO  = 0xA7, // [DS:im]         = A
    INS_STA_DPO = 0xA8, // [[DS:im]+im]    = A

    // TA-X - Transfer Accumulator to X
    INS_TAB     = 0xA9, // B  = A
    INS_TAX     = 0xAA, // X  = A
    INS_TAY     = 0xAB, // Y  = A
    INS_TASS    = 0xAC, // SS = A
    INS_TACS    = 0xAD, // CS = A
    INS_TADS    = 0xAE, // DS = A

    // LDB - Load Base
    INS_LDB_IM  = 0xB0, // B = Im
    INS_LDB_SO  = 0xB1, // B = [SS:BP+X]
    INS_LDB_SPO = 0xB2, // B = [[SS:BP+X]+Y]
    INS_LDB_DO  = 0xB3, // B = [DS:X]
    INS_LDB_DPO = 0xB4, // B = [[DS:X]+Y]

    // STB - Store Base
    INS_STB_SO  = 0xB5, // [SS:BP+X]     = B
    INS_STB_SPO = 0xB6, // [[SS:BP+X]+Y] = B
    INS_STB_DO  = 0xB7, // [DS:X]        = B
    INS_STB_DPO = 0xB8, // [[DS:X]+Y]    = B

    // TB-X - Transfer Base to X
    INS_TBA     = 0xB9, // A  = B
    INS_TBX     = 0xBA, // X  = B
    INS_TBY     = 0xBB, // Y  = B
    INS_TBSS    = 0xBC, // SS = B
    INS_TBCS    = 0xBD, // CS = B
    INS_TBDS    = 0xBE, // DS = B

    // LDX - Load X-index
    INS_LDX_IM  = 0x80, // Immediate
    INS_LDX_SO  = 0x81, // X = [SS:BP+im]
    INS_LDX_SPO = 0x82, // X = [[SS:BP+im]+Y]
    INS_LDX_DO  = 0x83, // X = [DS:im]
    INS_LDX_DPO = 0x84, // X = [[DS:im]+Y]

    // STX - Store X-index
    INS_STX_SO  = 0x85, // [SS:BP+im]     = X
    INS_STX_SPO = 0x86, // [[SS:BP+im]+Y] = X
    INS_STX_DO  = 0x87, // [DS:im]        = X
    INS_STX_DPO = 0x88, // [[DS:im]+Y]    = X

    // TX-X - Transfer X-index to X
    INS_TXA     = 0x89, // A  = X
    INS_TXB     = 0x8A, // B  = X
    INS_TXY     = 0x8B, // Y  = X
    INS_TXSS    = 0x8C, // SS = X
    INS_TXCS    = 0x8D, // CS = X
    INS_TXDS    = 0x8E, // DS = X

    // LDY - Load Y-pointer
    INS_LDY_IM  = 0x90, // Y = Im
    INS_LDY_SO  = 0x91, // Y = [SS:BP+X]
    INS_LDY_SPO = 0x92, // Y = [[SS:BP+X]+im]
    INS_LDY_DO  = 0x93, // Y = [DS:X]
    INS_LDY_DPO = 0x94, // Y = [[DS:X]+im]

    // STY - Store Y-pointer
    INS_STY_SO  = 0x95, // [SS:BP+X]      = Y
    INS_STY_SPO = 0x96, // [[SS:BP+X]+im] = Y
    INS_STY_DO  = 0x97, // [DS:X]         = Y
    INS_STY_DPO = 0x98, // [[DS:X]+im]    = Y

    // TY-X - Transfer Y-pointer to X
    INS_TYA     = 0x99, // A  = Y
    INS_TYB     = 0x9A, // B  = Y
    INS_TYX     = 0x9B, // X  = Y
    INS_TYSS    = 0x9C, // SS = Y
    INS_TYCS    = 0x9D, // CS = Y
    INS_TYDS    = 0x9E, // DS = Y

    // Stack operations
    INS_PUSHA   = 0x50, // Push A (not all)
    INS_POPA    = 0x51, // Pop A (not all)
    INS_PUSHB   = 0x52, // Push B
    INS_POPB    = 0x53, // Pop B
    INS_PUSHX   = 0x54, // Push X
    INS_POPX    = 0x55, // Pop X
    INS_PUSHY   = 0x56, // Push Y
    INS_POPY    = 0x57, // Pop Y
    INS_ENTER   = 0x58, // Enter stack frame ( push bp, bp = sp )
    INS_LEAVE   = 0x59, // Leave stack frame ( sp = bp, pop bp )
    INS_CALL    = 0x5A, // Call a function (push IP and long jump to address)
    INS_RET     = 0x5B, // Return from a function ( pop IP )

    // Misc transfers
    INS_TSSA    = 0x5C, // A = SS
    INS_TCSA    = 0x5D, // A = CS
    INS_TDSA    = 0x5E, // A = DS

    // Control flow
    INS_CMP     = 0xC0, // Compare A with B
    INS_CMP_IM  = 0xC1, // Compare A with im
    INS_CMP_SO  = 0xC2, // Compare A with [SS:BP+im]
    INS_CMP_SPO = 0xC3, // Compare A with [[SS:BP+im]+im]
    INS_CMP_DO  = 0xC4, // Compare A with [DS:im]
    INS_CMP_DPO = 0xC5, // Compare A with [[DS:im]+im]
    INS_JE      = 0xC6, // Jump equal,         if ZF == 1
    INS_JNE     = 0xC7, // Jump not equal,     if ZF == 0
    INS_JG      = 0xC8, // Jump greater,       if CF == 1
    INS_JGE     = 0xC9, // Jump greater equal, if NF == 1
    INS_JMP     = 0xCA, // Short jump,         increments IP
    INS_LJMP    = 0xCB, // Long jump,          sets IP

    // Interrupts
    INS_INT     = 0x00, // Interrupt 
                        //     push flags, CS, and IP, then reference the
                        //     interrupt jump table, and jump to the interrupt
                        //     handler
    INS_RTI     = 0x01, // Return from interrupt (pop IP, CS, and flags)

    // Arithmetic
    INS_ADD     = 0xD0, // Add A to B
    INS_ADD_IM  = 0xD1, // Add A to im
    INS_ADD_SO  = 0xD2, // Add A to [SS:BP+im]
    INS_ADD_SPO = 0xD3, // Add A to [[SS:BP+im]+im]
    INS_ADD_DO  = 0xD4, // Add A to [DS:im]
    INS_ADD_DPO = 0xD5, // Add A to [[DS:im]+im]

    INS_SUB     = 0xD6, // Subtract A by B
    INS_SUB_IM  = 0xD7, // Subtract A by im
    INS_SUB_SO  = 0xD8, // Subtract A by [SS:BP+im]
    INS_SUB_SPO = 0xD9, // Subtract A by [[SS:BP+im]+im]
    INS_SUB_DO  = 0xDA, // Subtract A by [DS:im]
    INS_SUB_DPO = 0xDB, // Subtract A by [[DS:im]+im]

    INS_INA     = 0xDC, // Increment A
    INS_DEA     = 0xDD, // Decrement A
    INS_INB     = 0xDE, // Increment B
    INS_DEB     = 0xDF, // Decrement B

    INS_SHR     = 0x20, // Shift A right by B
    INS_SHR_IM  = 0x21, // Shift A right by im
    INS_SHR_SO  = 0x22, // Shift A right by [SS:BP+im]
    INS_SHR_SPO = 0x23, // Shift A right by [[SS:BP+im]+im]
    INS_SHR_DO  = 0x24, // Shift A right by [DS:im]
    INS_SHR_DPO = 0x25, // Shift A right by [[DS:im]+im]

    INS_SHL     = 0x26, // Shift A left by B
    INS_SHL_IM  = 0x27, // Shift A left by im
    INS_SHL_SO  = 0x28, // Shift A left by [SS:BP+im]
    INS_SHL_SPO = 0x29, // Shift A left by [[SS:BP+im]+im]im]
    INS_SHL_DO  = 0x2A, // Shift A left by [DS:im]
    INS_SHL_DPO = 0x2B, // Shift A left by [[DS:im]+im]

    INS_INX     = 0x2C, // Increment X
    INS_DEX     = 0x2D, // Decrement X
    INS_INY     = 0x2E, // Increment Y
    INS_DEY     = 0x2F, // Decrement Y
};

#endif
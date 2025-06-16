#ifndef INSTRUCTIONS_HPP
#define INSTRUCTIONS_HPP

enum Instructions {
    // LDA - Load Accumulator
    INS_LDA_IM  = 0xA0, // Immediate
    INS_LDA_SO  = 0xA1, // Stack offset [SS:BP+X]
    INS_LDA_SPO = 0xA2, // Stack pointer offset [[SS:BP+X]+Y]
    INS_LDA_DO  = 0xA3, // Data offset [DS:X]
    INS_LDA_DPO = 0xA4, // Data pointer offset [[DS:X]+Y]

    // STA - Store Accumulator
    INS_STA_SO  = 0xA5, // [SS:BP+X]     = A
    INS_STA_SPO = 0xA6, // [[SS:BP+X]+Y] = A
    INS_STA_DO  = 0xA7, // [DS:X]        = A
    INS_STA_DPO = 0xA8, // [[DS:X]+Y]    = A

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

    // Stack operations
    INS_PUSHA   = 0x50, // Push A (not all)
    INS_POPA    = 0x51, // Pop A (not all)
    INS_PUSHB   = 0x52, // Push B
    INS_POPB    = 0x53, // Pop B
    INS_PUSHB   = 0x54, // Push X
    INS_POPB    = 0x55, // Pop X
    INS_PUSHB   = 0x56, // Push Y
    INS_POPB    = 0x57, // Pop Y
    INS_ENTER   = 0x58, // Enter stack frame ( push bp, bp = sp )
    INS_LEAVE   = 0x59, // Leave stack frame ( pop bp )
    INS_CALL    = 0x5A, // Call a function (push IP and short jump to address)
    INS_RET     = 0x5B, // Return from a function ( pop IP )

    // Misc transfers
    INS_TSSB    = 0x5C, // B = SS
    INS_TCSB    = 0x5D, // B = CS
    INS_TDSB    = 0x5E, // B = DS

    // Control flow
    INS_CMP     = 0xC0, // Compare A with B
    INS_JE      = 0xC1, // Jump equal,         if ZF == 0
    INS_JNE     = 0xC2, // Jump not equal,     if ZF == 1
    INS_JG      = 0xC3, // Jump greater,       if CF == 1
    INS_JL      = 0xC4, // Jump lesser,        if CF == 0
    INS_JGE     = 0xC5, // Jump greater equal, if NF == 1
    INS_JLE     = 0xC6, // Jump lesser equal,  if NF == 0
    INS_JMP     = 0xC7, // Short jump,         sets IP
    INS_LJMP    = 0xC8, // Long jump,          sets CS:IP

    // Interrupts
    INS_INT     = 0x00, // Interrupt 
                        //     push flags and IP, then reference the
                        //     interrupt jump table, and jump to the interrupt
                        //     handler
    INS_RTI     = 0x01, // Return from interrupt (pop IP and flags)
};

#endif
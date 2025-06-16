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
    INS_LDB_IM  = 0xB0, // Immediate
    INS_LDB_SO  = 0xB1, // Stack offset [SS:BP+X]
    INS_LDB_SPO = 0xB2, // Stack pointer offset [[SS:BP+X]+Y]
    INS_LDB_DO  = 0xB3, // Data offset [DS:X]
    INS_LDB_DPO = 0xB4, // Data pointer offset [[DS:X]+Y]

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
};

#endif
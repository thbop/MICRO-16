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

#include "Btp.hpp"

namespace btp {

// Executes one instruction
void BetterThanPico::Execute() {
    uint8_t instruction = Fetch();
    #ifdef BTP_DEBUG
    printf(
        "INS %02X CS:IP %04X:%04X A %04X B %04X X %04X Y %04X SS %04X DS %04X"
        "\n",
        instruction, CS, IP, A, B, X, Y, SS, DS
    );
    #endif

    switch ( instruction ) {
        // LDA
        case INS_LDA_IM:  A = LoadImmediate();                          break;
        case INS_LDA_SO:  A = LoadOffsetIm( SS, BP );                   break;
        case INS_LDA_SPO: A = LoadPointerImOffsetIm( SS, BP, 0 );       break;
        case INS_LDA_DO:  A = LoadOffsetIm( DS, 0 );                    break;
        case INS_LDA_DPO: A = LoadPointerImOffsetIm( DS, 0, 0 );        break;

        // STA
        case INS_STA_SO:  StoreOffsetIm( SS, BP, A );                   break;
        case INS_STA_SPO: StorePointerImOffsetIm( SS, BP, 0, A );       break;
        case INS_STA_DO:  StoreOffsetIm( DS, 0, A );                    break;
        case INS_STA_DPO: StorePointerImOffsetIm( DS, 0, 0, A );        break;

        // TA-X
        case INS_TAB:     B = A;  GenericFlagSet( A );                  break;
        case INS_TAX:     X = A;  GenericFlagSet( A );                  break;
        case INS_TAY:     Y = A;  GenericFlagSet( A );                  break;
        case INS_TASS:    SS = A; GenericFlagSet( A );                  break;
        case INS_TACS:    CS = A; GenericFlagSet( A );                  break;
        case INS_TADS:    DS = A; GenericFlagSet( A );                  break;

        // LDB
        case INS_LDB_IM:  B = LoadImmediate();                          break;
        case INS_LDB_SO:  B = LoadOffset( SS, BP + X );                 break;
        case INS_LDB_SPO: B = LoadPointerOffset( SS, BP + X, Y );       break;
        case INS_LDB_DO:  B = LoadOffset( DS, X );                      break;
        case INS_LDB_DPO: B = LoadPointerOffset( DS, X, Y );            break;

        // STB
        case INS_STB_SO:  StoreOffset( SS, BP + X, B );                 break;
        case INS_STB_SPO: StorePointerOffset( SS, BP + X, Y, B );       break;
        case INS_STB_DO:  StoreOffset( DS, X, B );                      break;
        case INS_STB_DPO: StorePointerOffset( DS, X, Y, B );            break;

        // TB-X
        case INS_TBA:     A = B;  GenericFlagSet( B );                  break;
        case INS_TBX:     X = B;  GenericFlagSet( B );                  break;
        case INS_TBY:     Y = B;  GenericFlagSet( B );                  break;
        case INS_TBSS:    SS = B; GenericFlagSet( B );                  break;
        case INS_TBCS:    CS = B; GenericFlagSet( B );                  break;
        case INS_TBDS:    DS = B; GenericFlagSet( B );                  break;

        // LDX
        case INS_LDX_IM:  X = LoadImmediate();                          break;
        case INS_LDX_SO:  X = LoadOffsetIm( SS, BP );                   break;
        case INS_LDX_SPO: X = LoadPointerOffsetIm( SS, BP, Y );         break;
        case INS_LDX_DO:  X = LoadOffsetIm( DS, 0 );                    break;
        case INS_LDX_DPO: X = LoadPointerOffsetIm( DS, 0, Y );          break;

        // STX
        case INS_STX_SO:  StoreOffsetIm( SS, BP, X );                   break;
        case INS_STX_SPO: StorePointerOffsetIm( SS, BP, Y, X );         break;
        case INS_STX_DO:  StoreOffsetIm( DS, 0, X );                    break;
        case INS_STX_DPO: StorePointerOffsetIm( DS, 0, Y, X );          break;

        // TX-X
        case INS_TXA:     A = X;  GenericFlagSet( X );                  break;
        case INS_TXB:     B = X;  GenericFlagSet( X );                  break;
        case INS_TXY:     Y = X;  GenericFlagSet( X );                  break;
        case INS_TXSS:    SS = X; GenericFlagSet( X );                  break;
        case INS_TXCS:    CS = X; GenericFlagSet( X );                  break;
        case INS_TXDS:    DS = X; GenericFlagSet( X );                  break;

        // LDY
        case INS_LDY_IM:  Y = LoadImmediate();                          break;
        case INS_LDY_SO:  Y = LoadOffset( SS, BP + X );                 break;
        case INS_LDY_SPO: Y = LoadPointerImOffset( SS, BP + X, 0 );     break;
        case INS_LDY_DO:  Y = LoadOffset( DS, X );                      break;
        case INS_LDY_DPO: Y = LoadPointerImOffset( DS, X, 0 );          break;

        // STY
        case INS_STY_SO:  StoreOffset( SS, BP + X, Y );                 break;
        case INS_STY_SPO: StorePointerImOffset( SS, BP + X, 0, Y );     break;
        case INS_STY_DO:  StoreOffset( DS, X, Y );                      break;
        case INS_STY_DPO: StorePointerImOffset( DS, X, 0, Y );          break;

        // TY-X
        case INS_TYA:     A = Y;  GenericFlagSet( Y );                  break;
        case INS_TYB:     B = Y;  GenericFlagSet( Y );                  break;
        case INS_TYX:     X = Y;  GenericFlagSet( Y );                  break;
        case INS_TYSS:    SS = Y; GenericFlagSet( Y );                  break;
        case INS_TYCS:    CS = Y; GenericFlagSet( Y );                  break;
        case INS_TYDS:    DS = Y; GenericFlagSet( Y );                  break;

        // Stack operations
        case INS_PUSHA:   Push16( A );                                  break;
        case INS_POPA:    A = Pop16();                                  break;
        case INS_PUSHB:   Push16( B );                                  break;
        case INS_POPB:    B = Pop16();                                  break;
        case INS_PUSHX:   Push16( X );                                  break;
        case INS_POPX:    X = Pop16();                                  break;
        case INS_PUSHY:   Push16( Y );                                  break;
        case INS_POPY:    Y = Pop16();                                  break;
        case INS_ENTER:   Push16( BP ); BP = SP;                        break;
        case INS_LEAVE:   SP = BP; BP = Pop16();                        break;
        case INS_CALL:    Push16( IP ); IP = Fetch16();                 break;
        case INS_RET:     IP = Pop16() + 2; /* Call argument offset */  break;

        // Misc transfers
        case INS_TSSA:    A = SS; GenericFlagSet( A );                  break;
        case INS_TCSA:    A = CS; GenericFlagSet( A );                  break;
        case INS_TDSA:    A = DS; GenericFlagSet( A );                  break;

        // Control flow + jumps
        case INS_CMP:     Compare( A, B );                              break;
        case INS_CMP_IM:  Compare( A, Fetch16() );                      break;
        case INS_CMP_SO:  Compare( A, LoadOffsetIm( SS, BP ) );         break;
        case INS_CMP_SPO:
            Compare( A, LoadPointerImOffsetIm( SS, BP, 0 ) );           break;
        case INS_CMP_DO:  Compare( A, LoadOffsetIm( DS, 0 ) );          break;
        case INS_CMP_DPO:
            Compare( A, LoadPointerImOffsetIm( DS, 0, 0 ) );            break;
        case INS_JE:      JumpCondition( flags.Z );                     break;
        case INS_JNE:     JumpCondition( !flags.Z );                    break;
        case INS_JG:      JumpCondition( flags.C );                     break;
        case INS_JGE:     JumpCondition( flags.N );                     break;
        case INS_JMP:     Jump();                                       break;
        case INS_LJMP:    LongJump();                                   break;

        // Interrupts
        case INS_INT:     Interrupt( Fetch() );                         break;
        case INS_RTI:     ReturnFromInterrupt();                        break;

        // Arithmetic
        case INS_ADD:     BTP_MATH_OP( A, B, + )                        break;
        case INS_ADD_IM:  BTP_MATH_OP( A, Fetch16(), + )                break;
        case INS_ADD_SO:  BTP_MATH_OP( A, LoadOffsetIm( SS, BP ), + )   break;
        case INS_ADD_SPO:
            BTP_MATH_OP( A, LoadPointerImOffsetIm( SS, BP, 0 ), + )     break;
        case INS_ADD_DO:  BTP_MATH_OP( A, LoadOffsetIm( DS, 0 ), + )    break;
        case INS_ADD_DPO:
            BTP_MATH_OP( A, LoadPointerImOffsetIm( DS, 0, 0 ), + )      break;

        case INS_SUB:     BTP_MATH_OP( A, B, - )                        break;
        case INS_SUB_IM:  BTP_MATH_OP( A, Fetch16(), - )                break;
        case INS_SUB_SO:  BTP_MATH_OP( A, LoadOffsetIm( SS, BP ), - )   break;
        case INS_SUB_SPO:
            BTP_MATH_OP( A, LoadPointerImOffsetIm( SS, BP, 0 ), - )     break;
        case INS_SUB_DO:  BTP_MATH_OP( A, LoadOffsetIm( DS, 0 ), - )    break;
        case INS_SUB_DPO:
            BTP_MATH_OP( A, LoadPointerImOffsetIm( DS, 0, 0 ), - )      break;

        case INS_SHR:     BTP_MATH_OP( A, B, >> )                       break;
        case INS_SHR_IM:  BTP_MATH_OP( A, Fetch16(), >> )               break;
        case INS_SHR_SO:  BTP_MATH_OP( A, LoadOffsetIm( SS, BP ), >> )  break;
        case INS_SHR_SPO:
            BTP_MATH_OP( A, LoadPointerImOffsetIm( SS, BP, 0 ), >> )    break;
        case INS_SHR_DO:  BTP_MATH_OP( A, LoadOffsetIm( DS, 0 ), >> )   break;
        case INS_SHR_DPO:
            BTP_MATH_OP( A, LoadPointerImOffsetIm( DS, 0, 0 ), >> )     break;

        case INS_SHL:     BTP_MATH_OP( A, B, << )                       break;
        case INS_SHL_IM:  BTP_MATH_OP( A, Fetch16(), << )               break;
        case INS_SHL_SO:  BTP_MATH_OP( A, LoadOffsetIm( SS, BP ), << )  break;
        case INS_SHL_SPO:
            BTP_MATH_OP( A, LoadPointerImOffsetIm( SS, BP, 0 ), << )    break;
        case INS_SHL_DO:  BTP_MATH_OP( A, LoadOffsetIm( DS, 0 ), << )   break;
        case INS_SHL_DPO:
            BTP_MATH_OP( A, LoadPointerImOffsetIm( DS, 0, 0 ), << )     break;
        
        case INS_INA:     BTP_MATH_OP( A, 1, + )                        break;
        case INS_DEA:     BTP_MATH_OP( A, 1, - )                        break;
        case INS_INB:     BTP_MATH_OP( B, 1, + )                        break;
        case INS_DEB:     BTP_MATH_OP( B, 1, - )                        break;
        case INS_INX:     BTP_MATH_OP( X, 1, + )                        break;
        case INS_DEX:     BTP_MATH_OP( X, 1, - )                        break;
        case INS_INY:     BTP_MATH_OP( Y, 1, + )                        break;
        case INS_DEY:     BTP_MATH_OP( Y, 1, - )                        break;

        // Bit tests
        case INS_AND:     BTP_MATH_OP( A, B, & )                        break;
        case INS_AND_IM:  BTP_MATH_OP( A, Fetch16(), & )                break;
        case INS_AND_SO:  BTP_MATH_OP( A, LoadOffsetIm( SS, BP ), & )   break;
        case INS_AND_SPO:
            BTP_MATH_OP( A, LoadPointerImOffsetIm( SS, BP, 0 ), & )     break;
        case INS_AND_DO:  BTP_MATH_OP( A, LoadOffsetIm( DS, 0 ), & )    break;
        case INS_AND_DPO:
            BTP_MATH_OP( A, LoadPointerImOffsetIm( DS, 0, 0 ), & )      break;

        case INS_OR:      BTP_MATH_OP( A, B, | )                        break;
        case INS_OR_IM:   BTP_MATH_OP( A, Fetch16(), | )                break;
        case INS_OR_SO:   BTP_MATH_OP( A, LoadOffsetIm( SS, BP ), | )   break;
        case INS_OR_SPO:
            BTP_MATH_OP( A, LoadPointerImOffsetIm( SS, BP, 0 ), | )     break;
        case INS_OR_DO:   BTP_MATH_OP( A, LoadOffsetIm( DS, 0 ), | )    break;
        case INS_OR_DPO:
            BTP_MATH_OP( A, LoadPointerImOffsetIm( DS, 0, 0 ), | )      break;

        case INS_XOR:     BTP_MATH_OP( A, B, ^ )                        break;
        case INS_XOR_IM:  BTP_MATH_OP( A, Fetch16(), ^ )                break;
        case INS_XOR_SO:  BTP_MATH_OP( A, LoadOffsetIm( SS, BP ), ^ )   break;
        case INS_XOR_SPO:
            BTP_MATH_OP( A, LoadPointerImOffsetIm( SS, BP, 0 ), ^ )     break;
        case INS_XOR_DO:  BTP_MATH_OP( A, LoadOffsetIm( DS, 0 ), ^ )    break;
        case INS_XOR_DPO:
            BTP_MATH_OP( A, LoadPointerImOffsetIm( DS, 0, 0 ), ^ )      break;

        // Flag stuff
        case INS_SEC:     flags.C = 1;                                  break;
        case INS_CLC:     flags.C = 0;                                  break;
        case INS_CLV:     flags.V = 0;                                  break;
    }
}

#ifdef BTP_DEBUG
// Dumps all the memory to a file
void BetterThanPico::DumpMemory( const char *outputFile ) {
    std::ofstream file( outputFile );

    file.write( (char*)memory->data(), BOB3K_SIZE );

    file.close();
}
#endif

}
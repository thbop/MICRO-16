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
        "INS: %02X IP: %02X A: %02X B: %02X X: %02X Y: %02X\n",
        instruction, IP, A.value, B.value, X, Y
    );
    #endif

    switch ( instruction ) {
        // LDA
        case INS_LDA_IM:  A.value = LoadImmediate();                    break;
        case INS_LDA_SO:  A.value = LoadOffsetIm( SS, BP );             break;
        case INS_LDA_SPO: A.value = LoadPointerImOffsetIm( SS, BP, 0 ); break;
        case INS_LDA_DO:  A.value = LoadOffsetIm( DS, 0 );              break;
        case INS_LDA_DPO: A.value = LoadPointerImOffsetIm( DS, 0, 0 );  break;

        // STA
        case INS_STA_SO:  StoreOffsetIm( SS, BP, A.value );             break;
        case INS_STA_SPO: StorePointerImOffsetIm( SS, BP, 0, A.value ); break;
        case INS_STA_DO:  StoreOffsetIm( DS, 0, A.value );              break;
        case INS_STA_DPO: StorePointerImOffsetIm( DS, 0, 0, A.value );  break;

        // TA-X
        case INS_TAB:     B.value = A.value;                            break;
        case INS_TAX:     X = A.value;                                  break;
        case INS_TAY:     Y = A.value;                                  break;
        case INS_TASS:    SS = A.value;                                 break;
        case INS_TACS:    CS = A.value;                                 break;
        case INS_TADS:    DS = A.value;                                 break;

        // LDB
        case INS_LDB_IM:  B.value = LoadImmediate();                    break;
        case INS_LDB_SO:  B.value = LoadOffset( SS, BP + X );           break;
        case INS_LDB_SPO: B.value = LoadPointerOffset( SS, BP + X, Y ); break;
        case INS_LDB_DO:  B.value = LoadOffset( DS, X );                break;
        case INS_LDB_DPO: B.value = LoadPointerOffset( DS, X, Y );      break;

        // STB
        case INS_STB_SO:  StoreOffset( SS, BP + X, B.value );           break;
        case INS_STB_SPO: StorePointerOffset( SS, BP + X, Y, B.value ); break;
        case INS_STB_DO:  StoreOffset( DS, X, B.value );                break;
        case INS_STB_DPO: StorePointerOffset( DS, X, Y, B.value );      break;

        // TB-X
        case INS_TBA:     A.value = B.value;                            break;
        case INS_TBX:     X = B.value;                                  break;
        case INS_TBY:     Y = B.value;                                  break;
        case INS_TBSS:    SS = B.value;                                 break;
        case INS_TBCS:    CS = B.value;                                 break;
        case INS_TBDS:    DS = B.value;                                 break;

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
        case INS_TXA:     A.value = X;                                  break;
        case INS_TXB:     B.value = X;                                  break;
        case INS_TXY:     Y = X;                                        break;
        case INS_TXSS:    SS = X;                                       break;
        case INS_TXCS:    CS = X;                                       break;
        case INS_TXDS:    DS = X;                                       break;

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
        case INS_TYA:     A.value = Y;                                  break;
        case INS_TYB:     B.value = Y;                                  break;
        case INS_TYX:     X = Y;                                        break;
        case INS_TYSS:    SS = Y;                                       break;
        case INS_TYCS:    CS = Y;                                       break;
        case INS_TYDS:    DS = Y;                                       break;

        case INS_PUSHA:   Push16( A.value );                            break;
        case INS_POPA:    A.value = Pop16();                            break;
        case INS_PUSHB:   Push16( B.value );                            break;
        case INS_POPB:    B.value = Pop16();                            break;
        case INS_PUSHX:   Push16( X );                                  break;
        case INS_POPX:    X = Pop16();                                  break;
        case INS_PUSHY:   Push16( Y );                                  break;
        case INS_POPY:    Y = Pop16();                                  break;
        case INS_ENTER:   Push16( BP ); BP = SP;                        break;
        case INS_LEAVE:   SP = BP; BP = Pop16();                        break;
        case INS_CALL:    Push16( IP ); IP = Fetch16();                 break;
        case INS_RET:     IP = Pop16() + 2; /* Call argument offset */  break;

        // JMP
        case INS_JMP:     IP += (int8_t)Fetch();                        break;
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
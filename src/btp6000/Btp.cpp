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

// Executes one instruction
void btp::BetterThanPico::Execute() {
    uint8_t instruction = Fetch();
    switch ( instruction ) {
        // LDA
        case INS_LDA_IM:  A.value = LoadImmediate();                    break;
        case INS_LDA_SO:  A.value = LoadOffset( SS, BP + X );           break;
        case INS_LDA_SPO: A.value = LoadPointerOffset( SS, BP + X, Y ); break;
        case INS_LDA_DO:  A.value = LoadOffset( DS, X );                break;
        case INS_LDA_DPO: A.value = LoadPointerOffset( DS, X, Y );      break;

        // STA
        case INS_STA_SO:  StoreOffset( SS, BP + X, A.value );           break;
        case INS_STA_SPO: StorePointerOffset( SS, BP + X, Y, A.value ); break;
        case INS_STA_DO:  StoreOffset( DS, X, A.value );                break;
        case INS_STA_DPO: StorePointerOffset( DS, X, Y, A.value );      break;

        // TA-X
        case INS_TAB:     B.value = A.value;                            break;
        case INS_TAX:     X = A.value;                                  break;
        case INS_TAY:     Y = A.value;                                  break;
        case INS_TASS:    SS = A.value;                                 break;
        case INS_TACS:    CS = A.value;                                 break;
        case INS_TADS:    DS = A.value;                                 break;

        // LDB
        case INS_LDB_IM:  B.value = LoadImmediate();                    break;
        case INS_LDB_SO:  B.value = LoadOffsetIm( SS, BP );             break;
        case INS_LDB_SPO: B.value = LoadPointerImOffsetIm( SS, BP, 0 ); break;
        case INS_LDB_DO:  B.value = LoadOffsetIm( DS, 0 );              break;
        case INS_LDB_DPO: B.value = LoadPointerImOffsetIm( DS, 0, 0 );  break;

        // STB
        case INS_STB_SO:  StoreOffsetIm( SS, BP, B.value );             break;
        case INS_STB_SPO: StorePointerImOffsetIm( SS, BP, 0, B.value ); break;
        case INS_STB_DO:  StoreOffsetIm( DS, 0, B.value );              break;
        case INS_STB_DPO: StorePointerImOffsetIm( DS, 0, 0, B.value );  break;

        // TB-X
        case INS_TBA:     A.value = B.value;                            break;
        case INS_TBX:     X = B.value;                                  break;
        case INS_TBY:     Y = B.value;                                  break;
        case INS_TBSS:    SS = B.value;                                 break;
        case INS_TBCS:    CS = B.value;                                 break;
        case INS_TBDS:    DS = B.value;                                 break;
    }
}
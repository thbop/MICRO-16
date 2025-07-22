; 
; Copyright © 2025 Micro-16 Team
; 
; Permission is hereby granted, free of charge, to any person obtaining a copy
; of this software and associated documentation files (the “Software”), to deal
; in the Software without restriction, including without limitation the rights
; to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
; copies of the Software, and to permit persons to whom the Software is
; furnished to do so, subject to the following conditions:
; 
; The above copyright notice and this permission notice shall be included in all
; copies or substantial portions of the Software.
; 
; THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
; IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
; FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
; AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
; LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
; OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
; SOFTWARE.
; 

.org 0x0000

; 16 Interrupt vectors
db panic       ; int 00h
db test_fill   ; int 01h
db panic       ; int 02h
db panic       ; int 03h
db panic       ; int 04h
db panic       ; int 05h
db panic       ; int 06h
db panic       ; int 07h
db panic       ; int 08h
db panic       ; int 09h
db panic       ; int 0Ah
db panic       ; int 0Bh
db panic       ; int 0Ch
db panic       ; int 0Dh
db panic       ; int 0Eh
db panic       ; int 0Fh


; Takes no arguments, just halts the processor
panic:
    jmp panic


; Fills the test sprite with the desired color
; Args
;     B = color
test_fill:
    pusha      ; push a
    tdsa
    pusha      ; push ds

    lda 0x3C0
    tads       ; Set ds to vram palette section

    tba
    sta [1]    ; Set the first non-background color to B

.fill:
    lda 0x300
    tads

    lda 0xFFFF ; Set the raw pixel data to reference this color
    sta [0]
    sta [2]
    sta [4]
    sta [6]
    lda 0x0000
    sta [8]
    sta [10]
    sta [12]
    sta [14]

    ; Clean up and return from interrupt
    popa
    tads       ; pop ds
    popa       ; pop a
    rti
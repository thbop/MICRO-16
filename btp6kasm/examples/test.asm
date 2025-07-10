.org 2000h
.data 3000h

main:
    lda 0x200
    tass       ; Stack Segment = 0x200

    lda 0xEFBE ; Load BE EF

.loop:
    pusha      ; Fill the stack with BE EF
    jmp .loop

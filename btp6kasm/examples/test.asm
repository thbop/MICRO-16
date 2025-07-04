.org 2000h


main:
    ; Setup stack
    lda 0x200
    tass       ; Stack Segment = 0x200


    lda 0xEFBE ; push BE EF to the stack
    pusha

; .side:
;     lda 0x4345

beef:
    pusha
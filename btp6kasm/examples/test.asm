.org 2000h


main:
    ; Setup stack
    lda 0x200
    tass       ; Stack Segment = 0x200

    ldb [[main]+0]

    lda 0xEFBE ; push BE EF to the stack
    pusha
    jmp main



beef:
    pusha
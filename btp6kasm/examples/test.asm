.org 2000h

; Setup stack
lda 0x200
tass


lda 0xEFBE ; push BE EF to the stack
pusha
.org 2000h

lda 0x12
tax
stx [0]
txy
ldx 2
sty [bp+x]
jmp 0xFE
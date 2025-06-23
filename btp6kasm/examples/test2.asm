.org 0000h

main:
    lda 50
    ldy 12
.side_quest:
    ldx [bp+12]
    ldb [bp+54]
    lda [[x]y]

main2:
    tax
.side_quest:
    tab

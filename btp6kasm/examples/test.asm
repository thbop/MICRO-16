.org $0000

main:
    lda 50
    ldy 12
.side_quest:
    ldx [bp+12]
    ldb [bp+x]
    lda [[x]y]

main2:
    tax
.side_quest:
    tab

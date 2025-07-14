.org 2000h

_start:
    jmp main


; Sets the first sprite and palette to resemble a pineapple
; Takes no args
pineapple_sprite:
    pushb        ; push B
    tdsb
    pushb        ; push DS
    pusha        ; push A


    lda 0x03C0
    tads         ; Set data segment to the vram palette section

    lda 0        ; Light blue
    sta [0]      ; Set background to light blue

    lda 8        ; Yellow
    sta [1]
    lda 10       ; Green
    sta [2]
    lda 2        ; White
    sta [3]

.draw:
    lda 0x0300
    tads         ; Set DS to sprite vram

    lda 0x0      ; Raw pixel data (no binary literals yet)
    sta [0]
    lda 0x2C
    sta [1]
    lda 0x7E
    sta [2]
    sta [3]
    sta [4]
    sta [5]
    sta [6]
    lda 0x3C
    sta [7]
    lda 0x18
    sta [8]
    lda 0x10
    sta [9]
    lda 0x8
    sta [11]
    sta [12]
    lda 0x10
    sta [13]

    ; Cleanup
    popa         ; pop A
    popb
    tbds         ; pop DS
    popb         ; pop B

    ret


; Tests control flow
; Takes no arguments
cmp_test:
    pusha
    pushb

.compare:
    lda 10
    ldb 5
    cmp
    jg .compare

    popa
    popb
    ret


main:
    lda 0x200
    tass         ; Stack Segment = 0x200

    call pineapple_sprite

    call cmp_test



halt:
    jmp halt
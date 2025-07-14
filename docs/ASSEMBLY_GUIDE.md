# Assembly Guide for Beginners

## Introduction

For more experienced programmers, check out the [assembly reference](ASSEMBLY_REFERENCE.md).

For many novice programmers, assembly is the ledgendary verbose-but-fast programming "language." It is some collection of elusive symbols that "run real fast."

While these statements are partially true, assembly can be more accurately described as *human-readable machine code*. Thus, to understand assembly, you have to understand the processor.

Watch this Tom Scott video for a visual explanation of the fetch-execute cycle:
[![thumbnail](https://img.youtube.com/vi/Z5JC9Ve1sfI/0.jpg)](https://www.youtube.com/watch?v=Z5JC9Ve1sfI)

## Assembly -> Machine code

Assembly is only a human readable representation of machine code. For example this code:
```x86asm
; Micro-16 (btp6kasm) assembly

main:
    lda 12        ; Loads 12 into the accumulator
    ldb 10        ; Loads 10 into the base (another register like A)

    je .do_thing  ; Jump if A == B
    jmp .stop     ; Otherwise, jump to stop

.do_thing:
    ; ...         ; Notice, once .do_thing is finished the CPU will
                  ; automatically execute down to .stop and so on.

.stop:
    ; ...
```

Assembles into this (documented) machine code:
```
A0 0C 00 ; lda 12 (0C in hex is 12)
B0 0A 00 ; ldb 10
C6 02    ; je .do_thing (jumps past the next instruction)
CA 00    ; jmp .stop (would jump past .do_thing if there was something there)
```

## Bytes

So thats it, machine code is purely numbers. This means if something goes wrong, the CPU might try to execute your variables instead of your code!

Though while programming assembly it is important to remember that digital numbers are finite. For example, an unsigned byte (cannot be negative) has a range from 0 to 255. Thus, immediate byte instructions cannot exceed this limit.

To get around this, many immediate values are words (two bytes strung together); these have an unsigned range of 0 to 65536. [Little Endian](https://en.wikipedia.org/wiki/Endianness) systems will put the least significant byte (lsb) first (which is why a 12 is represented as 0C 00 **instead of** 00 0C).

So when working with immediate arguments (numbers in your code/small jumps) keep these restrictions in mind.

## Segmentation

Since the Micro-16's CPU (BetterThanPico 6000) is fully custom, I primarily picked bits and pieces that I liked from the [Intel 8086](https://en.wikipedia.org/wiki/Intel_8086) and the [Mos Techology 6502](https://en.wikipedia.org/wiki/MOS_Technology_6502). One of the 8086's more confusing features I chose to adopt is its memory segmentation.

Essentially, to organize memory access we could choose to break our memory up into chunks, let's say each chunk is 256 bytes wide. Let's say our particular task needed 250 bytes of memory, then we would have 6 bytes of waste (which is not that bad). Same goes if we needed 500 bytes (then we could just use two chunks and waste only 12 bytes). The problem with this method of segmentation is aparent when we try to allocate something like 257 bytes of memory. We now have to use two segments and are wasting 255 bytes!

To fix this issue, we could instead make each segment quite large (say 0xFFFF bytes), but offset each overlapping segment by 16 bytes. Now segments can be large (0xFFFF) or small (0x10) and will at most only waste 15 bytes (which is not too bad!).

## The Stack

The stack is a fairly basic and beginner-friendly data structure that is built into many CPUs. Essentially, the stack is a first-in-last-out data structure. You can push stuff on top and you can take stuff off the top. Let's have an example, say we have a basic function:
```x86asm
; Function demonstrating push and pop
; No arguments
function:
    ; Note: in x86 assembly pusha and popa mean something completely different
    pusha  ; Pushes the old value of A (61) to the stack
    lda 45 ; Now we can load in a new value of A and do some processing

    ; Do stuff with A

    popa   ; When we're done, let's pop the old value (61) back into A

    ret


main:
    lda 61
    call function
    ; `A` still equals 61
```

Now it is important to remember push and pop order, for example:
```x86asm
pusha ; Stack: -> A
pushb ; Stack: -> B -> A

popa  ; Stack: <- B <- A
popb  ; Stack: <- A

; Now A = B and B = A
```
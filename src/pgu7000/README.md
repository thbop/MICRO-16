# The Pixel Graphics Unit 7000

*Basically a GPU*

## Memory Layout
The PGU uses the memory from 0x3000 to 0x3C31 to store sprites, background 
elements, and palettes. This memory is further divided as follows:
| Range       | Name        | Description                                  |
|-------------|-------------|----------------------------------------------|
| 3000h-37FFh | Sprites     | Stores the sprite images                     |
| 3800h-39FFh | Nametable 0 | Stores the arrangement of background sprites |
| 3A00h-3BFFh | Nametable 1 | Second nametable for scroll                  |
| 3C00h-3C31h | Palette     | Stores the background and sprite colors      |

The structure of these regions of memory is inspired by the NES, so it might be 
worth checking out how the NES PPU works:

[![NES Emulator Part #4: PPU - Background Rendering](https://img.youtube.com/vi/-THeUXqR3zY/0.jpg)](https://www.youtube.com/watch?v=-THeUXqR3zY)

### Sprites

Essentially, sprites are 8x8 bitmap images. Each pixel is 2 bits for a total of
16 bytes per sprite (2 bits * 8 * 8 = 128 bits; 128 bits / 8 bits = 16 bytes). A 
naive implementation would store each pixel's bits right next to each other for 
effectively 4 pixels per byte (( 8 pixels * 8 pixels ) / 4 pixels per byte = 16
bytes). Extracting each pixel's value would take many unecessarily complicated
bit shifts, so instead we will opt for a more workable model. The first 8 bytes
will store the least significant bit (lsb) of each pixel and the next 8 bytes
will store the most significant bit (msb) of each pixel. Here's an example:

```c
// LSB
0b01010000 // Row 0
0b00000000
0b00000000
0b00000000
0b00000000
0b00000000
0b00000000
0b00000000 // Row 7

// MSB
0b00110000
0b00000000
0b00000000
0b00000000
0b00000000
0b00000000
0b00000000
0b00000000

pix[0, 0] = 0
pix[1, 0] = 1
pix[2, 0] = 2
pix[3, 0] = 3
```

We store 128 of these sprites in a 16x8 grid. To index a sprite we follow the
formula:
`byteOffset = ( y * width + x ) * spriteSize`
Though since both width and spriteSize are 16, instead of multiplying by 16, we
can bit shift to the left 4 times. Thus in bits our indexing formula can be
simplified to:
`0000 yyyy xxxx 0000` (big endian word for readability)

In C/C++, the topmost formula will be used for clarity, but the binary trick
can be very useful for assembly.

### Nametables

TODO

### Palettes

Colors in memory are stored as indices into a color table. (Check out the
`Colors` enum and the `colors` array in `Pgu.hpp`.) Thus, each color
technically could take only 4 bits, but for simplicity we are keeping them
byte aligned. The palette memory begins with a single byte defining the
background color, then proceeded by 16 3-byte entries. When a sprite is
rendered, pixels with the value of 0 will default to the background color.
Pixels 1-3 will index the selected palette. Thus, a single sprite can have any
palette for variety.

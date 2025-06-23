# The Pixel Graphics Unit 7000

Basically a GPU

## TODO List:
- SDL3 abstraction that creates a 128x128 screen and allows per-pixel drawing with a limited palette/palette swaping
- The PGU will interface with this abstraction and receive draw calls from memory.
- - Draw calls are interrupts that jump to CPU-side code
- https://lospec.com/palette-list/anb16 palette
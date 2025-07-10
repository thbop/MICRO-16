# Better Than Pico 6000 Assembler

The btp6kasm transforms assembly code into and object file (.o). This file will
be linked into the final cartridge by the linker (TODO). Object creation takes
two steps, lexing and parsing. The [lexer](lexer.hpp) iterates through the
assembly source file and determines the type and value of each symbol/token.
Tokens include instruction names, brackets ([]), operators (+), numbers, and so
on. Throughout the process of lexing, basic syntax is validated and the
appropriate errors will be displayed. The [parser](parser.hpp) will take the
lexer structure and construct the raw bytes of the object file, handling
labels, instructions, etc.

## Object File Format
The actual assembler will not produce a valid cartridge, instead it will
produce a custom object file for linking. The object file is composed of
several chunks (or blocks of data) containing linker information and machine
code. Chunks are defined in the metadata section at the very beginning.

### Metadata
The first byte of the file provides the length in bytes of the metadata
section. Here is an example entry:

```
BYTE type
WORD pointer
```

Here is a table of the available chunk types:

| Code | Type   | Description                                         |
|------|--------|-----------------------------------------------------|
| 0x00 | Header | Contains all origin, label, and external references |
| 0x01 | Code   | Contains raw machine code                           |

### Header Chunk

```
WORD origin      // Code origin (.org)
WORD data origin // Data origin (.data)
```

### Code chunk

Literally just the code. Though in areas where immediate labels are used,
immediate values are set to zero.
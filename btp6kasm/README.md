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
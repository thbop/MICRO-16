# Assembly Reference

## Contents
- [Registers](#registers)
- [Addressing Modes](#addressing-modes)
- [Instructions](#instructions)
- [Linker Directives](#linker-directives)

## Registers

All registers are 16 bit besides the flags.

### General purpose
- Accumulator (A)
- Base (B)

### Index/Pointer
- X-index (X)
- Y-pointer (Y)

### Segments
- Stack Segment (SS)
- Code Segment (CS)
- Data Segment (DS)

Check out the [assembly guide](ASSEMBLY_GUIDE.md) for more information about segmentation.

### Flags
- Carry flag (CF)
- Zero flag (ZF)
- Overflow flag (VF)
- Negative flag (NF)
- Interrupt flag (IF)

### Program
- Instruction Pointer (IP)

### Stack
- Stack Pointer (SP)
- Base Pointer (BP)

## Addressing Modes

| Name  | Description                    |
|-------|--------------------------------|
| IM8   | An immediate byte (or a label) |
| IM16  | An immediate word (or a label) |
| SO    | [SS:BP + X]                    |
| SOI   | [SS:BP + IM8]                  |
| SPO   | [[SS:BP + X] + Y]              |
| SPOI  | [[SS:BP + IM8] + Y]            |
| SPOI  | [[SS:BP + X] + IM16]           |
| SPIOI | [[SS:BP + IM8] + IM16]         |
| DO    | [DS:X]                         |
| DOI   | [DS:IM8]                       |
| DPO   | [[DS:X] + Y]                   |
| DPOI  | [[DS:IM8] + Y]                 |
| DPOI  | [[DS:X] + IM16]                |
| DPIOI | [[DS:IM8] + IM16]              |


## Instructions

For opcodes, check the [Instructions.hpp](../src/btp6000/Instructions.hpp) file.

| Name  | Addressing Modes                   | Description                          |
|-------|------------------------------------|--------------------------------------|
| LDA   | IM16, SOI, SPIOI, DOI, DPIOI       | Loads a value into the accumulator   |
| STA   | SOI, SPIOI, DOI, DPIOI             | Stores the accumulator in memory     |
| TAB   | NONE                               | Transfer A to B                      |
| TAX   | NONE                               | Transfer A to X                      |
| TAY   | NONE                               | Transfer A to Y                      |
| TASS  | NONE                               | Transfer A to SS                     |
| TACS  | NONE                               | Transfer A to CS                     |
| TADS  | NONE                               | Transfer A to DS                     |
| LDB   | IM16, SO, SPO, DO, DPO             | Loads a value into the base          |
| STB   | SO, SPO, DO, DPO                   | Stores the base in memory            |
| TBA   | NONE                               | Transfer B to A                      |
| TBX   | NONE                               | Transfer B to X                      |
| TBY   | NONE                               | Transfer B to Y                      |
| TBSS  | NONE                               | Transfer B to SS                     |
| TBCS  | NONE                               | Transfer B to CS                     |
| TBDS  | NONE                               | Transfer B to DS                     |
| LDX   | IM16, SOI, SPOI, DOI, DPOI         | Loads a value into the X-index       |
| STX   | SOI, SPOI, DOI, DPOI               | Stores the base in memory            |
| TXA   | NONE                               | Transfer X to A                      |
| TXB   | NONE                               | Transfer X to B                      |
| TXY   | NONE                               | Transfer X to Y                      |
| TXSS  | NONE                               | Transfer X to SS                     |
| TXCS  | NONE                               | Transfer X to CS                     |
| TXDS  | NONE                               | Transfer X to DS                     |
| LDY   | IM16, SO, SPIO, DO, DPIO           | Loads a value into the Y-pointer     |
| STY   | SO, SPIO, DO, DPIO                 | Stores the Y-pointer in memory       |
| TYA   | NONE                               | Transfer Y to A                      |
| TYB   | NONE                               | Transfer Y to B                      |
| TYX   | NONE                               | Transfer Y to X                      |
| TYSS  | NONE                               | Transfer Y to SS                     |
| TYCS  | NONE                               | Transfer Y to CS                     |
| TYDS  | NONE                               | Transfer Y to DS                     |
| TSSA  | NONE                               | Transfer SS to A                     |
| TCSA  | NONE                               | Transfer CS to A                     |
| TDSA  | NONE                               | Transfer DS to A                     |
| PUSHA | NONE                               | Pushes A to the stack                |
| POPA  | NONE                               | Pops A from the stack                |
| PUSHB | NONE                               | Pushes B to the stack                |
| POPB  | NONE                               | Pops B from the stack                |
| PUSHX | NONE                               | Pushes X to the stack                |
| POPX  | NONE                               | Pops X from the stack                |
| PUSHY | NONE                               | Pushes Y to the stack                |
| POPY  | NONE                               | Pops Y from the stack                |
| ENTER | NONE                               | Enter stack frame (push BP, BP = SP) |
| LEAVE | NONE                               | Exit stack frame (SP = BP, pop BP)   |
| CALL  | IM16                               | Calls a function (push IP, jump)     |
| RET   | NONE                               | Returns from a function (pop IP)     |
| CMP   | NONE, IM16, SOI, SPIOI, DOI, DPIOI | Compares a value with A              |
| JE    | IM16                               | Jump if equal (ZF == 1)              |
| JNE   | IM16                               | Jump if not equal (ZF == 0)          |
| JG    | IM16                               | Jump if greater (CF == 1)            |
| JGE   | IM16                               | Jump if greater or equal (NF == 1)   |
| JMP   | IM8                                | Unconditional short jump             |
| LJMP  | IM16                               | Uncondition long jump

## Linker Directives

| Name   | Arguments | Description                           |
|--------|-----------|---------------------------------------|
| .org   | IM16      | Code Origin                           |
| extern | LABEL     | Reference or create an external label |
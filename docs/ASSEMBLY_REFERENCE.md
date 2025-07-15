# Assembly Reference

## Contents
- [Registers](#registers)
- [Addressing Modes](#addressing-modes)
- [Instructions](#instructions)
- [Linker Directives](#linker-directives)

## Registers
### General purpose
- Accumulator (A)
- Base (B)

### Index/Pointer
- X-index (X)
- Y-pointer (Y)

### Segment
- Stack Segment (SS)
- Code Segment (CS)
- Data Segment (DS)
Check out the [assembly guide](ASSEMBLY_GUIDE.md) for more information about segmentation.

## Flag
- Carry flag (C)
- Zero flag (Z)
- Overflow flag (V)
- Negative flag (N)
- Interrupt flag (I)

## Addressing Modes

| Name  | Description            |
|-------|------------------------|
| IM8   | An immediate byte      |
| IM16  | An immediate word      |
| SO    | [SS:BP + X]            |
| SOI   | [SS:BP + IM8]          |
| SPO   | [[SS:BP + X] + Y]      |
| SPOI  | [[SS:BP + IM8] + Y]    |
| SPOI  | [[SS:BP + X] + IM16]   |
| SPIOI | [[SS:BP + IM8] + IM16] |
| DO    | [DS:X]                 |
| DOI   | [DS:IM8]               |
| DPO   | [[DS:X] + Y]           |
| DPOI  | [[DS:IM8] + Y]         |
| DPOI  | [[DS:X] + IM16]        |
| DPIOI | [[DS:IM8] + IM16]      |


## Instructions

### LDA
Loads the a value into the accumulator.

## Linker Directives
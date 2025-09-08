BCPU Emulator is a CPU emulator, that emulates a custom instruction set CPU
the instruction set is as follows

- 0000 - NOP no operation
- 0001 - STA store register A
- 0010 - STX store register X
- 0011 - STY store register Y
- 0100 - STN store operand as number at PC address
- 0101 - LDX load register X
- 0110 - LDY load register Y
- 0111 - TAX transfer Register A into X
- 1000 - ADD adds X and Y together and stores sum in A register 
- 1001 - SUB subtracts Y from X and stores difference in A
- 1010 - CMP subtracts Y from X and discards the difference
- 1011 - AND bitwise and (X & Y)
- 1100 - OR  bitwise or (X | Y)
- 1101 - JSR jump to subroutine
- 1110 - JMP jump to address
- 1111 - BNE branch if not equal

Address  Instruction (2 bytes each)
// 0x00:    LDY  0xF0      ; load input into RegY
// 0x02:    SNX  0x01      ; RegX = 1
// 0x04:    AND  0x00      ; RegA = RegX & RegY  -> LSB in RegA; sets Z flag if 0 (even)
// 0x06:    BEQ  0x0E      ; if Z (even) jump to set 0xFF in RAM[0xE0]
// 0x08:    SNX  0x00      ; RegX = 0 (odd case)
// 0x0A:    STX  0xE0      ; store 0 at RAM[0xE0] (odd)
// 0x0C:    JMP  0x12      ; jump to HALT
// 0x0E:    SNX  0xFF      ; RegX = 255 (even case)
// 0x10:    STX  0xE0      ; store 255 at RAM[0xE0] (even)
// 0x12:    NOP/HALT 0xFF  ; HALT

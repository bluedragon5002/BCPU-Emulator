#include <iostream>
#include <cstdint>
#include <array>
#include <vector>
#include <iomanip>
#include <limits>
#include <algorithm>

using std::cout;
using std::cerr;
using std::cin;
using std::endl;

typedef uint8_t Byte;
typedef uint16_t Word;

class MemIO {
public:
    static constexpr size_t SIZE = 256;

    MemIO() { RAM.fill(0); }

    Byte Read8(Word address) const {
        if (address >= SIZE) {
            cerr << "Memory read out of range: " << address << "\n";
            return 0;
        }
        return RAM[address];
    }

    void Write8(Word address, Byte data) {
        if (address >= SIZE) {
            cerr << "Memory write out of range: " << address << "\n";
            return;
        }
        RAM[address] = data;
    }

    void Fill(const Byte* data, size_t len, Word offset = 0) {
        if (offset + len > SIZE) len = SIZE - offset;
        for (size_t i = 0; i < len; ++i) RAM[offset + i] = data[i];
    }

    void Clear() { RAM.fill(0); }

    const std::array<Byte, SIZE>& Raw() const { return RAM; }

private:
    std::array<Byte, SIZE> RAM{};
};

MemIO Ram;

class CPU {
public:
    explicit CPU(MemIO &memory, bool debug = false) : mem(memory), debugMode(debug) { Reset(); }

    void Reset() {
        RegA = RegX = RegY = 0;
        RegPC = 0;
        FlagZ = false;
        FlagHalt = false;
        ExecState = false;
        RegInst = 0;
    }

    void Cycle() {
        if (FlagHalt) return;
        if (!ExecState) {
            Fetch();
            ExecState = true;
        } else {
            DecodeInst(RegInst);
            ExecState = false;
        }
    }

    bool IsHalted() const { return FlagHalt; }

private:
    MemIO &mem;
    bool debugMode;

    // Registers
    Byte RegA{0}, RegX{0}, RegY{0};
    Word RegPC{0};
    bool FlagZ{false};
    bool FlagHalt{false};
    bool ExecState{false};

    Word RegInst{0};

    void Fetch() {
        // Fetch two bytes at PC
        RegInst = (Word(mem.Read8(RegPC)) << 8) | Word(mem.Read8(RegPC + 1));
        if (debugMode) cout << "Fetched 0x" << std::hex << std::setw(4) << std::setfill('0') << RegInst << std::dec << "\n";
    }

    void DecodeInst(Word inst) {
        Byte opcode = (inst >> 8) & 0xFF;
        Byte operand = inst & 0xFF;
        bool pcModified = false;

        if (debugMode) cout << "Exec: opcode=" << int(opcode) << " operand=0x" << std::hex << int(operand) << std::dec << "\n";

        switch (opcode) {
            case 0: // NOP or HALT
                if (operand == 0xFF) {
                    FlagHalt = true;
                    if (debugMode) cout << "HALT encountered\n";
                }
                break;
            case 1: // STA addr
                mem.Write8(operand, RegA);
                break;
            case 2: // STX addr
                mem.Write8(operand, RegX);
                break;
            case 3: // STY addr
                mem.Write8(operand, RegY);
                break;
            case 4: // SNX immediate (set RegX = immediate)
                RegX = operand;
                break;
            case 5: // LDX addr
                RegX = mem.Read8(operand);
                break;
            case 6: // LDY addr
                RegY = mem.Read8(operand);
                break;
            case 7: // TAX
                RegX = RegA;
                break;
            case 8: // ADD (RegA = RegX + RegY)
                RegA = Byte(RegX + RegY); // wrap-around semantics
                FlagZ = (RegA == 0);
                break;
            case 9: // SUB (RegA = RegX - RegY)
                RegA = Byte(RegX - RegY);
                FlagZ = (RegA == 0);
                break;
            case 10: // CMP (sets zero flag based on RegX - RegY)
                FlagZ = (RegX == RegY);
                break;
            case 11: // AND
                RegA = RegX & RegY;
                FlagZ = (RegA == 0);
                break;
            case 12: // OR
                RegA = RegX | RegY;
                FlagZ = (RegA == 0);
                break;
            case 14: // JMP addr
                RegPC = operand;
                pcModified = true;
                break;
            case 15: // BEQ addr
                if (FlagZ) {
                    RegPC = operand;
                } else {
                    RegPC = RegPC + 2; // skip this instruction
                }
                pcModified = true;
                break;
            default:
                cerr << "Unknown opcode: " << int(opcode) << " -- halting.\n";
                FlagHalt = true;
                break;
        }

        // If instruction did not explicitly change PC and CPU not halted, advance by instruction size
        if (!pcModified && !FlagHalt) {
            RegPC = RegPC + 2;
        }

        if (debugMode) {
            cout << "Regs: A=" << int(RegA) << " X=" << int(RegX) << " Y=" << int(RegY) << " PC=0x" << std::hex << RegPC << std::dec << " Z=" << FlagZ << "\n";
        }
    }
};

int main(int argc, char** argv) {
    // Set debug to false by default, set to true for verbose instruction trace
    bool debug = false;
    CPU cpu(Ram, debug);

    // Example program (same as original) -- keep as vector so size is clear
    std::vector<Byte> program = {
        0x06, 0xF0, // LDY 0xF0   00
        0x04, 0xFF, // SNX 0xFF   02
        0x0B, 0x00, // AND        04
        0x05, 0x00, // LDX 0x00   06
        0x0A, 0x00, // CMP        08
        0x0F, 0x12, // BEQ 0x12   0A
        0x04, 0x00, // SNX 0x00   0C
        0x02, 0xE0, // STX 0xE0   0E
        0x0E, 0x16, // JMP 0x16   10  (note: original comment referenced 0x1C)
        0x04, 0xFF, // SNX 0xFF   12
        0x02, 0xE0, // STX 0xE0   14
        0x00, 0xFF  // HALT       16
    };

    // Load program into RAM starting at 0
    Ram.Clear();
    Ram.Fill(program.data(), program.size(), 0);

    // Print program memory snapshot
    cout << "Program memory snapshot:\n";
    for (size_t i = 0; i < program.size(); ++i) {
        cout << "Ram at 0x" << std::hex << i << ": 0x" << std::setw(2) << std::setfill('0') << int(Ram.Read8(i)) << std::dec << "\n";
    }

    // Read user input (robust)
    int input = -1;
    while (true) {
        cout << "\nEnter a number (0-255): ";
        if (!(cin >> input)) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number between 0 and 255.\n";
            continue;
        }
        if (input < 0 || input > 255) {
            cout << "Value out of range. Clamping to 0-255.\n";
            input = std::clamp(input, 0, 255);
        }
        break;
    }

    Ram.Write8(0xF0, static_cast<Byte>(input & 0xFF));

    // Execute until HALT
    while (!cpu.IsHalted()) {
        cpu.Cycle();
    }

    cout << "If RAM at 0xE0 is 255 the number was even, else odd.\n";
    cout << "Ram at 0xE0: " << int(Ram.Read8(0xE0)) << "\n";

    return 0;
}

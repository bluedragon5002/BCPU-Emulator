#include <iostream>
using namespace std;
typedef unsigned char Byte;

class MemIO {
public:
	Byte Read8(Byte Address) {
		return RAM[Address];
	}
	void Write8(Byte Address, Byte Data) {
		RAM[Address] = Data;
	}
private: 
	Byte RAM[256];
};
MemIO Ram;

class CPU {
private:
	Byte RegA, RegX, RegY, RegPC = 0;
	bool FlagZ = false, ExecState = false;
	short RegInst;
public:	
	bool FlagHalt = false;
void DecodeInst(short inst) {
	Byte opcode = inst >> 8;
	Byte operand = inst & 0x00FF;
	switch (opcode) {
		case 0: //nop or halt
			if (operand == 255) FlagHalt = true; 
		case 1: //sta
			Ram.Write8(operand, RegA); break;
		case 2: //stx
			Ram.Write8(operand, RegX); break;
		case 3: //sty
			Ram.Write8(operand, RegY); break;
		case 4: //snx
			RegX = operand;
		case 5: //ldx
			RegX = Ram.Read8(operand); break;
		case 6: //ldy
			RegY = Ram.Read8(operand); break;
		case 7: //tax 
			RegX = RegA; break;
		case 8: //add
			RegA = RegX + RegY; (RegX - RegY) == 0; break;
		case 9: //sub
			RegA = RegX - RegY; (RegX - RegY) == 0; break;
		case 10: //cmp
			FlagZ = (RegX - RegY) == 0; break;
		case 11: //and
			RegA = RegX & RegY; break;
		case 12: //or
			RegA = RegX | RegY; break;
		case 14: //jmp
			RegPC = operand; break;
		case 15: //beq
			if (FlagZ) RegPC = operand; else RegPC += 2; break;
	}
	if(opcode < 13) RegPC += 2;
	cout << (int) opcode << " " << (int) operand << endl;
}
void Cycle() {
	if(FlagHalt) return;
	if(!ExecState) {
		RegInst = (short) Ram.Read8(RegPC) << 8;
		RegInst = RegInst | Ram.Read8(RegPC + 1);
		ExecState = true;
	} else {
		DecodeInst(RegInst);
		ExecState = false;
	}
}
};

int main(int argc, char** argv) {
	CPU cpu;
	
	//cpu decode test
	/*Ram.Write8(0, 10);
	Ram.Write8(1, 5);
	
	cout << "Ram\n00: 10\n01: 5\n";
	
	cpu.DecodeInst(0x0500); 
	cpu.DecodeInst(0x0601);
	cpu.DecodeInst(0x0800);
	cpu.DecodeInst(0x0102);
	
	cout << "02: " << (int) Ram.Read8(2) << endl;*/
	
	//cpu code execution test
	Byte program[] = {
	0x06, 0xF0, //LDY 0xF0	00
	0x04, 0xFF,	//SNX 1		02
	0x0B, 0x00, //AND		04
	0x05, 0x00, //LDY 0x00	06
	0x0A, 0x00, //CMP		08
	0x0F, 0x12, //BEQ 0x12	0A
	0x04, 0x00, //SNX 0		0C
	0x02, 0xE0, //STX 0xE0	0E
	0x0E, 0x16,	//JMP 0x1C	10
	0x04, 0xFF, //SNX 255	12
	0x02, 0xE0, //STX 0xE0	14
	0x00, 0xFF 	//HALT		16
	};
	
	for (int i=0;i<24;i++) {
		Ram.Write8(i, program[i]);
	}
	
	for (int i=0;i<24;i++) {
		cout << "Ram at " << std::hex << i << ": " << std::dec << (int) Ram.Read8(i) << endl;
	}
	
	int input = 0;
	cout << "\nenter a number: ";
	cin >> input;
	Ram.Write8(0xF0, input);
	
	while (!cpu.FlagHalt) {
		cpu.Cycle();
	}
	
	cout << "if Ram at 0xE0 is 255 number is even, else odd\n";
	cout << "Ram at 0xE0: " << (int) Ram.Read8(0xE0) << endl;
	
	return 0;
}

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
	Byte RegA, RegX, RegY, RegPC;
	bool FlagZ;
public:	
void DecodeInst(short inst) {
	Byte opcode = inst >> 8;
	Byte operand = inst & 0x00FF;
	switch (opcode) {
		case 1: //sta
			Ram.Write8(operand, RegA); break;
		case 2: //stx
			Ram.Write8(operand, RegX); break;
		case 3: //sty
			Ram.Write8(operand, RegY); break;
		case 4: //stn
			Ram.Write8(RegPC, operand); break;
		case 5: //ldx
			RegX = Ram.Read8(operand); break;
		case 6: //ldy
			RegY = Ram.Read8(operand); break;
		case 7: //tax 
			RegX = RegA; break;
		case 8: //add
			RegA = RegX + RegY; break;
		case 9: //sub
			RegA = RegX - RegY; break;
		case 10: //cmp
			FlagZ = (RegX - RegY) == 0; break;
		case 11: //and
			RegA = RegX & RegY; break;
		case 12: //or
			RegA = RegX | RegY; break;
		case 14: //jmp
			RegPC = operand; break;
		case 15: //bne
			if (FlagZ) RegPC = operand; break;
	}
	if(opcode < 13) RegPC++;
}
};

int main(int argc, char** argv) {
	CPU cpu;
	
	//cpu decode test
	Ram.Write8(0, 10);
	Ram.Write8(1, 5);
	
	cout << "Ram\n00: 10\n01: 5\n";
	
	cpu.DecodeInst(0x0500);
	cpu.DecodeInst(0x0601);
	cpu.DecodeInst(0x0800);
	cpu.DecodeInst(0x0102);
	
	cout << "02: " << (int) Ram.Read8(2) << endl;
	
	return 0;
}

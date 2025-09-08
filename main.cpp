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
public:	
void DecodeOpcode(short inst) {
	Byte opcode = inst >> 8;
	Byte operand = inst & 0x00FF;
	switch (opcode) {
		case 0: //nop
			RegPC++;
		break;
		case 1: //sta
			Ram.Write8(operand, RegA);
		break;
		case 2: //stn
			Ram.Write8(RegPC, operand);
		break;
		case 3: //ldy
			RegY = Ram.Read8(operand);
		break;
		case 4: //swp
			
		break;
		case 5: //tax
			RegX = RegA;
		break;
		case 6: //jmp
			RegPC = operand;
		break;
		case 7: //bne
	}
	if(opcode < 6) RegPC++;
}

int main(int argc, char *argv[])
{
	int opcode;
	cout << "enter opcode: ";
	cin >> opcode;

}
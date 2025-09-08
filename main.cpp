#include <iostream>
using namespace std;
typedef unsigned char Byte;

class MemIO {
public:
	Byte Read8(Byte Address) {
		RAM[Address];
	}
private: 
	Byte RAM[256];
}

class CPU {
public:
	Byte RegA, RegX, RegY, RegPC;
	
void DecodeOpcode(int opcode) {
	switch (opcode) {
		case 0: //nop
		break;
		case 1: //lda
		
		break;
		
	}
}

int main(int argc, char *argv[])
{
	int opcode;
	cout << "enter opcode: ";
	cin >> opcode;

}
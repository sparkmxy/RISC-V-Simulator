#include "pch.h"
#include "Simulator.h"

//#define DEBUG

Simulator::Simulator(std::istream &_is,std::ostream &_os): is(_is),os(_os){
	ISA_base::linkMemoryAndRegisters(&Memory, &Register);
}


Simulator::~Simulator(){}

void Simulator::run() {
	Memory.init(is);
	Register.init();
	ISA_base *flowLine[5];
	for (int i = 0; i < 5; i++) flowLine[i] = nullptr;
	while (true) {
#ifdef DEBUG
		Register.show();
		for (int i = 0; i < 4; i++)
			if (flowLine[i] == nullptr) std::cout << "null ";
			else std::cout << flowLine[i]->code << " ";
		std::cout << '\n';
#endif // DEBUG
		if (flowLine[2] != nullptr && flowLine[2]->isExit()) break;
		for(int i=3;i>=0;i--)
			if (flowLine[i] != nullptr && flowLine[i + 1] == nullptr) {
				if (i == 0 && flowLine[i]->conflict()) continue;
				flowLine[i]->execute();
				if (i == 2 && stay(flowLine[i]->type) && flowLine[i]->stage < 5) continue;
				flowLine[i + 1] = flowLine[i];
				flowLine[i] = nullptr;
			}
		if (flowLine[0] == nullptr && !Register.isPcLocked()) {
			flowLine[0] = decode(getCode());
			Register.nextpc();
		}
		if (flowLine[4] != nullptr) {
			delete flowLine[4];
			flowLine[4] = nullptr;
		}
	}
}

unit Simulator::getCode() {
	return Memory.getCode(Register.getpc());
}


ISA_base* Simulator:: decode(unsigned int code) {
	formatType type = getFormatType(code & 127u);
	//	std::cout << "format code : " << type << std::endl;
	//std::cout << "decode: " << code << "\n";
	switch (type)
	{
	case R_TYPE: return new R_type(code);
	case I_TYPE: return new I_type(code);
	case S_TYPE: return new S_type(code);
	case B_TYPE: return new B_type(code);
	case U_TYPE: return new U_type(code);
	case J_TYPE: return new J_type(code);
	default:
		break;
	}
}

bool Simulator::stay(const opType &type) {
	return type == SW || type == SB || type == SH ||
		type == LH || type == LHU || type == LBU || type == LB || type == LW;
}
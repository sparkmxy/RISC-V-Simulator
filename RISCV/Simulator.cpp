#include "pch.h"

#include "Simulator.h"

Simulator::Simulator(std::istream &_is,std::ostream &_os): is(_is),os(_os){
	ISA_base::linkMemoryAndRegisters(&Memory, &Register);
}


Simulator::~Simulator(){}

void Simulator::run() {
	Memory.init(is);
	Register.init();
	ISA_base *flowLine[5];
	for (int i = 0; i < 5; i++) flowLine[i] = nullptr;
	/*

	*/
	while (true) {
		/*
		Register.show();
		for (int i = 0; i < 4; i++)
			if (flowLine[i] == nullptr) std::cout << "null ";
			else std::cout << flowLine[i]->code << " ";
		std::cout << '\n';
		*/
		if (flowLine[2] != nullptr && flowLine[2]->isExit()) break;
		for(int i=3;i>=0;i--)
			if (flowLine[i] != nullptr && flowLine[i + 1] == nullptr) {
				if (i == 0 && flowLine[i]->conflict()) continue;
				flowLine[i]->execute();
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



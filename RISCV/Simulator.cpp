#include "pch.h"

#include "Simulator.h"

Simulator::Simulator(std::istream &_is,std::ostream &_os): is(_is),os(_os){
	ISA_base::linkMemoryAndRegisters(&Memory, &Register);
}


Simulator::~Simulator(){}

void Simulator::run() {
	Memory.init(is);
	Register.init();
	while (true) {
	//	Register.show();
		auto code = getCode();
	//	std::cout << "code = " << std::hex <<code << std::endl;
		Register.nextpc();
		if (code == exitCode) break;
		ISA_base *operation = decode(code);
	//	std::cout << "optype: " <<std::dec<< operation->Type() << std::endl;
		for (int i = 0; i < 4; i++) operation->execute();
		delete operation;
	}
}

unit Simulator::getCode() {
	return Memory.getCode(Register.getpc());
}



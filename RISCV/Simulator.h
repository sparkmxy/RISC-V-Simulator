#pragma once

#include "memory.h"
#include "registerManager.h"
#include "ISA.h"
#include <iostream>

class Simulator
{
	memory Memory;
	registerManager Register;
	std::ostream &os;
	std::istream &is;

public:
	Simulator(std::istream &_is = std::cin,std::ostream &_os = std::cout);
	
	void run();
	void answer() {os << std::dec << (Register[10] & 255u) << std::endl;}

	~Simulator();
private:
	unit getCode();

	ISA_base* decode(unsigned int code);
};


#include "pch.h"
#include "ISA_base.h"
#include "ISA.h"
#include "tool.h"


ISA_base* decode(unsigned int code) {
	formatType type = getFormatType(code & 127u);
//	std::cout << "format code : " << type << std::endl;
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
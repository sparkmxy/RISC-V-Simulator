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
		break;
	case I_TYPE: return new I_type(code);
		break;
	case S_TYPE: return new S_type(code);
		break;
	case B_TYPE: return new B_type(code);
		break;
	case U_TYPE: return new U_type(code);
		break;
	case J_TYPE: return new J_type(code);
		break;
	default:
		break;
	}
}
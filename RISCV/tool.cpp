#include "pch.h"
#include "tool.h"


formatType getFormatType(const unsigned int &num) {
	switch (num)
	{
	case 51: return R_TYPE;
	case 99: return B_TYPE;
	case 35: return S_TYPE;
	case 3 : case 103: case 19: return I_TYPE;
	case 111: return J_TYPE;
	case 55: case 23: return U_TYPE;
	default:
		break;
	}
}

unsigned int getInterval(const unsigned int &code, const int &l, const int &r) {
	if (r == 31) return code >> l;
	return (code & ((1u << (r+1)) - 1u) ) >> l;
}

unsigned int getBit(const unsigned int &code, const int &p) {
	return (code >> p) & 1;
}

#pragma once

#include "memory.h"
#include "registerManager.h"
#include "tool.h"

class ISA_base
{
	static const unsigned int exitCode = 0x00c68223;
protected:
	opType type;
	int stage;
	
	memory &mem;
	registerManager &RM;

	static memory *memptr;
	static registerManager *RMptr;

public:
	unsigned int code;
	ISA_base(const unsigned int &_code) :
		mem(*memptr),RM(*RMptr),code(_code),stage(0) {}

	static void linkMemoryAndRegisters(memory *mptr,registerManager *rptr) {
		memptr = mptr;
		RMptr = rptr;
	}

	virtual void execute() = 0;
	virtual bool conflict() { return false; }
	virtual ~ISA_base() {}
	opType Type() { return type; }
	bool isExit() {
		return code == exitCode;
	}
};

#pragma once

#include "ISA_base.h"

using unit = unsigned int;

class R_type :public ISA_base {
	int rs1,rs2,rd;
	unit buf[3];

public:
	R_type(const unit &_code);

	void execute();
	bool conflict();
private:
	opType getOpType();
};


class S_type :public ISA_base{
	int rs1, rs2;
	int offset;
	unit buf[2];
public:
	S_type(const unit &_code);

	void execute();
	bool conflict();

private:
	opType getOpType();
};

class B_type :public ISA_base {
	int rs1, rs2;
	int offset;
	bool taken,pre;
	unit buf[2],pc;

public:
	B_type(const unit &_code,const unit &_pc);
	void execute();
	bool conflict();
private:
	opType getOpType();
	bool predict();
};

class I_type :public ISA_base {
	int rs1, rd;
	unit imm;
	unit addr, oldpc, buf;
	short offset;
public:
	I_type(const unit &_code);
	void execute();
	bool conflict();
private:
	opType getOpType();
	void executeLoad();
	void executeRIOperation();
	void excuteJALR();
};

class J_type :public ISA_base {
	int offset, rd;
	unit buf;
public:
	J_type(const unit &_code);
	void execute();
};

class U_type :public ISA_base {
	unit imm,rd,buf;
public:
	U_type(const unit &_code);
	void execute();
	bool conflict();
};

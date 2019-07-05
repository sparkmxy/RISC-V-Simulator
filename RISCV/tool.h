#pragma once

enum  opType
{
	ADD, SUB, SLL, SLT, SLTU, XOR, SRL, SRA, OR, AND,    // R_type format;
	BEQ, BNE, BLT, BGE, BLTU, BGEU, // B_type format
	SB, SH, SW, //S_type format
	LB, LH, LW, LBU, LHU,
	ADDI, SLTI, SLTIU, XORI, ORI, ANDI, // I_type format
	SLLI, SRLI, SRAI, //I_type format (specail)
	LUI, AUIPC,   // U_type format
	JALR,     //J_type format
	JAL,
	WRONG_BRUNCH
};

enum formatType {
	R_TYPE, 
	I_TYPE, 
	S_TYPE,
	B_TYPE,
	U_TYPE,
	J_TYPE,
};

formatType getFormatType(const unsigned int &num);

unsigned int getInterval(const unsigned int &code, const int &l, const int &r); // 取出<code>的右起<l>位到<r>位的内容

unsigned int getBit(const unsigned int &code, const int &p);

struct signed_21_bit{
	int val : 21;
};

struct signed_12_bit {
	int val : 12;
};

struct signed_13_bit {
	int val : 13;
};

struct signed_20_bit {
	int val : 20;
};

struct unsigned_16_bit {
	unsigned int val : 16;
};

struct unsigned_8_bit {
	unsigned int val : 8;
};

struct unsigned_12_bit {
	unsigned int val : 12;
};
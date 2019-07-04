
#include "pch.h"
#include "ISA.h"
#include "tool.h"

memory *ISA_base::memptr = nullptr;
registerManager *ISA_base::RMptr = nullptr;

/*R_type*/
opType R_type::getOpType() {
	auto func3 = getInterval(code, 12, 14);
	auto func7 = getInterval(code, 25, 31);
	switch (func3)
	{
	case 0: return func7 ? SUB : ADD;
	case 1: return SLL;
	case 2: return SLT;
	case 3: return SLTU;
	case 4: return XOR;
	case 5: return func7 ? SRA : SRL;
	case 6: return OR;
	case 7: return AND;
	default:
		break;
	}
}

R_type::R_type(const unit &_code):ISA_base(_code){
	type = getOpType();
	rs1 = getInterval(code,15,19);
	rs2 = getInterval(code,20,24);
	rd = getInterval(code,7,11);
}


void R_type::execute() {
	switch (stage)
	{
	case 0:
		buf[0] = RM[rs1];
		buf[1] = RM[rs2];
		RM.lock(rd);
		break;
	case 1:
		if (type == ADD) buf[2] = buf[0] + buf[1];
		else if (type == SUB) buf[2] = buf[0] - buf[1];
		else if (type == SLL) buf[2] = buf[0] << (buf[1] & 31u);
		else if (type == SRL) buf[2] = buf[0] >> (buf[1] & 31u);
		else if (type == SLT) buf[2] = (static_cast<int>(buf[0]) < static_cast<int>(buf[1]));
		else if (type == SLTU) buf[2] = buf[0] < buf[1];
		else if (type == AND) buf[2] = buf[0] & buf[1];
		else if (type == OR) buf[2] = buf[0] | buf[1];
		else if (type == XOR) buf[2] = buf[0] ^ buf[1];
		else if (type == SRA) buf[2] = static_cast<int>(buf[0]) >> (buf[1] & 31u);
		//std::cout << "result: " << buf[2] << '\n';
		break;
	case 3:
		RM[rd] = buf[2];
		RM.unlock(rd);
		break;
	default:
		break;
	}
	stage++;
}

bool R_type::conflict() {
	return RM.islocked(rs1) || RM.islocked(rs2);
}

/*B_type*/

opType B_type::getOpType() {
	auto func3 = getInterval(code, 12, 14);
	switch (func3)
	{
	case 0: return BEQ;
	case 1: return BNE;
	case 4: return BLT;
	case 5: return BGE;
	case 6: return BLTU;
	case 7: return BGEU;
	}
}

B_type::B_type(const unit &_code) :ISA_base(_code){
	type = getOpType();
	rs1 = getInterval(code, 15, 19);
	rs2 = getInterval(code, 20, 24);
	offset = signed_13_bit{ static_cast<int>(
		(getBit(code,31) << 12) |
		(getBit(code,7) << 11) |
		(getInterval(code,25,30) << 5) |
		(getInterval(code,8,11) << 1) ) }.val;
}

bool B_type::conflict() {
	return RM.islocked(rs1) || RM.islocked(rs2);
}

void B_type::execute() {
	switch (stage)
	{
	case 0:
		buf[0] = RM[rs1];
		buf[1] = RM[rs2];
		RM.lockpc();
		break;
	case 1:
		if (type == BEQ) taken = (buf[0] == buf[1]);
		else if (type == BNE) taken = (buf[0] != buf[1]);
		else if (type == BLT) taken = static_cast<int>(buf[0]) < static_cast<int>(buf[1]);
		else if (type == BLTU) taken = buf[0] < buf[1];
		else if (type == BGE) taken = static_cast<int>(buf[0]) >= static_cast<int>(buf[1]);
		else if (type == BGEU) taken = buf[0] >= buf[1];
		break;
	case 2:
		if (taken) RM.movepc(offset-4);
		RM.unlockpc();
		break;
	case 3:
	default:
		break;
	}
	stage++;
}

/*S_type*/

opType S_type::getOpType() {
	auto func3 = getInterval(code, 12, 14);
	switch (func3)
	{
	case 0: return SB;
	case 1: return SH;
	case 2: return SW;
	}
}

S_type::S_type(const unit &_code):ISA_base(_code) {
	type = getOpType();
	rs1 = getInterval(code, 15, 19);
	rs2 = getInterval(code, 20, 24);
	offset = signed_12_bit{ static_cast<int>((getInterval(code, 25, 31) << 5) |
			getInterval(code,7,11)) }.val;
}

bool S_type::conflict() {
	return RM.islocked(rs1) || RM.islocked(rs2);
}

void S_type::execute() {
	switch (stage)
	{
	case 0:
		buf[0] = RM[rs1] + offset;
		buf[1] = RM[rs2];
		break;
	case 2:
		if (type == SB)mem.store(buf[0],buf[1]&((1u<<8)-1),8);
		else if (type == SH) mem.store(buf[0], buf[1] & ((1u << 16)-1), 16);
		else if (type == SW) mem.store(buf[0], buf[1]);
	default:
		break;
	}
	stage++;
}

/*I_type*/
opType I_type::getOpType() {
	auto opnum = code & 127u;
	auto func3 = getInterval(code, 12, 14);
	auto func7 = getInterval(code, 25, 31);
	if (opnum == 103) return JALR;
	else if (opnum == 19) {
		switch (func3)
		{
		case 0: return ADDI;
		case 1: return SLLI;
		case 2: return SLTI;
		case 3: return SLTIU;
		case 4: return XORI;
		case 5: return func7 ? SRAI : SRLI;
		case 6: return ORI;
		case 7: return ANDI;
		}
	}
	else {
		switch (func3)
		{
		case 0: return LB;
		case 1: return LH;
		case 2: return LW;
		case 3: return LBU;
		case 4: return LHU;
		}
	}
}

I_type::I_type(const unit &_code) :ISA_base(_code) {
	type = getOpType();
	rs1 = getInterval(code, 15, 19);
	if (type == SLLI || type == SRLI || type == SRLI)
		imm = getInterval(code,20,24);
	else if (type == JALR) {
		offset = signed_12_bit{ static_cast<int>(getInterval(code, 20, 31)) }.val;
	}
	else imm = getInterval(code, 20, 31);
	rd = getInterval(code, 7, 11);
	//std::cout << "I type imm = " << (type == JALR ? offset: imm )<< std::endl;
	//std::cout << "REGSTER: " << rs1<<" "<<RM[rs1];
}

void I_type::execute() {
	if ((code & 127u) == 19) executeRIOperation();
	else if (type == JALR) excuteJALR();
	else executeLoad();
}

void I_type::executeRIOperation() {
	switch (stage)
	{
	case 0: RM.lock(rd); buf = RM[rs1]; break;
	case 1:
		if (type == ADDI) buf = (int)buf + signed_12_bit{ static_cast<int>(imm) }.val;
		else if (type == SLTI) buf = (int) buf < signed_12_bit{ static_cast<int>(imm) }.val;
		else if (type == SLTIU) buf = buf < imm;
		else if (type == ANDI) buf &= signed_12_bit{ static_cast<int>(imm) }.val;
		else if (type == ORI) buf |= signed_12_bit{ static_cast<int>(imm) }.val;
		else if (type == XORI) buf ^= signed_12_bit{ static_cast<int>(imm) }.val;
		else if (type == SLLI) buf <<= imm;
		else if (type == SRLI) buf >>= imm;
		else if (type == SRAI) buf = static_cast<int>(buf) >> imm;
		break;
	case 3:
		RM[rd] = buf;
		RM.unlock(rd);
		break;
	default:
		break;
	}
	stage++;
}

void I_type::excuteJALR() {
	switch (stage)
	{
	case 0: RM.lock(rd); buf = RM[rs1]; break;
	case 1: 
		oldpc = RM.getpc();
		buf = RM[rs1] + offset;
		buf >>= 1; buf <<= 1;
		break;
	case 2: 
		RM.setpc(buf);
		RM.unlockpc();
	default:
		RM[rd] = oldpc; // + 4 ? 
		RM.unlock(rd);
		break;
	}
	stage++;
}

void I_type::executeLoad() {
	switch (stage)
	{
	case 0:
		addr = RM[rs1] + signed_12_bit{ static_cast<int>(imm) }.val;
		RM.lock(rd);
		break;
	case 2:
		if (type == LB) buf = mem.loadAsInt(addr, 8);
		else if (type == LH) buf = mem.loadAsInt(addr, 16);
		else if (type == LW) buf = mem.load(addr);
		else if (type == LBU) buf = mem.load(addr,8);
		else if (type == LHU) buf = mem.load(addr,16);
		break;
	case 3:
		RM[rd] = buf;
		break;
	default:
		break;
	}
	stage++;
}

bool I_type::conflict() {
	return RM.islocked(rs1) || (type == JALR && RM.isPcLocked());
}

/*J_type*/
J_type::J_type(const unit &_code):ISA_base(_code) {
	rd = getInterval(code, 7, 11);
	type = JAL;
	offset = signed_21_bit{static_cast<int>(
		(getBit(code, 31) << 20) | (getInterval(code, 12, 19) << 12)
		| (getBit(code, 20) << 11) | (getInterval(code, 21, 30) << 1)
		)}.val;
//	std::cout << "offset = " << std::dec << offset << std::endl;
}

void J_type::execute() {
	switch (stage)
	{
	case 1: RM.lockpc(); break;// stage 0 or 1?
	case 2:
		buf = RM.getpc();
		RM.movepc(offset-4);
		RM.unlockpc();
		break;
	case 3:
		RM[rd] = buf;
		break;
	default:
		break;
	}
	stage++;
}

/*U_type*/
U_type::U_type(const unit &_code) :ISA_base(_code) {
	auto opcode = code & (127u);
	type = opcode == 55 ? LUI : AUIPC;
	rd = getInterval(code, 7, 11);
	imm = getInterval(code,12,31);
	//std::cout << "U-type: IMM = " << imm << " code = " << code << std::endl;
}

void U_type::execute() {
	switch (stage)
	{
	case 0: 
		RM.lock(rd); 
		buf = RM.getpc();
		break;
	case 1:
		if (type == AUIPC) buf += signed_20_bit{ static_cast<int>(imm) }.val;
		else if (type == LUI) buf = imm << 12;
	case 3: RM[rd] = buf; RM.unlock(rd);  break;
	default:
		break;
	}
	stage++;
}

bool U_type::conflict() {
	return type == AUIPC && RM.isPcLocked();
}


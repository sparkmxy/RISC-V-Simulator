#include "pch.h"
#include "memory.h"
#include "sstream"
#include "tool.h"

memory::memory(){
	std::memset(pool,0,sizeof(pool));
}

memory::~memory(){}


void memory::init(std::istream &is) {
	char ch;
	string temp;
	Address addr;
	is >> temp;
	addr = toHex(temp.substr(1,temp.length()-1));
	while (!is.eof()) {
		int cnt = 0;
		while (!is.eof()) {
			is >> temp;
			if (temp[0] == '@') {
				addr = toHex(temp.substr(1, temp.length() - 1));
				break;
			}
			else {
				pool[addr + cnt] = toHex(temp);
				cnt++;
			}
		}
	}
	//std::cout << "Initailize finished." << std::endl;
}

memory::unit memory::load(const Address &addr,int bit) const {
	//std::cout << "load: " << addr << " " << bit <<std:: endl;
	if(bit == 32)return *reinterpret_cast<const unit *>(pool + addr);
	else if (bit == 16) return unsigned_16_bit{ (unit)*reinterpret_cast<const short *>(pool + addr) }.val;
	else if (bit == 8) return unsigned_8_bit{ (unit)*reinterpret_cast<const char *>(pool + addr) }.val;
}

void memory::store(const Address &addr, const unit &code,int bit) {
	if(bit == 32) *reinterpret_cast<unit *>(pool + addr) = code;
	else if(bit == 16) *reinterpret_cast<short *>(pool + addr) = code;
	else if (bit == 8) pool[addr] = code;
}

int memory::memory::loadAsInt(const Address &addr,const int &bit) const {
	if (bit == 16) return *reinterpret_cast<const short *>(pool + addr);
	else if (bit == 8)return pool[addr];
}

unsigned int memory::getCode(const Address &addr) {
	//std::cout << "address = " << std::hex<<addr << std::endl;
	return *reinterpret_cast<unsigned int *>(pool+addr);
}
#pragma once
#include <iostream>
#include <cstring>
#include <string>
#include <sstream>

using std::string;
class memory
{
	//range: from 0x00000 to 0x20000
	static const int SIZE = 140000;
	using byte = char;
	using unit = unsigned int;
	using Address = unsigned int;
	
	byte pool[SIZE];

public:
	memory();
	void init(std::istream &is);

	unit load(const Address &addr,int bit = 32) const ;
	int loadAsInt(const Address &addr, const int &bit) const;
	void store(const Address &addr, const unit &code,int bit = 32);
	unit getCode(const Address &addr);

	~memory();
private:
	unit toHex(const string &s) {
		unit x;
		std::stringstream ss(s);
		ss >> std::hex >> x;
		//std::cout << "toHex :" << s << "return with " << x << std::endl;
		return x;
	}
};


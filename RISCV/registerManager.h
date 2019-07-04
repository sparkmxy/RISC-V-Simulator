#pragma once
#include <cstring>
class registerManager
{
	static const int SIZE = 32;
	using unit = unsigned int;
	
	unit r[SIZE];
	unit pc,x0Shadow;

	int locked[SIZE];
	int pclock;

public:
	registerManager() :pclock(false) {
		std::memset(r, 0, sizeof(r));
		std::memset(locked, 0, sizeof(locked));
	}
	~registerManager() {};

	void init() {pc = 0;}

	const unit &operator [](int idx) const { return r[idx]; }
	unit &operator [] (int idx) { 
		if(idx) return r[idx]; 
		else return x0Shadow = 0;
	}
	unit getpc() const { return pc; }
	void movepc(const int &offset) { pc += offset; }
	void setpc(const unit &addr) { pc = addr; }


	void lockpc() { pclock++; }
	void unlockpc() { pclock--; }
	void lock(int idx) { locked[idx]++; }
	void unlock(int idx) { locked[idx]--; }
	bool islocked(int idx) { return locked[idx]; }
	bool isPcLocked() { return pclock; }

	void nextpc() { pc += 4; }

	void show() {
		std::cout << "pc = " << std::hex << pc << ' '<<pclock<<'\n';
		for (int i = 0; i < 32; i++) std::cout << locked[i] << ' '; std::cout << '\n';
	}
};


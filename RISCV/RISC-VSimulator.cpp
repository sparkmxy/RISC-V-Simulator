// RISC-VSimulator.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

#include "pch.h"
#include <iostream>
#include "Simulator.h"
#include <fstream>

int main(){
	std::ios_base::sync_with_stdio(0);
	Simulator simulator;
	simulator.run();
	simulator.answer();
	return 0;
}

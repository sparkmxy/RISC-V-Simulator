﻿// RISC-VSimulator.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。


#include "pch.h"
#pragma warning(disable : 4996)
#include "Simulator.h"
#include <fstream>
#include <iostream>
#include <cstdlib>

int main(){
	
	std::ios_base::sync_with_stdio(0);
	std::srand(233);
	//freopen("ans.out", "w", stdout);
	//std::ifstream file("hanoi.in");
	Simulator simulator;
	simulator.run();
	simulator.answer();
	return 0;
}

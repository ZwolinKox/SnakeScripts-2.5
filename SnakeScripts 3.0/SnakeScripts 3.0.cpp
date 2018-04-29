// SnakeScripts 3.0.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "SnakeScript.h"
#include <time.h>

using namespace std;

int func(SnakeScript* skrypt)
{
	std::cout << "TO DZIALA HAHAHA" << std::endl;
	
	register int x = 0;

	for (auto& i : skrypt->CPP_ARGC)
		x += i.value;

	return x;
}

int main()
{

	srand(time(NULL)); //Do liczb pseudolosowych

	SnakeScript skrypt{ "Skrypt.sn" };

	skrypt.registerFunc(func, "funkcja", 3);

	skrypt.run();

	std::vector<std::string> vs;
	vs.push_back("10");

	skrypt.runProc("xd", vs);
	skrypt.runMethod("metoda", "obj", vs);



	system("pause > nul");

    return 0;
}


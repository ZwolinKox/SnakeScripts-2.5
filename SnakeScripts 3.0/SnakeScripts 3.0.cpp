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

	skrypt.run();

	system("pause > nul");

	return 0;
}


// SnakeScripts 3.0.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "SnakeScript.h"

using namespace std;

int main()
{
	SnakeScript skrypt{ "Skrypt.sn" };
	skrypt.run();

	system("pause > nul");

    return 0;
}


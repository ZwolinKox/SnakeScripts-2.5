#include "stdafx.h"
#include <iostream>
#include "SnakeScript.h"
#include <time.h>

using namespace std;


int main()
{
	srand(time(NULL)); //Do liczb pseudolosowych

	SnakeScript skrypt{ "Skrypt.sn" };

	skrypt.run();

	system("pause > nul");

	return 0;
}


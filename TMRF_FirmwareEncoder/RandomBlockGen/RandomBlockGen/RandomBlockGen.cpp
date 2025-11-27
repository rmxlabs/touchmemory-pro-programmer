// RandomBlockGen.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>

#define RANDOM_BLOCK_SIZE		150

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	ofstream fout("rand_block.txt");


	fout << "unsigned char RandomBlock[" << RANDOM_BLOCK_SIZE << "] = {";

	clock_t t = clock();
	srand(t);

	for(int i = 0; i < RANDOM_BLOCK_SIZE; i++)
	{
		if( i % 16 == 0 ) fout << endl << '\t';
		fout << rand() % 256 << ", ";
	}

	fout << " };" << endl;

	fout.close();
	cout << "OK!" << endl;
	return 0;
}


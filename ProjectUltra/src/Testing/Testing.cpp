#include <iostream>
#include <vector>
#include <array>

#include "Testing.h"
#include "SelfTest.h"
#include "LoopsTest.h"
#include "../UI/UI.h"
#include "../Enigma/Enigma.h"
#include "Benchmark.h"

using namespace std;

void testing()
{
	vector<char> options;
	char option;
	cout << "Select option S - Self test / L - Loops test / B - Benchmark: ";
	options = { 'S', 'L', 'B' };
	option = getOption(options);
	switch (option)
	{
	case 'S':
		selfTest();
		break;
	case 'L':
		loopsTest();
		break;
	case 'B':
		benchmark();
	}
}
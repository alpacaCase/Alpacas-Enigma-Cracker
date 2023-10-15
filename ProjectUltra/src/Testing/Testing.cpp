#include <iostream>
#include <vector>
#include <array>

#include "Testing.h"
#include "SelfTest.h"
#include "CircuitsTest.h"
#include "../UI/UI.h"
#include "../Enigma/Enigma.h"
#include "CipherBenchmark.h"

using namespace std;

void testing()
{
	vector<char> options;
	char option;
	cout << "Select option S - Self test / C - Ciphertext only benchmark / L - Circuits test: ";
	options = { 'S', 'C', 'L' };
	option = getOption(options);
	switch (option)
	{
	case 'S':
		selfTest();
		break;
	case 'C':
		cipherBenchmark();
		break;
	case 'L':
		circuitsTest();
		break;
	}
}
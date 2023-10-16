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

void testing(logbook& record)
{
	vector<char> options;
	char option;
	cout << "Select option D - Toggle debug mode / S - Self test / C - Ciphertext only benchmark / L - Circuits test: ";
	options = { 'D', 'S', 'C', 'L' };
	option = getOption(options);
	switch (option)
	{
	case 'D':
		if (record.printingLevel == 'L')
		{
			record.printingLevel = 'D';
			cout << "\nDebug mode enabled\n";
		}
		else
		{
			record.printingLevel = 'L';
			cout << "\nDebug mode disabled\n";
		}
		break;
	case 'S':
		selfTest(record);
		break;
	case 'C':
		cipherBenchmark(record);
		break;
	case 'L':
		circuitsTest(record);
		break;
	}
}
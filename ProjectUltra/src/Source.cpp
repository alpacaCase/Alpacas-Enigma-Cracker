#include <iostream>
#include <vector>
#include <string>
#include <array>

#include "UI/UI.h"
#include "Enigma/EnigmaEmulator.h"
#include "CipherAttack/CipherAttack.h"
#include "BombeAttack/BombeAttack.h"
#include "Testing/Testing.h"

using namespace std;

int main()
{
	cout << "Project Ultra V0.2\n";
	vector<char> options;
	char option;
	while (true)
	{
		cout << "\nSelect option E - Enigma emulator / B - Bombe style attack / C - Ciphertext only style attack / T - Testing: ";
		options = { 'E', 'B', 'C', 'T' };
		option = getOption(options);
		switch (option)
		{
		case 'E':
			enigmaEmulator();
			break;
		case 'B':
			bombeAttack();
			break;
		case 'C':
			cipherAttack();
			break;
		case 'T':
			testing();
			break;
		}

		//Repeat?
		cout << "Perform another operation Y/N? ";
		options = { 'Y', 'N' };
		option = getOption(options);
		if (option == 'N') break;
	}
	return 0;
}
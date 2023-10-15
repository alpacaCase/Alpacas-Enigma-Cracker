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
	cout << "Alpacas Enigma Cracker V0.3\n";
	vector<char> options;
	char option;
	while (true)
	{
		cout << "\nSelect option E - Enigma emulator / C - Ciphertext only style attack / K - Known plaintext attack / T - Testing: ";
		options = { 'E', 'C', 'K', 'T' };
		option = getOption(options);
		switch (option)
		{
		case 'E':
			enigmaEmulator();
			break;
		case 'C':
			cipherAttack();
			break;
		case 'K':
			bombeAttack();
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
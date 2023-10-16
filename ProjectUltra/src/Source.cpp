#include <iostream>
#include <vector>
#include <string>
#include <array>

#include "UI/UI.h"
#include "UI/Log.h"
#include "Enigma/EnigmaEmulator.h"
#include "CipherAttack/CipherAttack.h"
#include "BombeAttack/BombeAttack.h"
#include "Testing/Testing.h"

using namespace std;

int main()
{
	cout << "Alpaca's Enigma Cracker V0.3.2\n";

	logbook record;

	vector<char> options;
	char option;
	while (true)
	{
		cout << "\nSelect option E - Enigma emulator / C - Ciphertext only attack / K - Known plaintext attack / T - Testing: ";
		options = { 'E', 'C', 'K', 'T' };
		option = getOption(options);
		switch (option)
		{
		case 'E':
			enigmaEmulator(record);
			break;
		case 'C':
			cipherAttack(record);
			break;
		case 'K':
			bombeAttack(record);
			break;
		case 'T':
			testing(record);
			break;
		}
	}
	return 0;
}
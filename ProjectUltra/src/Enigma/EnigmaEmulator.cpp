#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "EnigmaEmulator.h"
#include "Enigma.h"
#include "../UI/UI.h"

using namespace std;

void enigmaEmulator(logbook& record)
{
	cout << "\nENIGMA EMULATOR\n\n";
	cout << "Rotor types are given as numbers [1,8], all settings should be given as letters or numbers [0,25]\n\n";
	
	//Load plaintext
	string plaintext;
	cout << "Select option F - Load plaintext from file / I - Input plaintext manually: ";
	vector<char> options = { 'F', 'I' };
	char option = getOption(options);
	if (option == 'F')
	{
		cout << "Enter filename: ";
		plaintext = getUserFile();
	}
	else
	{
		cout << "Enter plaintext followed by $:\n";
		getline(cin, plaintext, '$');
	}
	vector<int> plainnumbers = stringToNumbers(plaintext);

	//Settings
	enigmaSetting setting;

	//Reflector
	cout << "\nEnter reflector type (lower case indicates thin relflector) [A/B/C/b/c]: ";
	options = { 'A', 'B' , 'C', 'b', 'c' };
	setting.reflector[0] = getOption(options, false);
	if (setting.reflector[0] == 'b' || setting.reflector[0] == 'c')
	{
		cout << "Enter extra rotor type B - Beta / G - Gamma: ";
		options = { 'B', 'G' };
		setting.reflector[1] = getOption(options);
		if (setting.reflector[1] == 'B') setting.reflector[1] = 'T';
	}

	//Rotor
	cout << "Enter left rotor type:                       ";
	setting.rotors[0][0] = getNumber(1, 9);
	cout << "Enter middle rotor type:                     ";
	setting.rotors[1][0] = getNumber(1, 9);
	cout << "Enter right rotor type:                      ";
	setting.rotors[2][0] = getNumber(1, 9);
	if (setting.reflector[0] == 'b' || setting.reflector[0] == 'c')
	{
		cout << "Enter extra rotor start position:            ";
		setting.reflector[2] = getSetting();
	}
	cout << "Enter left rotor start position:             ";
	setting.rotors[0][1] = getSetting();
	cout << "Enter middle rotor start position:           ";
	setting.rotors[1][1] = getSetting();
	cout << "Enter right rotor start position:            ";
	setting.rotors[2][1] = getSetting();
	if (setting.reflector[0] == 'b' || setting.reflector[0] == 'c')
	{
		setting.reflector[3] = 0;
	}
	cout << "Enter left rotor ring setting:               ";
	setting.rotors[0][2] = getSetting();
	cout << "Enter middle rotor ring setting:             ";
	setting.rotors[1][2] = getSetting();
	cout << "Enter right rotor ring setting:              ";
	setting.rotors[2][2] = getSetting();

	//Plug
	cout << "Enter number of plug pairs:                  ";
	int numberOfPlugs = getNumber(0,14);
	while (numberOfPlugs > 0)
	{
		cout << "Enter a new plug pair:                       ";
		getPlug(setting.plug);
		numberOfPlugs--;
	}

	//Encrypt
	cout << "\nEncrypting\n\n";
	enigma machine;
	machine.set(setting);
	machine.code(plainnumbers);
	string result = numberVectorToString(plainnumbers);
	result = formatForOutput(result);

	//Output
	string filename = "ciphertext.txt";
	cout << "Encrypted text is:\n" << result;
	saveFile(filename, result);
	cout << "\n\nSaved text as " << filename << "\n\n";
}
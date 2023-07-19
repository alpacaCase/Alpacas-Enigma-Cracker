#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "EnigmaEmulator.h"
#include "Enigma.h"
#include "../UI/UI.h"

using namespace std;

void enigmaEmulator()
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

	//Reflector settings
	int reflectorSetting[4];
	cout << "\nEnter reflector type (lower case indicates thin relflector) [A/B/C/b/c]: ";
	options = { 'A', 'B' , 'C', 'b', 'c' };
	reflectorSetting[0] = getOption(options, false);
	if (reflectorSetting[0] == 'b' || reflectorSetting[0] == 'c')
	{
		cout << "Enter extra rotor type B - Beta / G - Gamma: ";
		options = { 'B', 'G' };
		reflectorSetting[1] = getOption(options);
		if (reflectorSetting[1] == 'B') reflectorSetting[1] = 'T';
	}

	//Rotor settings
	int rotorSetting[3][3];
	cout << "Enter left rotor type:                       ";
	rotorSetting[0][0] = getNumber(1, 9);
	cout << "Enter middle rotor type:                     ";
	rotorSetting[1][0] = getNumber(1, 9);
	cout << "Enter right rotor type:                      ";
	rotorSetting[2][0] = getNumber(1, 9);
	if (reflectorSetting[0] == 'b' || reflectorSetting[0] == 'c')
	{
		cout << "Enter extra rotor start position:            ";
		reflectorSetting[2] = getSetting();
	}
	cout << "Enter left rotor start position:             ";
	rotorSetting[0][1] = getSetting();
	cout << "Enter middle rotor start position:           ";
	rotorSetting[1][1] = getSetting();
	cout << "Enter right rotor start position:            ";
	rotorSetting[2][1] = getSetting();
	if (reflectorSetting[0] == 'b' || reflectorSetting[0] == 'c')
	{
		cout << "Enter extra rotor ring setting:              ";
		reflectorSetting[3] = getSetting();
	}
	cout << "Enter left rotor ring setting:               ";
	rotorSetting[0][2] = getSetting();
	cout << "Enter middle rotor ring setting:             ";
	rotorSetting[1][2] = getSetting();
	cout << "Enter right rotor ring setting:              ";
	rotorSetting[2][2] = getSetting();

	//Plug settings
	vector<array<int, 2>> plug = { };
	cout << "Enter number of plug pairs:                  ";
	int plugNumber = getNumber(0,14);
	while (plugNumber > 0)
	{
		cout << "Enter a new plug pair:                       ";
		getPlug(plug);
		plugNumber--;
	}

	//Encrypt
	cout << "\nEncrypting\n\n";
	enigma machine;
	machine.initialise(rotorSetting, reflectorSetting, plug);
	machine.code(plainnumbers);
	string result = numberVectorToString(plainnumbers);
	result = formatForOutput(result);

	//Output
	string filename = "ciphertext.txt";
	cout << "Encrypted text is:\n" << result;
	saveFile(filename, result);
	cout << "\n\nSaved text as " << filename << "\n\n";
}
#include <iostream>

#include "CipherAttack.h"
#include "Crack.h"
#include "../UI/UI.h"

using namespace std;

void cipherAttack()
{
	vector<array<array<int, 2>, 3>> searchInstructions;
	vector<int> ciphernumbers;
	vector<int> reflectorPossibilities;
	vector<int> extraPossibilities;
	vector<int> rotorPossibilities;
	vector<array<int, 2>> startingPlugs = {};
	char logging = 'L';
	string plaintext;
	
	//Get ciphernumbers
	cout << "\nCIPHERTEXT ONLY ATTACK\n\n";
	cout << "Enter ciphertext filename: ";
	string ciphertext = getUserFile();
	ciphernumbers = stringToNumbers(ciphertext);

	//Get reflector possibilities
	cout << "\nSelect reflector possibilities B - B reflector only / C - C reflector only / D - B and C reflectors\nb - B-thin reflector only / c - C-thin reflector only  / d - B-thin and C-thin reflectors: ";
	vector<char> options = { 'B','C','D','b','c','d' };
	char option;
	option = getOption(options, false);
	switch (option)
	{
	case 'B':
		reflectorPossibilities = { 'B' };
		break;
	case 'C':
		reflectorPossibilities = { 'C' };
		break;
	case 'D':
		reflectorPossibilities = { 'B','C' };
		break;
	case 'b':
		reflectorPossibilities = { 'b' };
		break;
	case 'c':
		reflectorPossibilities = { 'c' };
		break;
	case 'd':
		reflectorPossibilities = { 'b','c' };
		break;
	}

	//Get extra rotor possibilities
	if (option > 90)
	{
		cout << "Select extra rotor possibilities B - Beta rotor only / G - Gamma rotor only / A - Both: ";
		options = { 'B','G','A' };
		option = getOption(options);
		switch (option)
		{
		case 'B':
			extraPossibilities = { 'T' };
			break;
		case 'G':
			extraPossibilities = { 'G' };
			break;
		case 'A':
			extraPossibilities = { 'T','G' };
			break;
		}
	}
	else
	{
		extraPossibilities = { 'T' };
	}

	//Get rotor possibilities
	cout << "Select rotor possibilities A - Only 3 rotors available / B - Only 5 rotors available / C - 8 rotors available: ";
	options = { 'A','B','C' };
	option = getOption(options);
	switch (option)
	{
	case 'A':
		rotorPossibilities = { 1, 2, 3 };
		break;
	case 'B':
		rotorPossibilities = { 1, 2, 3, 4, 5 };
		break;
	case 'C':
		rotorPossibilities = { 1, 2, 3, 4, 5, 6, 7, 8 };
		break;
	}

	//Get search instructions
	array<int, 2> tempArray;
	array<array<int, 2>, 3> tempBigArray;
	cout << "Select search type A - Fast / B - Standard / C - Two rounds: ";
	options = { 'A','B','C' };
	option = getOption(options);
	switch (option)
	{
	case 'A':
		tempArray = {'I', 1};
		tempBigArray[0] = tempArray;
		tempBigArray[1] = tempArray;
		tempBigArray[2] = tempArray;
		searchInstructions.push_back(tempBigArray);
		break;
	case 'B':
		tempArray = { 'I', 60 };
		tempBigArray[0] = tempArray;
		tempArray = { 'I', 12 };
		tempBigArray[1] = tempArray;
		tempArray = { 'I', 1 };
		tempBigArray[2] = tempArray;
		searchInstructions.push_back(tempBigArray);
		break;
	case 'C':
		tempArray = { 'I', 60 };
		tempBigArray[0] = tempArray;
		tempBigArray[1] = tempArray;
		tempArray = { 'I', 12 };
		tempBigArray[2] = tempArray;
		searchInstructions.push_back(tempBigArray);
		tempBigArray[0] = tempArray;
		tempArray = { 'I', 1 };
		tempBigArray[1] = tempArray;
		tempBigArray[2] = tempArray;
		searchInstructions.push_back(tempBigArray);
		break;
	}

	cout << "\n";

	plaintext = formatForOutput(numberVectorToString(crack(searchInstructions, ciphernumbers, reflectorPossibilities, extraPossibilities, rotorPossibilities, rotorPossibilities, rotorPossibilities, startingPlugs, logging)));

	cout << "Plaintext is:\n" << plaintext << "\n\n";
}
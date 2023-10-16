#include <string>
#include <iostream>

#include "BombeAttack.h"
#include "Bombe.h"
#include "CipherGraph.h"
#include "../UI/UI.h"

using namespace std;

/*
Function returns true if the two strings have the same character at any index
plaintext should be smaller than ciphertext
*/
bool checkCrash(string& plaintext, string& ciphertext)
{
	bool result = false;
	for (unsigned int i = 0; i < plaintext.length(); i++)
	{
		if (plaintext[i] != ' ' && plaintext[i] == ciphertext[i])
		{
			result = true;
			break;
		}
	}
	return result;
}

/*
Function pads the start of plaintext with spaces until there are no crashes with ciphertext
If force step is true the function will add at least one space to the start of plain text, otherewise if the two start without a crash function will make no changes
Throws error 31 if trys to pad plaintext to longer than ciphertext
*/
void findNextCribPosition(string& plaintext, string& ciphertext, bool forceStep = true)
{
	if (forceStep) plaintext.insert(0, " ");
	while (true)
	{
		if (plaintext.length() <= ciphertext.length())
		{
			if (!checkCrash(plaintext, ciphertext))
				return;
		}
		else 
			throw 31;
		plaintext.insert(0, " ");
	}
}

void bombeAttack(logbook& record)
{	
	vector<int> ciphernumbers;
	vector<int> plainnumbers;
	vector<int> reflectorPossibilities;
	vector<int> extraPossibilities;
	vector<int> rotorPossibilities;
	vector<char> options;
	char option;
	array<int, 2> instructions;
	instructions[0] = record.printingLevel;
	string plaintext;

	//Get ciphernumbers
	cout << "\nKnown plaintext attack\n\n";
	cout << "Enter ciphertext filename: ";
	string ciphertext = cleanString(getUserFile());

	//Get plaintext and find its position relative to ciphertext
	while (true)
	{
		cout << "Enter plaintext: ";
		cin >> plaintext;
		plaintext = cleanString(plaintext);

		try
		{
			findNextCribPosition(plaintext, ciphertext, false);
			while (true)
			{
				cout << ciphertext << endl;
				cout << plaintext << endl;
				cout << "A - Advance plaintext to next possible positions / K - Keep current position: ";
				options = { 'A', 'K' };
				option = getOption(options);
				if (option == 'K') break;
				findNextCribPosition(plaintext, ciphertext);
			}
			break;
		}
		catch (int i)
		{
			cout << "No more possible positions for plaintext, try again Y/N? ";
			options = { 'Y', 'N' };
			option = getOption(options);
			switch (option)
			{
			case 'Y':
				break;
			case 'N':
				return;
			}
		}
	}

	//Get reflector possibilities
	cout << "\nSelect reflector possibilities B - B reflector only / C - C reflector only / D - B and C reflectors\nb - B-thin reflector only / c - C-thin reflector only  / d - B-thin and C-thin reflectors: ";
	options = { 'B','C','D','b','c','d' };
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

	//Get extra rotors possibilities
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

	//Get rotors possibilities
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

	//Get search mode
	cout << "Run full search (takes 26x longer and only necessary if the first rotor steps during the crib) Y/N? ";
	options = { 'Y', 'N' };
	option = getOption(options);
	switch (option)
	{
	case 'Y':
		instructions[1] = 1;
		break;
	case 'N':
		instructions[1] = 0;
		break;
	}

	cout << "\n";

	//Run attack
	try
	{
		plainnumbers = bombe(instructions, plaintext, ciphertext, reflectorPossibilities, extraPossibilities, rotorPossibilities, rotorPossibilities, rotorPossibilities);

		plaintext = formatForOutput(numberVectorToString(plainnumbers));

		cout << "Plaintext is:\n" << plaintext << "\n\n";
	}
	catch (int error)
	{
		cout << "\nERROR\n";
		switch (error)
		{
		case 1:
			cout << "No loops were found\nTry using a longer crib\n\n";
			break;
		case 2:
			cout << "Too many possible settings were generated\nTry using a longer crib\n\n";
			break;
		case 3:
			cout << "No possible settings were generated\nIf you are sure the crib is correct try enabling full search or using a shorter crib\n\n";
		}
	}

}

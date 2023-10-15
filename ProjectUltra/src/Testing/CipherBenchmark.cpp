#include <iostream>
#include <vector>

#include "../UI/UI.h"
#include "TestingTools.h"
#include "../CipherAttack/Crack.h"
#include "../UI/Log.h"

using namespace std;

void cipherBenchmark()
{
	cout << "\nCipher Text Only Benchmark\n\n";
	cout << "This benchmark measures the performance, both speed and accuracy, of the ciphertext only attack\n";

	//Get cipherBenchmark settings
	cout << "Enter number of settings to generate: ";
	int n = getNumber(0, 10000);

	cout << "Select settings space A - Army or Airforce M3 / N - M3 naval enigma (i.e. with 8 possible rotors) / F - M4 four rotor enigma: ";
	std::vector<char> options = { 'A', 'N', 'F' };
	char option = getOption(options);
	
	vector<int> reflectorPossibilities;
	vector<int> extraPossibilities;
	vector<int> rotorPossibilities;
	vector<array<int, 2>> startingPlugs = {};

	switch (option)
	{
	case 'A':
		reflectorPossibilities = { 'B' };
		extraPossibilities = { 'T' };
		rotorPossibilities = { 1,2,3,4,5 };
		break;
	case 'N':
		reflectorPossibilities = { 'B' };
		extraPossibilities = { 'T' };
		rotorPossibilities = { 1,2,3,4,5,6,7,8 };
		break;
	case 'F':
		reflectorPossibilities = { 'b', 'c' };
		extraPossibilities = { 'T', 'G' };
		rotorPossibilities = { 1,2,3,4,5,6,7,8 };
		break;
	}
	
	cout << "Select number of plug pairs: ";
	int numberOfPlugs = getNumber(0,14);

	vector<array<array<int, 2>, 3>> searchInstructions;
	array<int, 2> tempArray;
	array<array<int, 2>, 3> tempSearchArray;
	cout << "Select search type A - Fast / B - Standard / C - Two rounds: ";
	options = { 'A','B','C' };
	option = getOption(options);
	switch (option)
	{
	case 'A':
		tempArray = { 'I', 1 };
		tempSearchArray[0] = tempArray;
		tempSearchArray[1] = tempArray;
		tempSearchArray[2] = tempArray;
		searchInstructions.push_back(tempSearchArray);
		break;
	case 'B':
		tempArray = { 'I', 60 };
		tempSearchArray[0] = tempArray;
		tempArray = { 'I', 12 };
		tempSearchArray[1] = tempArray;
		tempArray = { 'I', 1 };
		tempSearchArray[2] = tempArray;
		searchInstructions.push_back(tempSearchArray);
		break;
	case 'C':
		tempArray = { 'I', 60 };
		tempSearchArray[0] = tempArray;
		tempSearchArray[1] = tempArray;
		tempArray = { 'I', 12 };
		tempSearchArray[2] = tempArray;
		searchInstructions.push_back(tempSearchArray);
		tempSearchArray[0] = tempArray;
		tempArray = { 'I', 1 };
		tempSearchArray[1] = tempArray;
		tempSearchArray[2] = tempArray;
		searchInstructions.push_back(tempSearchArray);
		break;
	}

	cout << "Enter plaintext filename: ";
	string plaintext = getUserFile();
	vector<int> plainnumbers = stringToNumbers(plaintext);
	vector<int> recoverednumbers;
	double percentCorrect;
	cout << "\n";


	//Benchmark
	logbook record;
	record.printingLevel = 'S';
	record.log("Starting benchmark",'S');

	//Generate random ciphertext
	vector<vector<int>> ciphernumbersVector = encryptRandoms(n, plainnumbers, reflectorPossibilities, extraPossibilities, rotorPossibilities, numberOfPlugs);

	//For each ciphertext
	for (unsigned int i = 0; i < ciphernumbersVector.size(); i++)
	{
		//Attack
		recoverednumbers = crack(searchInstructions, ciphernumbersVector[i], reflectorPossibilities, extraPossibilities, rotorPossibilities, rotorPossibilities, rotorPossibilities, startingPlugs, record);

		//Get percent of recovered text that matches input
		int correct = 0;
		for (unsigned int j = 0; j < recoverednumbers.size(); j++)
		{
			if (recoverednumbers[j] == plainnumbers[j]) correct++;
		}
		percentCorrect = double(correct) / double(recoverednumbers.size());
		percentCorrect *= 100.0;
		cout << "Percent match: " << percentCorrect << "\n";
	}

	record.log("Finished benchmark\n",'S');
	cout << "Time taken for benchmark: " << record.timeInterval("Starting benchmark", "Finished benchmark\n") << "ms\n";
}
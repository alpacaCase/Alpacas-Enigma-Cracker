#include <iostream>
#include <vector>
#include <string>

#include "CircuitsTest.h"
#include "TestingTools.h"
#include "../BombeAttack/CipherGraph.h"
#include "../UI/UI.h"

using namespace std;

void circuitsTest(logbook& record)
{
	//Possible settings
	vector<int> reflectorPossibilities = { 'B' };
	vector<int> extraPossibilities = { 'T' };
	vector<int> rotorPossibilities = { 1, 2, 3, 4, 5 };

	//Strings to test
	vector<string> strings;
	strings.push_back("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
	strings.push_back("tobeornottobethatisthetroublingthingtobeornottobethatisthetroublingthing");
	strings.push_back("itisatruthuniversallyacknowledgedthataitisatruthuniversallyacknowledgedt");
	strings.push_back("thequickbrownfoxjumpedoverthelazydogthequickbrownfoxjumpedoverthelazydog");

	//Output string
	string output = "";

	//Program will test length of plaintexts from [0,maxLength)
	//Note input strings have a length of 72
	int maxLength = 41;
	//Program will average over n encryption for each length/plaintext combination
	int n = 10000;

	cout << "\nCircuits Test\n\n";
	cout << "The known plaintext attack requires finding all the circuits in a graph formed by combining the plaintext and cipher text. This program tests the average number produced and their length for given plaintexts when encrypted using random settings " << n << " times\n";

	for (unsigned int i = 0; i < strings.size(); i++)
	{
		string temp = "Case " + to_string(i) + ": " + strings[i] + "\n";
		cout << temp;
		output += temp;
	}

	cout << endl;

	//loop over each case and then each length
	for (unsigned int k = 0; k < strings.size(); k++) for (int length = 1; length < maxLength; length++)
	{
		//Convert to numbers of right size and randomly encrypt
		vector<int> plainnumbers = stringToNumbers(strings[k]);
		plainnumbers.resize(length);
		vector<vector<int>> ciphernumbers = encryptRandoms(n, plainnumbers, reflectorPossibilities, extraPossibilities, rotorPossibilities, 10);

		//Sum number of circuits, averaging number and lengths
		int total = 0;
		int totalLoopLengths = 0;
		for (int i = 0; i < n; i++)
		{
			cipherGraph menu;
			menu.buildGraph(numberVectorToString(plainnumbers), numberVectorToString(ciphernumbers[i]));
			menu.findCircuits();
			total += menu.circuits.size();
			for (unsigned int j = 0; j < menu.circuits.size(); j++)
			{
				totalLoopLengths += menu.circuits[j].size();
			}
		}

		//Calculate and output
		double average = double(total) / double(n);
		double averageLoopLength = double(totalLoopLengths) / double(total);
		string temp = "Case " + to_string(k) + ", length " + paddedNumber(length) + ", average # of circuits: " + to_string(average) + ", average # of vertices in circuit: " + to_string(averageLoopLength) + "\n";
		cout << temp;
		output += temp;
	}

	//Save
	saveFile("Data/TestData/CircuitsTest.txt", output);
	cout << "\nSaved as CircuitsTest.txt\n\n";
}
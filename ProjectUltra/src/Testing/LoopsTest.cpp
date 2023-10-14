#include <iostream>
#include <vector>
#include <string>

#include "LoopsTest.h"
#include "TestingTools.h"
#include "../BombeAttack/CipherGraph.h"
#include "../UI/UI.h"

using namespace std;

void loopsTest()
{
	//Possible settings
	vector<int> reflectorPossibilities = { 'B' };
	vector<int> extraPossibilities = { 'T' };
	vector<int> rotorPossibilities = { 1, 2, 3, 4, 5 };

	//Strings to test
	vector<string> strings;
	strings.push_back("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
	strings.push_back("tobeornottobethatisthetroublingthingtobeornottobethatisthetroublingthing");
	strings.push_back("itisatruthuniversallyacknowledgedthataitisatruthuniversallyacknowledgedthata");
	strings.push_back("thequickbrownfoxjumpedoverthelazydogthequickbrownfoxjumpedoverthelazydog");

	//Output string
	string output = "";

	//Number per string-length combination
	int n = 10000;

	cout << "\nLoops Test\n\n";
	cout << "This program tests the average number of loops (and their average length) generated in the \"menus\" when different texts are encrypted with random settings\n";

	for (unsigned int i = 0; i < strings.size(); i++)
	{
		string temp = "Case " + to_string(i) + ": " + strings[i] + "\n";
		cout << temp;
		output += temp;
	}

	cout << endl;

	for (unsigned int k = 0; k < strings.size(); k++) for (int length = 1; length < 31; length++)
	{
		//Convert to numbers of right size and randomly encrypt
		vector<int> plainnumbers = stringToNumbers(strings[k]);
		plainnumbers.resize(length);
		vector<vector<int>> ciphernumbers = encryptRandoms(n, plainnumbers, reflectorPossibilities, extraPossibilities, rotorPossibilities, 10);

		//Sum number of loops
		int total = 0;
		int totalLoopLengths = 0;
		for (int i = 0; i < n; i++)
		{
			cipherGraph menu;
			menu.buildGraph(numberVectorToString(plainnumbers), numberVectorToString(ciphernumbers[i]));
			menu.findLoops();
			total += menu.loops.size();
			for (unsigned int j = 0; j < menu.loops.size(); j++)
			{
				totalLoopLengths += menu.loops[j].size();
			}
		}

		//Calculate and output
		double average = double(total) / double(n);
		double averageLoopLength = double(totalLoopLengths) / double(total);
		string temp = "Case " + to_string(k) + ", length " + paddedNumber(length) + ", average # of loops: " + to_string(average) + ", average # of vertices in loop: " + to_string(averageLoopLength) + "\n";
		cout << temp;
		output += temp;
	}

	//Save
	saveFile("Data/TestData/LoopsTest.txt", output);
	cout << "\nSaved as LoopsTest.txt\n\n";
}
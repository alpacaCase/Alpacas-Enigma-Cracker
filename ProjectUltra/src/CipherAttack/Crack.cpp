#include <mutex>
#include <thread>
#include <iostream>
#include <iomanip>

#include "Crack.h"
#include "IOC.h"
#include "CrackingTools.h"
#include "../UI/UI.h"
#include "../Enigma/Enigma.h"

using namespace std;

/*
Function removes all but best n apspe
*/
void selectBest(int n, vector<array<int, 5>>& arrangements, vector<array<int, 4>>& positions, vector<array<int, 4>>& settings, vector<vector<array<int, 2>>>& plugs, vector<double>& evals)
{
	//Check actually need to select best
	if (unsigned(n) < arrangements.size())
	{
		vector<array<int, 5>> tempArrangements;
		vector<array<int, 4>> tempPositions;
		vector<array<int, 4>> tempSettings;
		vector<vector<array<int, 2>>> tempPlugs;
		vector<double> tempEvals;
		tempArrangements.reserve(n);
		tempPositions.reserve(n);
		tempSettings.reserve(n);
		tempEvals.reserve(n);

		double bestEval;
		unsigned int highestIndex = -1;

		//Pull out best n
		for (int i = 0; i < n; i++)
		{
			//Find index of best
			bestEval = 0;
			for (unsigned int x = 0; x < arrangements.size(); x++)
			{
				if (evals[x] > bestEval)
				{
					highestIndex = x;
					bestEval = evals[x];
				}
			}

			//Add best to temp and then remove from apspe
			tempArrangements.push_back(arrangements[highestIndex]);
			arrangements.erase(arrangements.begin() + highestIndex);
			tempPositions.push_back(positions[highestIndex]);
			positions.erase(positions.begin() + highestIndex);
			tempSettings.push_back(settings[highestIndex]);
			settings.erase(settings.begin() + highestIndex);
			tempPlugs.push_back(plugs[highestIndex]);
			plugs.erase(plugs.begin() + highestIndex);
			tempEvals.push_back(evals[highestIndex]);
			evals.erase(evals.begin() + highestIndex);
		}

		//Set apspe to best
		arrangements = tempArrangements;
		positions = tempPositions;
		settings = tempSettings;
		plugs = tempPlugs;
		evals = tempEvals;
	}
}

/*
Function prints setting in apsp to the console
*/
void printSetting(array<int, 5>& arrangement, array<int, 4>& position, array<int, 4>& setting, vector<array<int, 2>>& plug)
{
	vector<string> rotorNames = { "   I","  II"," III","  IV","   V","  VI"," VII","VIII" };
	int rotorSetting[3][3];
	int reflectorSetting[4];
	string plugString = "";
	apsToSetting(arrangement, position, setting, rotorSetting, reflectorSetting);

	if (reflectorSetting[0] == 'b' or reflectorSetting[0] == 'c')
	{
		cout << (char)reflectorSetting[0] << "     " << (char)reflectorSetting[1] << "  " << rotorNames[rotorSetting[0][0] - 1] << "  " << rotorNames[rotorSetting[1][0] - 1] << "  " << rotorNames[rotorSetting[2][0] - 1] << "\n";
		cout << "     " << right << setw(2) << reflectorSetting[2] << "    " << right << setw(2) << rotorSetting[0][1] << "    " << right << setw(2) << rotorSetting[1][1] << "    " << right << setw(2) << rotorSetting[2][1] << "\n";
		cout << "     " << right << setw(2) << reflectorSetting[3] << "    " << right << setw(2) << rotorSetting[0][2] << "    " << right << setw(2) << rotorSetting[1][2] << "    " << right << setw(2) << rotorSetting[2][2] << "\n";
	}
	else
	{
		cout << (char)reflectorSetting[0] << "        " << rotorNames[rotorSetting[0][0] - 1] << "  " << rotorNames[rotorSetting[1][0] - 1] << "  " << rotorNames[rotorSetting[2][0] - 1] << "\n";
		cout << "           " << right << setw(2) << rotorSetting[0][1] << "    " << right << setw(2) << rotorSetting[1][1] << "    " << right << setw(2) << rotorSetting[2][1] << "\n";
		cout << "           " << right << setw(2) << rotorSetting[0][2] << "    " << right << setw(2) << rotorSetting[1][2] << "    " << right << setw(2) << rotorSetting[2][2] << "\n";
	}
	if (plug.size() == 0) plugString = "NO PLUGS";
	else for (unsigned int i = 0; i < plug.size(); i++)
	{
		plugString += plug[i][0] + 65;
		plugString += plug[i][1] + 65;
		plugString += ' ';
	}
	cout << plugString << "\n";
}

/*
Function prints the current rotor arrangement to the console
*/
void printSettingLite(array<int, 5>& arrangement, array<int, 4>& position, array<int, 4>& setting, vector<array<int, 2>>& plug)
{
	vector<string> rotorNames = { "   I","  II"," III","  IV","   V","  VI"," VII","VIII" };

	if (arrangement[4] == 'b' or arrangement[4] == 'c')
	{
		cout << (char)arrangement[4] << "     " << (char)arrangement[3] << "  " << rotorNames[arrangement[2] - 1] << "  " << rotorNames[arrangement[1] - 1] << "  " << rotorNames[arrangement[0] - 1] << "\n";
	}
	else
	{
		cout << (char)arrangement[4] << "        " << rotorNames[arrangement[2] - 1] << "  " << rotorNames[arrangement[1] - 1] << "  " << rotorNames[arrangement[0] - 1] << "\n";
	}
}

/*
Function acts as single thread to perform a positions, setting or plug search
	computeLock prevents computed being accessed by two threads
	consoleLock stops two threads printing to the console at once
	computed is true is the arrangement at that index has already been analysed
*/
void searchThread(array<int, 3>& instructions, mutex& computeLock, mutex& consoleLock, vector<bool>& computed, vector<int>& ciphernumbers, vector<array<int, 5>>& arrangements, vector<array<int, 4>>& positions,
	vector<array<int, 4>>& settings, vector<vector<array<int, 2>>>& plugs, vector<double>& evals)
{
	unsigned int i = 0;
	while (i < arrangements.size())
	{
		//Look for next uncomputed task
		computeLock.lock();
		while (true)
		{
			if (i < arrangements.size() && computed[i]) i++;
			else if (i < arrangements.size()) { computed[i] = true;  break; }
			else break;
		}
		computeLock.unlock();
		if (i >= arrangements.size()) break;

		//Carry out search
		switch (instructions[1])
		{
		case 'I':
			switch (instructions[2])
			{
			case 'O':
				searchPositionsIOC(ciphernumbers, arrangements[i], positions[i], settings[i], plugs[i], evals[i]);
				break;
			case 'S':
				searchSettingsIOC(ciphernumbers, arrangements[i], positions[i], settings[i], plugs[i], evals[i]);
				break;
			case 'U':
				searchPlugboardIOC(ciphernumbers, arrangements[i], positions[i], settings[i], plugs[i], evals[i]);
				break;
			}
			break;
		}

		//Print debug info
		if (instructions[0] == 'V')
		{
			consoleLock.lock();
			printSetting(arrangements[i], positions[i], settings[i], plugs[i]);
			cout << right << setw(25) << evals[i] << "\n\n";
			consoleLock.unlock();
		}
		else if (instructions[0] == 'L')
		{
			consoleLock.lock();
			printSettingLite(arrangements[i], positions[i], settings[i], plugs[i]);
			consoleLock.unlock();
		}

		i++;
	}
}

/*
Function creates necessary threads to perform a particular search
	instructions: 
		{[V-Verbose, logs everything to console/L-Lite, logs only some things to console/S-Silent, no console logging]
		 [I-Index of conicidience evaluations]
		 [O-Search positions/S-Search settings/U-Search plugs]}
*/
void crackThreadHandler(array<int, 3>& instructions, vector<int>& ciphernumbers, vector<array<int, 5>>& arrangements, vector<array<int, 4>>& positions,
	vector<array<int, 4>>& settings, vector<vector<array<int, 2>>>& plugs, vector<double>& evals)
{
	vector<thread> threads;
	vector<bool> computed(arrangements.size(), false);

	//Make optimum number of threads
	int numberOfThreads = _Thrd_hardware_concurrency();
	if (signed(arrangements.size()) < numberOfThreads) numberOfThreads = signed(arrangements.size());
	mutex computeLock;
	mutex consoleLock;

	//Print debug info
	if (instructions[0] == 'V') cout << "Using " << numberOfThreads << " threads\n\n";

	//Make threads
	for (int i = 0; i < numberOfThreads; i++)
	{
		thread tempThread(searchThread, ref(instructions), ref(computeLock), ref(consoleLock), ref(computed), ref(ciphernumbers), ref(arrangements), ref(positions), ref(settings), ref(plugs), ref(evals));
		threads.push_back(move(tempThread));
	}

	//Join threads
	for (int i = 0; i < numberOfThreads; i++)
	{
		threads[i].join();
	}
}

vector<int> crack(vector<array<array<int,2>, 3>> searchInstructions, vector<int> ciphernumbers, vector<int> reflectorPosibilities, vector<int> extraPosibilities,
	vector<int> zeroPosibilities, vector<int> onePosibilities, vector<int> twoPosibilities, vector<array<int, 2>> startingPlugs, char logging)
{
	bool liteLogging = logging == 'V' || logging == 'L';
	bool fullLogging = logging == 'V';
	
	//Generate possible arrangements
	if (liteLogging) cout << "Generating search space\n\n";
	vector<array<int, 5>> arrangements = generatePossibleArrangements(reflectorPosibilities, extraPosibilities, zeroPosibilities, onePosibilities, twoPosibilities);

	//Declare variables and initialise
	if (fullLogging) cout << "Preparing search\n";
	vector<array<int, 4>> positions;
	vector<array<int, 4>> settings;
	vector<vector<array<int, 2>>> plugs;
	vector<double> evals;
	positions.reserve(arrangements.size());
	settings.reserve(arrangements.size());
	evals.reserve(arrangements.size());
	for (unsigned int i = 0; i < arrangements.size(); i++)
	{
		positions.push_back({ 0,0,0,0 });
		settings.push_back({ 0,0,0,0 });
		plugs.push_back(startingPlugs);
		evals.push_back(0.0);
	}
	array<int, 3> instructions = { 0,0,0 };

	for (unsigned int i = 0; i < searchInstructions.size(); i++)
	{
		//Search arrangements
		if (liteLogging) cout << "Beginning arrangement search\n";
		instructions[0] = logging;
		instructions[1] = searchInstructions[i][0][0];
		instructions[2] = 'O';
		crackThreadHandler(instructions, ciphernumbers, arrangements, positions, settings, plugs, evals);
		if (liteLogging) cout << "Search finished\n\n";

		selectBest(searchInstructions[i][0][1], arrangements, positions, settings, plugs, evals);

		//search for best setting
		if (liteLogging) cout << "Beginning setting search\n";
		instructions[0] = logging;
		instructions[1] = searchInstructions[i][1][0];
		instructions[2] = 'S';
		crackThreadHandler(instructions, ciphernumbers, arrangements, positions, settings, plugs, evals);
		if (liteLogging) cout << "Search finished\n\n";

		selectBest(searchInstructions[i][1][1], arrangements, positions, settings, plugs, evals);

		//search plugboard
		if (liteLogging) cout << "Beginning plugboard search\n";
		instructions[0] = logging;
		instructions[1] = searchInstructions[i][2][0];
		instructions[2] = 'U';
		crackThreadHandler(instructions, ciphernumbers, arrangements, positions, settings, plugs, evals);
		if (liteLogging) cout << "Search finished\n\n";

		selectBest(searchInstructions[i][2][1], arrangements, positions, settings, plugs, evals);
	}

	if (liteLogging)
	{
		cout << "Best settings are:\n";
		printSetting(arrangements[0], positions[0], settings[0], plugs[0]);
		cout << right << setw(25) << evals[0] << "\n\n";
	}

	//Decode and output
	enigma machine;
	int rotorSetting[3][3];
	int reflectorSetting[4];
	apsToSetting(arrangements[0], positions[0], settings[0], rotorSetting, reflectorSetting);
	machine.initialise(rotorSetting, reflectorSetting, plugs[0]);
	machine.code(ciphernumbers);

	return ciphernumbers;
}

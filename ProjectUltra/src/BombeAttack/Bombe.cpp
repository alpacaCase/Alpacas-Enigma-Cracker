#include <deque>
#include <iostream>
#include <thread>
#include <mutex>
#include <iomanip>

#include "Bombe.h"
#include "../Enigma/Enigma.h"
#include "CipherGraph.h"
#include "../CipherAttack/Quadgram.h"
#include "../CipherAttack/GenerateArrangements.h"
#include "../UI/UI.h"

using namespace std;

//Vector to store quadgram scores
vector<vector<vector<vector<double>>>> quadgrams;

/*
Notes:
setting array takes form {reflector, extra rotors, extra rotors position, extra rotors setting, rotors zero, rotors zero position, rotors zero setting, rotors one...
arrangement array takes form {rotors two, rotors one, rotors zero, extra rotors, reflector}
plugArray[index] is -1 if no knowledge about what index is plugged to, otherwise plugArray[index] is what index is plugged to
*/

/*
Function returns deque of the next cribLength mappings from machine
*/
void makeMappings(enigma& machine, int cribLength, deque<array<int, 26>>& mappings, deque<array<int, 3>>& settings)
{
	mappings.clear();
	settings.clear();
	for (int i = 0; i < cribLength; i++)
	{
		settings.push_back(machine.getSettingArray());
		machine.stepRotors();
		mappings.push_back(machine.makeMap());
	}
}

/*
Function recursively tests menu for the current mappings
Adds setting and result to possibleSettings and plugs if loop is possible with current mappings
settingArray is setting information for mapping at mappings[0]
*/
void testLoops(unsigned int index, array<int, 26>& currentPlug, deque<array<int, 26>>& mappings, cipherGraph& menu, array<int, 13>& settingArray, vector<array<int, 13>>& possibleSettings, vector<array<int, 26>>& plugs)
{
	bool noCollision;
	int testLetter;

	//End recursion condition
	if (index + 1 > menu.loops.size())
	{
		bool hasChanged = true;
		noCollision = true;
		int a;
		int b;
		while (hasChanged)
		{
			hasChanged = false;
			for (int i = 0; (i < 26) && noCollision; i++) if (currentPlug[i] != -1)
			{
				for (unsigned int j = 0; j < menu.nodes[i].links.size(); j++)
				{
					//Put letter through plugboard and scrambler
					testLetter = currentPlug[i];
					testLetter = mappings[menu.nodes[i].links[j][1]][testLetter];
					a = testLetter;
					b = menu.nodes[i].links[j][0];

					//Either result result into crib
					if (currentPlug[a] == -1 && currentPlug[b] == -1)
					{
						currentPlug[a] = b;
						currentPlug[b] = a;
						hasChanged = true;
					}
					//Or check they are already plugged, otherwise eliminate this possibility
					else if (currentPlug[a] != b)
					{
						noCollision = false;
						hasChanged = false;
						break;
					}
				}
			}
		}

		if (noCollision)
		{
			//Loops are possible
			plugs.push_back(currentPlug);
			possibleSettings.push_back(settingArray);
		}
	}

	else
	{
		array<int, 26> newPlug;
		int a;
		int b;
		//Loop through all possible letters
		for (int i = 0; i < 26; i++)
		{
			testLetter = i;
			newPlug = currentPlug;
			noCollision = true;

			//Go round loop
			for (unsigned int j = 0; j < menu.loops[index].size(); j++)
			{
				//Plug loop letter, if thats impossible emilinate this possibility
				a = testLetter;
				b = menu.loops[index][j][0];
				if (newPlug[a] == -1 && newPlug[b] == -1)
				{
					newPlug[a] = b;
					newPlug[b] = a;
				}
				else if (newPlug[a] != b)
				{
					noCollision = false;
					break;
				}

				testLetter = mappings[menu.loops[index][j][1]][testLetter];
			}

			//If there is a loop and plugs are possible
			if (noCollision && testLetter == i)
			{
				testLoops(index + 1, newPlug, mappings, menu, settingArray, possibleSettings, plugs);
			}
		}
	}
}

/*
Function adds all possible setting and result combinations to possibleSettings and plugs for a given arrangement of rotors and menu
	fullSearch - if true will also try all possible ring settings for middle rotors, 26x slower, usually not necessary
*/
void checkArrangement(bool fullSearch, cipherGraph& menu, array<int, 5>& arrangement, vector<array<int, 13>>& possibleSettings, vector<array<int, 26>>& plugs)
{
	//Check if searching middle ring positions
	int oneSettingStop;
	if (fullSearch) oneSettingStop = 26;
	else oneSettingStop = 1;

	//Notch positions for each rotors
	vector<vector<int>> notches = { {16}, {4}, {21}, {9}, {25}, {25,12}, {25,12}, {25,12} };

	//Variable to hold settings for machine
	int rotorSetting[3][3];
	int reflectorSetting[4];
	vector<array<int, 2>> plug = {};

	rotorSetting[2][0] = arrangement[0];
	vector<int> twoNotches = notches[rotorSetting[2][0] - 1];

	rotorSetting[1][0] = arrangement[1];
	vector<int> oneNotches = notches[rotorSetting[1][0] - 1];

	rotorSetting[0][0] = arrangement[2];
	rotorSetting[0][2] = 0;

	reflectorSetting[0] = arrangement[4];
	reflectorSetting[1] = arrangement[3];
	reflectorSetting[3] = 0;

	//Calculate cycle length
	int cycleLength = ((26 / signed(twoNotches.size())) * ((26 / signed(oneNotches.size())) - 1)) + 1;

	//Check if four rotors enigma
	int extraPositionStop;
	if (reflectorSetting[0] == 'b' || reflectorSetting[0] == 'c') extraPositionStop = 26;
	else extraPositionStop = 1;

	enigma machine;
	deque<array<int, 26>> mappings;
	deque<array<int, 3>> settings;
	array<int, 13> settingArray;
	array<int, 26> startPlug;
	for (int i = 0; i < 26; i++) startPlug[i] = -1;

	//Record settings that do not change in loop
	settingArray[0] = reflectorSetting[0];
	settingArray[1] = reflectorSetting[1];
	settingArray[3] = reflectorSetting[3];
	settingArray[4] = rotorSetting[0][0];
	settingArray[6] = rotorSetting[0][2];
	settingArray[7] = rotorSetting[1][0];
	settingArray[10] = rotorSetting[2][0];

	//Loop extra rotors position
	for (reflectorSetting[2] = 0; reflectorSetting[2] < extraPositionStop; reflectorSetting[2]++)
	{
		settingArray[2] = reflectorSetting[2];

		//Loop middle rotors ring setting
		for (rotorSetting[1][2] = 0; rotorSetting[1][2] < oneSettingStop; rotorSetting[1][2]++)
		{
			settingArray[9] = rotorSetting[1][2];

			//Loop right rotors ring setting
			for (rotorSetting[2][2] = 0; rotorSetting[2][2] < 26; rotorSetting[2][2]++)
			{
				settingArray[12] = rotorSetting[2][2];

				//Loop left rotors position
				for (rotorSetting[0][1] = 0; rotorSetting[0][1] < 26; rotorSetting[0][1]++)
				{
					//Loop through start positions
					for (unsigned int i = 0; i < oneNotches.size(); i++)
					{
						rotorSetting[1][1] = oneNotches[i];
						for (unsigned int j = 0; j < twoNotches.size(); j++)
						{
							rotorSetting[2][1] = twoNotches[j];

							//Build mappings
							machine.set(rotorSetting, reflectorSetting, plug);
							makeMappings(machine, menu.cribLength, mappings, settings);

							//Test loops
							for (int k = 0; k < cycleLength; k++)
							{
								settingArray[5] = settings[0][0];
								settingArray[8] = settings[0][1];
								settingArray[11] = settings[0][2];

								testLoops(0, startPlug, mappings, menu, settingArray, possibleSettings, plugs);

								//Increment settings by one
								settings.pop_front();
								settings.push_back(machine.getSettingArray());
								machine.stepRotors();
								mappings.pop_front();
								mappings.push_back(machine.makeMap());
							}
						}
					}
				}
			}
		}
	}
}

/*
Function logs arrangement to the console, does not teminate it with a newline
*/
void printArrangement(array<int, 5>& arrangement)
{
	vector<string> rotorNames = { "   I","  II"," III","  IV","   V","  VI"," VII","VIII" };

	if (arrangement[4] == 'b' or arrangement[4] == 'c')
	{
		cout << (char)arrangement[4] << "     " << (char)arrangement[3] << "  " << rotorNames[arrangement[2] - 1] << "  " << rotorNames[arrangement[1] - 1] << "  " << rotorNames[arrangement[0] - 1];
	}
	else
	{
		cout << (char)arrangement[4] << "        " << rotorNames[arrangement[2] - 1] << "  " << rotorNames[arrangement[1] - 1] << "  " << rotorNames[arrangement[0] - 1];
	}
}

/*
Function is worker thread for checking arrangements
	instructions - {[V - verbose lots of logging, L - Lite logging, S - Silent no logging], [1 full search, 0 normal search]}
	computeLock - Lock for getting next compute task
	resultsLock - lock for adding to possibleSettings etc.
	consoleLock - lock for printing to the console
	menu - menu to check
	error - 0 if no error otherwise holds error code
*/
void bombeThread(array<int, 2>& instructions, mutex& computeLock, mutex& resultsLock, mutex& consoleLock, vector<bool>& computed, cipherGraph& menu, vector<array<int, 5>>& arrangements, 
	vector<array<int, 13>>& possibleSettings, vector<array<int, 26>>& plugs, int& error)
{
	unsigned int i = 0;
	vector<array<int, 13>> tempPossibleSettings;
	vector<array<int, 26>> tempPlugs;
	bool liteLogging = instructions[0] == 'L' || instructions[0] == 'V';
	while (i < arrangements.size() && !error)
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
		tempPossibleSettings.clear();
		tempPlugs.clear();
		checkArrangement(instructions[1], menu, arrangements[i], tempPossibleSettings, tempPlugs);

		//Save results
		if (tempPossibleSettings.size() > 0)
		{
			resultsLock.lock();
			possibleSettings.insert(possibleSettings.end(), tempPossibleSettings.begin(), tempPossibleSettings.end());
			plugs.insert(plugs.end(), tempPlugs.begin(), tempPlugs.end());
			if (possibleSettings.size() > 1000000) error = 2;
			resultsLock.unlock();
		}

		//Print debug info
		if (liteLogging)
		{
			consoleLock.lock();
			printArrangement(arrangements[i]);
			cout << " Found " << tempPossibleSettings.size() << " possible settings\n";
			consoleLock.unlock();
		}

		i++;
	}
}

/*
Function creates optimal number of bombeThreads, runs them and joins them
	instructions - {[V - verbose lots of logging, L - Lite logging, S - Silent no logging], [1 full search, 0 normal search]}
	menu - menu to check
Error 2: Would generate more than 10e6 possibilities
*/
void bombeThreadHandler(array<int, 2>& instructions, cipherGraph& menu, vector<array<int, 5>>& arrangements, vector<array<int,13>>& possibleSettings, vector<array<int, 26>>& plugs)
{
	bool fullLogging = instructions[0] == 'V';
	
	vector<thread> threads;
	vector<bool> computed(arrangements.size(), false);
	int error = 0;

	//Make optimum number of threads
	int numberOfThreads = _Thrd_hardware_concurrency();
	if (signed(arrangements.size()) < numberOfThreads) numberOfThreads = signed(arrangements.size());
	mutex computeLock;
	mutex consoleLock;
	mutex resultsLock;

	//Print debug info
	if (fullLogging) cout << "Using " << numberOfThreads << " threads\n";

	//Make threads
	for (int i = 0; i < numberOfThreads; i++)
	{
		thread tempThread(bombeThread, ref(instructions), ref(computeLock), ref(resultsLock), ref(consoleLock), ref(computed), ref(menu), ref(arrangements), ref(possibleSettings), ref(plugs), ref(error));
		threads.push_back(move(tempThread));
	}

	//Join threads
	for (int i = 0; i < numberOfThreads; i++)
	{
		threads[i].join();
	}

	if (error) throw error;
}

/*
Function returns plug vector from plug array
*/
vector<array<int, 2>> plugArrayToPlug(array<int, 26> plugArray)
{
	vector<array<int, 2>> result = {};
	array<int, 2> tempPlug;

	for (int i = 0; i < 26; i++)
	{
		if (plugArray[i] > i)
		{
			tempPlug[0] = i;
			tempPlug[1] = plugArray[i];
			result.push_back(tempPlug);
		}
	}

	return result;
}

/*
Function encode plannumbers according to setting and plugArray
*/
void encodeFromSetttingArray(vector<int>& plainnnumbers, array<int, 13>& setting, array<int, 26>& plugArray)
{
	enigma machine;

	int rotorSetting[3][3];
	int reflectorSetting[4];

	reflectorSetting[0] = setting[0];
	reflectorSetting[1] = setting[1];
	reflectorSetting[2] = setting[2];
	reflectorSetting[3] = setting[3];

	rotorSetting[0][0] = setting[4];
	rotorSetting[0][1] = setting[5];
	rotorSetting[0][2] = setting[6];
	rotorSetting[1][0] = setting[7];
	rotorSetting[1][1] = setting[8];
	rotorSetting[1][2] = setting[9];
	rotorSetting[2][0] = setting[10];
	rotorSetting[2][1] = setting[11];
	rotorSetting[2][2] = setting[12];

	vector<array<int, 2>> plug = plugArrayToPlug(plugArray);

	machine.set(rotorSetting, reflectorSetting, plug);
	machine.code(plainnnumbers);
}

/*
Function returns quadscore for ciphernumbers when encoded according to possibleSetting and result
*/
double evaluate(vector<int>& ciphernumbers, array<int, 13>& possibleSetting, array<int, 26>& plug)
{
	vector<int> plainnumbers = ciphernumbers;
	encodeFromSetttingArray(plainnumbers, possibleSetting, plug);
	return quadScore(plainnumbers, quadgrams);
}

/*
Function tries to find optimal setting and result for ciphernumbers
*/
void improve(vector<int>& ciphernumbers, array<int, 13>& setting, array<int, 26>& plug, double& finalEval)
{
	//Find best settings
	array<int, 13> testSetting = setting;
	for (int i = 0; i < 26; i++)
	{
		testSetting[11] = (testSetting[11] + 1) % 26;
		testSetting[12] = (testSetting[12] + 1) % 26;
		for (int j = 0; j < 26; j++)
		{
			testSetting[8] = (testSetting[8] + 1) % 26;
			testSetting[9] = (testSetting[9] + 1) % 26;
			double eval = evaluate(ciphernumbers, testSetting, plug);
			if (eval > finalEval)
			{
				setting = testSetting;
				finalEval = eval;
			}
		}
	}

	//Find best plugs
	array<int, 26> testPlug;
	array<int, 2> bestPair;
	bool hasChanged = true;
	while (hasChanged)
	{
		hasChanged = false;
		for (int i = 0; i < 26; i++) if (plug[i] == -1) for (int j = i+1; j < 26; j++) if (plug[j] == -1)
		{
			testPlug = plug;
			testPlug[i] = j;
			testPlug[j] = i;
			double eval = evaluate(ciphernumbers, setting, testPlug);
			if (eval > finalEval)
			{
				bestPair[0] = i;
				bestPair[1] = j;
				finalEval = eval;
				hasChanged = true;
			}
		}
		if (hasChanged)
		{
			plug[bestPair[0]] = bestPair[1];
			plug[bestPair[1]] = bestPair[0];
		}
	}
}

/*
Function is worker thread for improve
	instructions - {[V - verbose lots of logging, L - Lite logging, S - Silent no logging], ignored}
	computeLock - Lock for getting next compute task
	consoleLock - lock for printing to the console
	percentCount - true if that percent has already been logged to console
*/
void improveThread(array<int, 2>& instructions, mutex& computeLock, mutex& consoleLock, vector<bool>& computed, array<bool, 100>& percentCount, vector<int>& ciphernumbers, vector<array<int, 13>>& possibleSettings,
	vector<array<int, 26>>& plugs, vector<double>& evals)
{
	unsigned int i = 0;
	bool liteLogging = instructions[0] == 'L' || instructions[0] == 'V';
	while (i < possibleSettings.size())
	{
		//Look for next uncomputed task
		computeLock.lock();
		while (true)
		{
			if (i < possibleSettings.size() && computed[i]) i++;
			else if (i < possibleSettings.size()) { computed[i] = true;  break; }
			else break;
		}
		computeLock.unlock();
		if (i >= possibleSettings.size()) break;

		//Carry out search
		improve(ciphernumbers, possibleSettings[i], plugs[i], evals[i]);

		//Print debug info
		if (liteLogging)
		{
			consoleLock.lock();
			int percent = (100*i) / possibleSettings.size();
			if (!percentCount[percent])
			{
				cout << percent << "%\n";
				percentCount[percent] = true;
			}
			consoleLock.unlock();
		}

		i++;
	}
}

/*
Function creates optimal number of improveThreads and joins them
		instructions - {[V - verbose lots of logging, L - Lite logging, S - Silent no logging], ignored}
*/
void improveThreadHandler(array<int, 2>& instructions, vector<int>& ciphernumbers, vector<array<int,13>>& possibleSettings, vector<array<int,26>>& plugs, vector<double>& evals)
{
	bool fullLogging = instructions[0] == 'V';
	bool liteLogging = instructions[0] == 'V' || instructions[0] == 'L';

	vector<thread> threads;
	vector<bool> computed(possibleSettings.size(), false);
	array<bool, 100> percentCount;
	for (int i = 0; i < 100; i++) percentCount[i] = false;

	//Make optimum number of threads
	int numberOfThreads = _Thrd_hardware_concurrency();
	if (signed(possibleSettings.size()) < numberOfThreads) numberOfThreads = signed(possibleSettings.size());
	mutex computeLock;
	mutex consoleLock;

	//Print debug info
	if (fullLogging) cout << "Using " << numberOfThreads << " threads\n";

	//Make threads
	for (int i = 0; i < numberOfThreads; i++)
	{
		thread tempThread(improveThread, ref(instructions), ref(computeLock), ref(consoleLock), ref(computed), ref(percentCount), ref(ciphernumbers), ref(possibleSettings), ref(plugs), ref(evals));
		threads.push_back(move(tempThread));
	}

	//Join threads
	for (int i = 0; i < numberOfThreads; i++)
	{
		threads[i].join();
	}

	if (liteLogging) cout << "100%\n\n";
}

/*
Function returns index of highest value in evals
*/
int bestIndex(vector<double>& evals)
{
	double bestEval = -INFINITY;
	int result = -1;
	for (unsigned int i = 0; i < evals.size(); i++)
	{
		if (evals[i] > bestEval)
		{
			result = signed(i);
			bestEval = evals[i];
		}
	}
	return result;
}

/*
Function print setting and plug to console
*/
void printSettingArray(array<int,13> setting, array<int,26> plugArray)
{
	vector<string> rotorNames = { "   I","  II"," III","  IV","   V","  VI"," VII","VIII" };

	if (setting[0] == 'b' or setting[0] == 'c')
	{
		cout << (char)setting[0] << "     " << (char)setting[1] << "  " << rotorNames[setting[4] - 1] << "  " << rotorNames[setting[7] - 1] << "  " << rotorNames[setting[10] - 1] << "\n";
		cout << "     " << right << setw(2) << setting[2] << "    " << right << setw(2) << setting[5] << "    " << right << setw(2) << setting[8] << "    " << right << setw(2) << setting[11] << "\n";
		cout << "     " << right << setw(2) << setting[3] << "    " << right << setw(2) << setting[6] << "    " << right << setw(2) << setting[9] << "    " << right << setw(2) << setting[12] << "\n";
	}
	else
	{
		cout << (char)setting[0] << "        " << rotorNames[setting[4] - 1] << "  " << rotorNames[setting[7] - 1] << "  " << rotorNames[setting[10] - 1] << "\n";
		cout << "           " << right << setw(2) << setting[5] << "    " << right << setw(2) << setting[8] << "    " << right << setw(2) << setting[11] << "\n";
		cout << "           " << right << setw(2) << setting[6] << "    " << right << setw(2) << setting[9] << "    " << right << setw(2) << setting[12] << "\n";
	}
	vector<array<int, 2>> plug = plugArrayToPlug(plugArray);
	string plugString;
	if (plug.size() == 0) plugString = "NO PLUGS";
	else for (unsigned int i = 0; i < plug.size(); i++)
	{
		plugString += plug[i][0] + 65;
		plugString += plug[i][1] + 65;
		plugString += ' ';
	}
	cout << plugString << "\n\n";
}

vector<int> bombe(array<int, 2> instructions, string plaintext, string ciphertext, vector<int> reflectorPossibilities, vector<int> extraPossibilities, vector<int> zeroPossibilities, vector<int> onePossibilities, 
	vector<int> twoPossibilities)
{
	bool liteLogging = instructions[0] == 'V' || instructions[0] == 'L';
	bool fullLogging = instructions[0] == 'V';

	//Generate possible arrangements
	if (liteLogging) cout << "Generating search space\n\n";
	vector<array<int, 5>> arrangements = generateArrangements(reflectorPossibilities, extraPossibilities, zeroPossibilities, onePossibilities, twoPossibilities);

	//Generate menu
	if (liteLogging) cout << "Generating menu\n\n";
	cipherGraph menu;
	menu.buildGraph(plaintext, ciphertext);

	//If no loops throw error 1
	if (!menu.loops.size()) throw 1;

	//Debug info
	if (fullLogging)
	{
		cout << "Crib length: " << menu.cribLength << "\n\n";
		cout << menu.debugLinksString();
	}
	if (liteLogging) cout << "Found " << menu.loops.size() << " loops\n";
	if(fullLogging)	cout << menu.debugLoopsString();

	//Declare variables and set
	vector<array<int, 13>> possibleSettings;
	vector<array<int, 26>> plugs;

	//Search arrangements
	if (liteLogging) cout << "\nBeginning arrangement search\n";
	bombeThreadHandler(instructions, menu, arrangements, possibleSettings, plugs);
	
	if (possibleSettings.size() == 0) throw 3;
	if (liteLogging) cout << "Found total of " << possibleSettings.size() << " possible settings\n\n";

	//Load quadgrams
	quadgrams = loadQuadgrams("Data/Grams/Quadgrams.txt");
	vector<int> ciphernumbers = stringToNumbers(ciphertext);

	if (liteLogging) cout << "Searching through possible settings\n";

	//Make evals
	vector<double> evals;
	for (unsigned int i = 0; i < possibleSettings.size(); i++)  evals.push_back(-INFINITY);

	//Improve and find best
	improveThreadHandler(instructions, ciphernumbers, possibleSettings, plugs, evals);
	int highestIndex = bestIndex(evals);

	//Decode with best setting
	vector<int> plainnumbers = ciphernumbers;
	encodeFromSetttingArray(plainnumbers, possibleSettings[highestIndex], plugs[highestIndex]);

	if (liteLogging)
	{
		cout << "Best settings are:\n";
		printSettingArray(possibleSettings[highestIndex], plugs[highestIndex]);
	}

	return plainnumbers;
}
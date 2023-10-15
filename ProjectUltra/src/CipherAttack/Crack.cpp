#include <mutex>
#include <thread>
#include <iomanip>
#include <algorithm>


#include "Crack.h"
#include "IOC.h"
#include "GenerateArrangements.h"
#include "../UI/UI.h"
#include "../Enigma/Enigma.h"

using namespace std;

/*
Chooses the best n settings (according to evals) from settings and sorts them highest eval at index 0
Does nothing if settings is not larger than n
*/
bool settingComparator(enigmaSetting a, enigmaSetting b)
{
	return a.eval > b.eval;
}

void selectBest(int n, vector<enigmaSetting>& settings)
{
	if (unsigned(n) < settings.size())
	{
		sort(settings.begin(), settings.end(), settingComparator);
		settings.resize(n);
	}
}

/*
Function acts as single thread to perform a positions, setting or plug search as dictated by instructions
	computeLock prevents computed being accessed by two threads
	logLock stops two threads adding to log at once
*/
void searchThread(array<int, 2>& instructions, mutex& computeLock, mutex& logLock, vector<int>& ciphernumbers, vector<enigmaSetting>& settings, logbook& record)
{
	unsigned int i = 0;
	while (i < settings.size())
	{
		//Look for next uncomputed task
		computeLock.lock();
		while (true)
		{
			if (i < settings.size() && settings[i].computed) i++;
			else if (i < settings.size()) { settings[i].computed = true;  break; }
			else break;
		}
		computeLock.unlock();
		if (i >= settings.size()) break;

		//Carry out search
		switch (instructions[0])
		{
		case 'I':
			switch (instructions[1])
			{
			case 'O':
				searchPositionsIOC(ciphernumbers, settings[i]);
				break;
			case 'S':
				searchSettingsIOC(ciphernumbers, settings[i]);
				break;
			case 'U':
				searchPlugboardIOC(ciphernumbers, settings[i]);
				break;
			}
			break;
		}

		logLock.lock();
		settings[i].log(record);
		logLock.unlock();

		i++;
	}
}

/*
Function creates necessary threads to perform a particular search, defined by instructions array
	instructions: 
		{[Type of evaluations: I-Index of conicidience evaluations]
		 [Type of search: O-Search positions/S-Search settings/U-Search plugs]}
*/
void crackThreadHandler(array<int, 2>& instructions, vector<int>& ciphernumbers, vector<enigmaSetting>& settings, logbook& record)
{
	vector<thread> threads;
	
	for (unsigned int i = 0; i < settings.size(); i++) settings[i].computed = false;

	//Make "optimal" number of threads
	int numberOfThreads = _Thrd_hardware_concurrency();
	if (signed(settings.size()) < numberOfThreads) numberOfThreads = signed(settings.size());
	mutex computeLock;
	mutex logLock;

	//Debug info
	record.log("Using " + to_string(numberOfThreads) + " threads", 'D');

	//Make threads
	for (int i = 0; i < numberOfThreads; i++)
	{
		thread tempThread(searchThread, ref(instructions), ref(computeLock), ref(logLock), ref(ciphernumbers), ref(settings), ref(record));
		threads.push_back(move(tempThread));
	}

	//Join threads
	for (int i = 0; i < numberOfThreads; i++)
	{
		threads[i].join();
	}
}

vector<int> crack(vector<array<array<int,2>, 3>> searchInstructions, vector<int> ciphernumbers, vector<int> reflectorPossibilities, vector<int> extraPossibilities,
	vector<int> zeroPossibilities, vector<int> onePossibilities, vector<int> twoPossibilities, vector<array<int, 2>> startingPlugs, logbook& record)
{
	record.log("Starting attack");
		
	//Generate possible arrangements
	record.log("Generating search space");
	vector<array<int, 5>> arrangements = generateArrangements(reflectorPossibilities, extraPossibilities, zeroPossibilities, onePossibilities, twoPossibilities);

	//Prepare settings
	vector<enigmaSetting> settings;
	settings.reserve(arrangements.size());

	enigmaSetting tempSetting;
	for (unsigned int i = 0; i < arrangements.size(); i++)
	{
		tempSetting.rotors[2][0] = arrangements[i][0];
		tempSetting.rotors[2][1] = 0;
		tempSetting.rotors[2][2] = 0;

		tempSetting.rotors[1][0] = arrangements[i][1];
		tempSetting.rotors[1][1] = 0;
		tempSetting.rotors[1][2] = 0;

		tempSetting.rotors[0][0] = arrangements[i][2];
		tempSetting.rotors[0][1] = 0;
		tempSetting.rotors[0][2] = 0;

		tempSetting.reflector[0] = arrangements[i][4];
		tempSetting.reflector[1] = arrangements[i][3];
		tempSetting.reflector[2] = 0;
		tempSetting.reflector[3] = 0;


		tempSetting.plug = startingPlugs;

		settings.push_back(tempSetting);
	}
	record.log("Finished generating search space\n");


	//Start seraching
	array<int, 2> instructions = { 0,0 };
	for (unsigned int i = 0; i < searchInstructions.size(); i++)
	{
		//Search arrangements
		record.log("Starting arrangement search");
		instructions[0] = searchInstructions[i][0][0];
		instructions[1] = 'O';
		crackThreadHandler(instructions, ciphernumbers, settings, record);
		record.log("Arrangment search finished\n");

		selectBest(searchInstructions[i][0][1], settings);

		//Search for best ring setting
		record.log("Starting setting search");
		instructions[0] = searchInstructions[i][1][0];
		instructions[1] = 'S';
		crackThreadHandler(instructions, ciphernumbers, settings, record);
		record.log("Setting search finished\n");

		selectBest(searchInstructions[i][1][1], settings);

		//search plugboard
		record.log("Starting plugboard search");
		instructions[0] = searchInstructions[i][2][0];
		instructions[1] = 'U';
		crackThreadHandler(instructions, ciphernumbers, settings, record);
		record.log("Plugboard search finished\n");

		selectBest(searchInstructions[i][2][1], settings);
	}

	record.log("Finished attack\n");


	record.log("Best settings are:");
	settings[0].log(record, 'L');
	record.log("Time taken for attack: " + to_string(record.timeInterval("Starting attack", "Finished attack\n")) + "ms\n");

	//Decode and output
	enigma machine;
	machine.set(settings[0]);
	machine.code(ciphernumbers);

	return ciphernumbers;
}

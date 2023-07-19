#include "IOC.h"
#include "CrackingTools.h"
#include "../Enigma/Enigma.h"

using namespace std;

/*
Function returns the index of coincidence score for the vector of numbers
0.0385 for random numbers in range [0,25] 0.067 for english text
*/
double IOC(vector<int>& numbers)
{
	int frequency[26] = { 0 };
	double result = 0;

	//Loop through vector
	for (unsigned int i = 0; i < numbers.size(); i++) frequency[numbers[i]]++;

	//Sum frequencies
	for (int i = 0; i < 26; i++)
	{
		result += frequency[i] * (frequency[i] - 1);
	}

	//Calculate result
	result = result / (numbers.size() * (numbers.size() - 1));
	return result;
}

void searchPositionsIOC(vector<int>& ciphernumbers, array<int, 5>& arrangement, array<int, 4>& bestPosition, array<int, 4>& setting, vector<array<int, 2>>& plug, double& eval)
{
	eval = 0;
	double currentIOC;
	vector<int> plainnumbers;
	plainnumbers.reserve(ciphernumbers.size());
	enigma machine;

	//Load starting settings
	int rotorSetting[3][3];
	int reflectorSetting[4];
	array<int, 4> startPosition = { 0,0,0,0 };
	apsToSetting(arrangement, startPosition, setting, rotorSetting, reflectorSetting);

	if (reflectorSetting[0] == 'b' || reflectorSetting[0] == 'c')
	{
		for (reflectorSetting[2] = 0; reflectorSetting[2] < 26; reflectorSetting[2]++) for (rotorSetting[2][1] = 0; rotorSetting[2][1] < 26; rotorSetting[2][1]++) 
			for (rotorSetting[1][1] = 0; rotorSetting[1][1] < 26; rotorSetting[1][1]++) for (rotorSetting[0][1] = 0; rotorSetting[0][1] < 26; rotorSetting[0][1]++)
		{
			//Copy and decrypt
			plainnumbers = ciphernumbers;
			machine.initialise(rotorSetting, reflectorSetting, plug);
			machine.code(plainnumbers);

			//Evaluate
			currentIOC = IOC(plainnumbers);
			if (currentIOC > eval)
			{
				eval = currentIOC;
				bestPosition[2] = rotorSetting[0][1];
				bestPosition[1] = rotorSetting[1][1];
				bestPosition[0] = rotorSetting[2][1];
				bestPosition[3] = reflectorSetting[2];
			}
		}
	}
	else
	{
		for (rotorSetting[2][1] = 0; rotorSetting[2][1] < 26; rotorSetting[2][1]++) for (rotorSetting[1][1] = 0; rotorSetting[1][1] < 26; rotorSetting[1][1]++)
			for (rotorSetting[0][1] = 0; rotorSetting[0][1] < 26; rotorSetting[0][1]++)
		{
			//Copy and decrypt
			plainnumbers = ciphernumbers;
			machine.initialise(rotorSetting, reflectorSetting, plug);
			machine.code(plainnumbers);

			//Evaluate
			currentIOC = IOC(plainnumbers);
			if (currentIOC > eval)
			{
				eval = currentIOC;
				bestPosition[2] = rotorSetting[0][1];
				bestPosition[1] = rotorSetting[1][1];
				bestPosition[0] = rotorSetting[2][1];
				bestPosition[3] = reflectorSetting[2];
			}
		}
	}
}

void searchSettingsIOC(vector<int>& ciphernumbers, array<int, 5>& arrangement, array<int, 4>& bestPosition, array<int, 4>& bestSetting, vector<array<int, 2>>& plug, double& eval)
{
	eval = 0;
	double currentIOC;
	vector<int> plainnumbers;
	plainnumbers.reserve(ciphernumbers.size());
	enigma machine;

	//load starting settings
	int rotorSetting[3][3];
	int reflectorSetting[4];
	array<int, 4> startSetting = { 0,bestSetting[1],bestSetting[2],bestSetting[3] };
	array<int, 4> startPosition = { 0,0,bestPosition[2],bestPosition[3] };
	apsToSetting(arrangement, startPosition, startSetting, rotorSetting, reflectorSetting);

	while (true)
	{
		//copy and decrypt
		plainnumbers = ciphernumbers;
		machine.initialise(rotorSetting, reflectorSetting, plug);
		machine.code(plainnumbers);

		//evaluate
		currentIOC = IOC(plainnumbers);
		if (currentIOC > eval)
		{
			eval = currentIOC;
			bestSetting[0] = rotorSetting[2][2];
			bestPosition[0] = rotorSetting[2][1];
		}

		//advance rotors
		rotorSetting[2][2] = (rotorSetting[2][2] + 1) % 26;
		if (rotorSetting[2][2] == 0)
		{
			rotorSetting[2][1] = (rotorSetting[2][1] + 1) % 26;
			if (rotorSetting[2][1] == 0)
			{
				rotorSetting[1][1] = (rotorSetting[1][1] + 1) % 26;
				if (rotorSetting[1][1] == 0) break;
			}
		}

	}

	eval = 0;
	startSetting = { bestSetting[0],0,bestSetting[2],bestSetting[3] };
	startPosition = { bestPosition[0],0,bestPosition[2],bestPosition[3] };
	apsToSetting(arrangement, startPosition, startSetting, rotorSetting, reflectorSetting);
	while (true)
	{
		//copy and decrypt
		plainnumbers = ciphernumbers;
		machine.initialise(rotorSetting, reflectorSetting, plug);
		machine.code(plainnumbers);

		//evaluate
		currentIOC = IOC(plainnumbers);
		if (currentIOC > eval)
		{
			eval = currentIOC;
			bestSetting[1] = rotorSetting[1][2];
			bestPosition[1] = rotorSetting[1][1];
		}

		//advance rotors
		rotorSetting[1][2] = (rotorSetting[1][2] + 1) % 26;
		if (rotorSetting[1][2] == 0)
		{
			rotorSetting[1][1] = (rotorSetting[1][1] + 1) % 26;
			if (rotorSetting[1][1] == 0) break;
		}

	}
}

void searchPlugboardIOC(vector<int>& ciphernumbers, array<int, 5>& arrangement, array<int, 4>& position, array<int, 4>& setting, vector<array<int, 2>>& plug, double& eval)
{
	double currentIOC;
	vector<int> plainnumbers;
	array<int, 2> bestPair;
	bool noCollision;
	plainnumbers.reserve(ciphernumbers.size());
	enigma machine;

	//Load starting settings
	int rotorSetting[3][3];
	int reflectorSetting[4];
	plug.clear();
	apsToSetting(arrangement, position, setting, rotorSetting, reflectorSetting);

	//Get initial evaluation
	plainnumbers = ciphernumbers;
	machine.initialise(rotorSetting, reflectorSetting, plug);
	machine.code(plainnumbers);
	eval = IOC(plainnumbers);

	while (true)
	{
		bestPair = { 0,0 };
		for (int i = 0; i < 26; i++) for (int j = i + 1; j < 26; j++)
		{
			//Check if i or j are already plugged
			noCollision = true;
			for (unsigned int x = 0; x < plug.size(); x++)
			{
				if (plug[x][0] == i || plug[x][1] == i || plug[x][0] == j || plug[x][1] == j) noCollision = false;
			}
			if (noCollision)
			{
				plug.push_back({ i,j });
				plainnumbers = ciphernumbers;
				machine.initialise(rotorSetting, reflectorSetting, plug);
				machine.code(plainnumbers);

				//Evaluate
				currentIOC = IOC(plainnumbers);
				if (currentIOC > eval)
				{
					eval = currentIOC;
					bestPair = plug.back();
				}
				plug.pop_back();
			}
		}
		
		//If there is a better pair add it otherwise end loop
		if (bestPair[1] == 0) break;
		else plug.push_back(bestPair);
	}
}
#include "IOC.h"
#include "CrackingTools.h"
#include "../Enigma/Enigma.h"

#include <iostream>

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

void searchPositionsIOC(vector<int>& ciphernumbers, enigmaSetting& setting)
{
	setting.eval = 0.0;
	array<int, 4> bestPosition;
	double currentIOC;
	vector<int> plainnumbers;
	plainnumbers.reserve(ciphernumbers.size());
	enigma machine;

	//4 rotor
	if (setting.reflector[0] == 'b' || setting.reflector[0] == 'c')
	{
		for (setting.reflector[2] = 0; setting.reflector[2] < 26; setting.reflector[2]++) for (setting.rotors[2][1] = 0; setting.rotors[2][1] < 26; setting.rotors[2][1]++) 
			for (setting.rotors[1][1] = 0; setting.rotors[1][1] < 26; setting.rotors[1][1]++) for (setting.rotors[0][1] = 0; setting.rotors[0][1] < 26; setting.rotors[0][1]++)
		{
			//Copy and decrypt
			plainnumbers = ciphernumbers;
			machine.set(setting);
			machine.code(plainnumbers);

			//Evaluate and if better store result
			currentIOC = IOC(plainnumbers);
			if (currentIOC > setting.eval)
			{
				setting.eval = currentIOC;
				bestPosition[0] = setting.rotors[0][1];
				bestPosition[1] = setting.rotors[1][1];
				bestPosition[2] = setting.rotors[2][1];
				bestPosition[3] = setting.reflector[2];
			}
		}

	}
	//3 rotor
	else
	{
		for (setting.rotors[2][1] = 0; setting.rotors[2][1] < 26; setting.rotors[2][1]++) for (setting.rotors[1][1] = 0; setting.rotors[1][1] < 26; setting.rotors[1][1]++)
			for (setting.rotors[0][1] = 0; setting.rotors[0][1] < 26; setting.rotors[0][1]++)
		{
			//Copy and decrypt
			plainnumbers = ciphernumbers;
			machine.set(setting);
			machine.code(plainnumbers);

			//Evaluate and if better store result
			currentIOC = IOC(plainnumbers);
			if (currentIOC > setting.eval)
			{
				setting.eval = currentIOC;
				bestPosition[0] = setting.rotors[0][1];
				bestPosition[1] = setting.rotors[1][1];
				bestPosition[2] = setting.rotors[2][1];
				bestPosition[3] = setting.reflector[2];
			}
		}
	}

	setting.rotors[0][1] = bestPosition[0];
	setting.rotors[1][1] = bestPosition[1];
	setting.rotors[2][1] = bestPosition[2];
	setting.reflector[2] = bestPosition[3];
}

void searchSettingsIOC(vector<int>& ciphernumbers, enigmaSetting& setting)
{
	setting.eval = 0;
	double currentIOC;
	vector<int> plainnumbers;
	plainnumbers.reserve(ciphernumbers.size());
	enigma machine;
	array<int, 5> bestSettings;

	setting.rotors[0][1] = (setting.rotors[0][1] + 25) % 26;
	setting.rotors[1][1] = (setting.rotors[1][1] + 25) % 26;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 26; k++)
			{
				for (int l = 0; l < 26; l++)
				{
					//copy and decrypt
					plainnumbers = ciphernumbers;
					machine.set(setting);
					machine.code(plainnumbers);

					currentIOC = IOC(plainnumbers);

					if (currentIOC > setting.eval)
					{
						setting.eval = currentIOC;
						bestSettings[0] = setting.rotors[0][1];
						bestSettings[1] = setting.rotors[1][1];
						bestSettings[2] = setting.rotors[1][2];
						bestSettings[3] = setting.rotors[2][1];
						bestSettings[4] = setting.rotors[2][2];
					}

					setting.rotors[2][1] = (setting.rotors[2][1] + 1) % 26;
					setting.rotors[2][2] = (setting.rotors[2][2] + 1) % 26;
				}
				setting.rotors[1][1] = (setting.rotors[1][1] + 1) % 26;
				setting.rotors[1][2] = (setting.rotors[1][2] + 1) % 26;
			}
			setting.rotors[1][1] = (setting.rotors[1][1] + 1) % 26;
		}
		setting.rotors[1][1] = (setting.rotors[1][1] + 23) % 26;
		setting.rotors[0][1] = (setting.rotors[0][1] + 1) % 26;
	}

	setting.rotors[0][1] = bestSettings[0];
	setting.rotors[1][1] = bestSettings[1];
	setting.rotors[1][2] = bestSettings[2];
	setting.rotors[2][1] = bestSettings[3];
	setting.rotors[2][2] = bestSettings[4];

	/*
	while (true)
	{
		//copy and decrypt
		plainnumbers = ciphernumbers;
		machine.set(rotorSetting, reflectorSetting, plug);
		machine.code(plainnumbers);

		//evaluate
		currentIOC = IOC(plainnumbers);
		if (currentIOC > setting.eval)
		{
			setting.eval = currentIOC;
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

	setting.eval = 0;

	while (true)
	{
		//copy and decrypt
		plainnumbers = ciphernumbers;
		machine.set(rotorSetting, reflectorSetting, plug);
		machine.code(plainnumbers);

		//evaluate
		currentIOC = IOC(plainnumbers);
		if (currentIOC > setting.eval)
		{
			setting.eval = currentIOC;
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
	*/
}

void searchPlugboardIOC(vector<int>& ciphernumbers, enigmaSetting& setting)
{
	double currentIOC;
	vector<int> plainnumbers;
	array<int, 2> bestPair;
	bool noCollision;
	plainnumbers.reserve(ciphernumbers.size());
	enigma machine;
	setting.plug.clear();

	//Get initial evaluation
	plainnumbers = ciphernumbers;
	machine.set(setting);
	machine.code(plainnumbers);
	setting.eval = IOC(plainnumbers);

	while (true)
	{
		bestPair = { 0,0 };
		for (int i = 0; i < 26; i++) for (int j = i + 1; j < 26; j++)
		{
			//Check if i or j are already plugged
			noCollision = true;
			for (unsigned int x = 0; x < setting.plug.size(); x++)
			{
				if (setting.plug[x][0] == i || setting.plug[x][1] == i || setting.plug[x][0] == j || setting.plug[x][1] == j) noCollision = false;
			}

			//If not test if it improves score
			if (noCollision)
			{
				setting.plug.push_back({ i,j });
				plainnumbers = ciphernumbers;
				machine.set(setting);
				machine.code(plainnumbers);

				//Evaluate
				currentIOC = IOC(plainnumbers);
				if (currentIOC > setting.eval)
				{
					setting.eval = currentIOC;
					bestPair = setting.plug.back();
				}
				setting.plug.pop_back();
			}
		}
		
		//If there is a better pair add it otherwise end loop
		if (bestPair[1] == 0) break;
		else setting.plug.push_back(bestPair);
	}
}
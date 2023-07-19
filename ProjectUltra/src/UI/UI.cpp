#include <iostream>
#include <fstream>

#include "UI.h"

using namespace std;

bool alphaToUpperCase(char& input)
{
	//Lowercase
	if (input < 123 && input > 96) input -= 32;
	//Not letter
	else if (input < 65 || input > 90) return false;
	return true;
}

bool isAlpha(char input)
{
	if (input < 65 || (input > 90 && input < 97) || input > 122) return false;
	return true;
}

int letterToInt(char letter)
{
	if (alphaToUpperCase(letter)) return letter - 65;
	else return 26;
}

string getAlphaCharacters(int numberOfCharacters, bool allUpper)
{
	string result;
	bool noErrors = true;
	while (true)
	{
		//Get input
		cin >> result;

		//Check size
		if (result.size() == numberOfCharacters)
		{
			//Check all chracters are letters and convert to upper case if necessary
			if (allUpper) for (unsigned int i = 0; i < result.size(); i++)
			{
				if (!(alphaToUpperCase(result[i]))) noErrors = false;
			}
			else for (unsigned int i = 0; i < result.size(); i++)
			{
				if (!(isAlpha(result[i]))) noErrors = false;
			}
		}
		else noErrors = false;

		if (noErrors) break;
		else
		{
			noErrors = true;
			cout << "Input error, please try again\n";
		}
	}
	return result;
}

char getOption(vector<char> options, bool allUpper)
{
	char result;
	bool isOption;
	while (true)
	{
		//Get input and check is character
		result = getAlphaCharacters(1, allUpper)[0];

		//Check is an option
		isOption = false;
		for (unsigned int i = 0; i < options.size(); i++)
		{
			if (result == options[i]) 
			{
				isOption = true; 
				break;
			}
		}
		if (isOption) break;
		else cout << "Input error, please try again\n";
	}
	return result;
}

string getUserFile()
{
	//Get filename
	string filename;
	string tempString;
	string result;

	//Check can open file
	while(true)
	{
		try
		{
			cin >> filename;
			ifstream plaintextFile(filename);
			if (plaintextFile.fail()) throw 1;
			while (getline(plaintextFile, tempString)) result.append(tempString);
			plaintextFile.close();
			break;
		}
		catch (...)
		{
			cout << "Could not open file, please try again\nEnter filename: ";
		}
	}
	return result;
}

string cleanString(string dirtyString)
{
	string result = "";
	for (unsigned int i = 0; i < dirtyString.length(); i++) 
		if (alphaToUpperCase(dirtyString[i])) result.append(string(1, dirtyString[i]));
	return result;
}

vector<int> stringToNumbers(string plainString)
{
	string cleanedString = cleanString(plainString);
	vector<int> result;
	result.reserve(cleanedString.size());
	for (unsigned int i = 0; i < cleanedString.size(); i++) result.push_back(cleanedString[i] - 65);
	return result;
}

int getSetting()
{
	string input;
	int result;
	while (true)
	{
		cin >> input;
		result = letterToInt(input[0]);
		if (result != 26)
		{
			if (input.size() == 1) break;
			else cout << "Input error, please try again\n";
		}
		else
		{
			try
			{
				result = stoi(input);
				if (result >= 0 && result < 26) break;
				else throw 1;
			}
			catch(...)
			{
				cout << "Input error, please try again\n";
			}
		}
	}
	return result;
}

int getNumber(int lowerBound, int upperBound)
{
	int result;
	string input;
	while (true)
	{
		cin >> input;
		try
		{
			result = stoi(input);
			if (result >= lowerBound && result < upperBound) break;
			else throw 1;
		}
		catch (...)
		{
			cout << "Input error, please try again\n";
		}
	}
	return result;
}

void getPlug(vector<array<int,2>>& plug)
{
	array<int, 2> tempPlug;
	bool noError;
	while (true)
	{
		string input = getAlphaCharacters(2);
		tempPlug[0] = input[0] - 65;
		tempPlug[1] = input[1] - 65;
		noError = true;
		for (unsigned int i = 0; i < plug.size(); i++)
		{
			if (tempPlug[0] == plug[i][0] || tempPlug[0] == plug[i][1] || tempPlug[1] == plug[i][0] || tempPlug[1] == plug[i][1])
			{
				noError = false;
				break;
			}
		}
		if (noError)
		{
			plug.push_back(tempPlug);
			break;
		}
		else cout << "Input error, please try again\n";
	}
}

string numberVectorToString(vector<int> ciphernumbers)
{
	string result = "";
	char temp;
	for (unsigned int i = 0; i < ciphernumbers.size(); i++)
	{
		temp = ciphernumbers[i] + 65;
		result.push_back(ciphernumbers[i] + 65);
	}
	return result;
}

string formatForOutput(string inputString)
{
	string result = "";
	for (unsigned int i = 0; i < inputString.size(); i++)
	{
		result.push_back(inputString[i]);
		if (i % 5 == 4) result.push_back(' ');
	}
	return result;
}

string paddedNumber(int n, int padTo)
{
	string result = to_string(n);
	while (result.length() < padTo) result = " " + result;
	return result;
}

string readFile(string filename)
{
	string tempString;
	string result;
	ifstream file(filename);
	while (getline(file, tempString)) result.append(tempString);
	file.close();
	return result;
}

void saveFile(string filename, string contents)
{
	ofstream outFile(filename);
	outFile << contents;
	outFile.close();
}
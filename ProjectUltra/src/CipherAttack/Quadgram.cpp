#include <fstream>
#include <sstream>
#include <math.h>

#include "Quadgram.h"

using namespace std;

vector < vector < vector < vector < double >>>> loadQuadgrams(string filename)
{
	//Construct vectors for count and quadgrams
	vector<long long> levelOneInt;
	vector<double> levelOneDouble;

	vector<vector<long long>> levelTwoInt;
	vector<vector<double>> levelTwoDouble;

	vector<vector<vector<long long>>> levelThreeInt;
	vector<vector<vector<double>>> levelThreeDouble;

	vector<vector<vector<vector<long long>>>> counts;
	vector<vector<vector<vector<double>>>> result;

	long long total = 0;

	for (int i = 0; i < 26; i++)
	{
		levelOneInt.push_back(0);
		levelOneDouble.push_back(-24);
	}

	for (int i = 0; i < 26; i++)
	{
		levelTwoInt.push_back(levelOneInt);
		levelTwoDouble.push_back(levelOneDouble);
	}

	for (int i = 0; i < 26; i++)
	{
		levelThreeInt.push_back(levelTwoInt);
		levelThreeDouble.push_back(levelTwoDouble);
	}

	for (int i = 0; i < 26; i++)
	{
		counts.push_back(levelThreeInt);
		result.push_back(levelThreeDouble);
	}

	ifstream quadgramsFile(filename);
	if (quadgramsFile.fail()) throw 50;
	string fileLine;

	//Load in quagram counts
	while (getline(quadgramsFile, fileLine))
	{
		istringstream iss(fileLine);
		string tempString;
		long long tempCount;
		int index[4];

		getline(iss, tempString, ' ');
		for (int i = 0; i < 4; i++) index[i] = tempString[i] - 65;
		getline(iss, tempString, ' ');
		tempCount = stoi(tempString);
		total += tempCount;
		counts[index[0]][index[1]][index[2]][index[3]] = tempCount;
	}
	
	quadgramsFile.close();

	//Calculate logs
	for (int i = 0; i < 26; i++) for (int j = 0; j < 26; j++) for (int k = 0; k < 26; k++) for (int l = 0; l < 26; l++)
	{
		long long count = counts[i][j][k][l];
		if (count > 0)
		{
			double probability = double(count) / double(total);
			result[i][j][k][l] = log(probability);
		}
	}

	return result;
}

double quadScore(vector<int> numbers, vector<vector<vector<vector<double>>>>& quadgrams)
{
	double result = 0;
	unsigned int stop = numbers.size() - 3;
	for (unsigned int i = 0; i < stop; i++) result += quadgrams[numbers[i]][numbers[i + 1]][numbers[i + 2]][numbers[i + 3]];
	return result;
}
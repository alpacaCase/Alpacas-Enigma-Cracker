#include "GenerateArrangements.h"

using namespace std;

vector<array<int, 5>> generateArrangements(vector<int> reflectorPossibilities, vector<int> extraPossibilities, vector<int> zeroPossibilities, vector<int> onePossibilities, vector<int> twoPossibilities)
{
	{
		array<int, 5> currentArrangement = { 0,0,0,0,0 };
		vector<array<int, 5>> result;
		bool noCollision;

		//Loop through all possible combinations
		for (unsigned int i = 0; i < reflectorPossibilities.size(); i++) for (unsigned int j = 0; j < extraPossibilities.size(); j++) for (unsigned int k = 0; k < zeroPossibilities.size(); k++)
			for (unsigned int l = 0; l < onePossibilities.size(); l++) for (unsigned int m = 0; m < twoPossibilities.size(); m++)
			{
				//Create arrangment
				currentArrangement[4] = reflectorPossibilities[i];
				currentArrangement[3] = extraPossibilities[j];
				currentArrangement[2] = zeroPossibilities[k];
				currentArrangement[1] = onePossibilities[l];
				currentArrangement[0] = twoPossibilities[m];

				//Check if any rotors used twice
				noCollision = true;
				for (int x = 0; x < 5; x++) for (int y = x + 1; y < 5; y++)
				{
					if (currentArrangement[x] == currentArrangement[y]) noCollision = false;
				}
				//Also remove extra arrangments of fourth rotors when not a thin reflector
				if (j > 0 && !(reflectorPossibilities[i] == 'b' || reflectorPossibilities[i] == 'c')) noCollision = false;

				if (noCollision) result.push_back(currentArrangement);
			}
		return result;
	}
}
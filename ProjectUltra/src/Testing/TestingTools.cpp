#include <random>
#include <array>

#include "TestingTools.h"
#include "../Enigma/Enigma.h"

using namespace std;

int compareNumbersPassFail(vector<int> plainnumbers, vector<int> testnumbers)
{
    for (int i = 0; i < signed(testnumbers.size()); i++) if (plainnumbers[i] != testnumbers[i]) return i;
    return -1;
}

vector<vector<int>> encryptRandoms(int n, vector<int> plainnumbers, vector<int> reflectorPossibilites, vector<int> extraPossibilities, vector<int> rotorPossibilities, int numberOfPugs, unsigned int seed)
{
    if (numberOfPugs > 13) throw 21;

    vector<vector<int>> result;

    mt19937 gen(seed);
    uniform_int_distribution<int> reflectorDist(0, reflectorPossibilites.size() - 1);
    uniform_int_distribution<int> extraDist(0, extraPossibilities.size() - 1);
    uniform_int_distribution<int> rotorDist(0, rotorPossibilities.size() - 1);
    uniform_int_distribution<int> posDist(0, 25);

    for (; n > 0; n--)
    {
        //Generator reflector and extra
        int reflectorSetting[4];
        reflectorSetting[0] = reflectorPossibilites[reflectorDist(gen)];
        reflectorSetting[1] = extraPossibilities[extraDist(gen)];
        reflectorSetting[2] = posDist(gen);
        reflectorSetting[2] = posDist(gen);

        //Generate rotors, settings and positions
        int rotorSetting[3][3];
        bool rotorAlreadyChosen[9] = { 0 };
        for (int i = 0; i < 3; i++)
        {
            while (true)
            {
                rotorSetting[i][0] = rotorPossibilities[rotorDist(gen)];
                if (!rotorAlreadyChosen[rotorSetting[i][0]])
                {
                    rotorAlreadyChosen[rotorSetting[i][0]] = true;
                    break;
                }
            }
            rotorSetting[i][1] = posDist(gen);
            rotorSetting[i][2] = posDist(gen);
        }

        //Generate plugs
        vector<array<int, 2>> plug;
        bool plugAlreadyChosen[26] = { 0 };
        for (int i = 0; i < numberOfPugs; i++)
        {
            array<int, 2> tempPlug;
            while (true)
            {
                tempPlug[0] = posDist(gen);
                if (!plugAlreadyChosen[tempPlug[0]])
                {
                    plugAlreadyChosen[tempPlug[0]] = true;
                    break;
                }
            }
            while (true)
            {
                tempPlug[1] = posDist(gen);
                if (!plugAlreadyChosen[tempPlug[1]])
                {
                    plugAlreadyChosen[tempPlug[1]] = true;
                    break;
                }
            }
            plug.push_back(tempPlug);
        }

        //Code
        enigma machine;
        machine.initialise(rotorSetting, reflectorSetting, plug);
        vector<int> ciphernumbers = plainnumbers;
        machine.code(ciphernumbers);
        result.push_back(ciphernumbers);
    }
    return result;
}
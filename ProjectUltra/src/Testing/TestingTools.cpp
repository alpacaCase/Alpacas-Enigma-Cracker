#include <random>
#include <array>

#include "TestingTools.h"

using namespace std;

int compareNumbersPassFail(vector<int> plainnumbers, vector<int> testnumbers)
{
    for (int i = 0; i < signed(testnumbers.size()); i++) if (plainnumbers[i] != testnumbers[i]) return i;
    return -1;
}

vector<enigmaSetting> genRandomSettings(int n, vector<int> reflectorPossibilites, vector<int> extraPossibilities, vector<int> rotorPossibilities, int numberOfPlugs, unsigned int seed)
{
    vector<enigmaSetting> result;

    //Set up random distributions
    mt19937 gen(seed);
    uniform_int_distribution<int> reflectorDist(0, reflectorPossibilites.size() - 1);
    uniform_int_distribution<int> extraDist(0, extraPossibilities.size() - 1);
    uniform_int_distribution<int> rotorDist(0, rotorPossibilities.size() - 1);
    uniform_int_distribution<int> posDist(0, 25);

    for (; n > 0; n--)
    {
        enigmaSetting tempSetting;

        //Generator reflector and extra
        tempSetting.reflector[0] = reflectorPossibilites[reflectorDist(gen)];
        tempSetting.reflector[1] = extraPossibilities[extraDist(gen)];
        tempSetting.reflector[2] = posDist(gen);
        tempSetting.reflector[2] = posDist(gen);

        //Generate rotors, settings and positions
        bool rotorAlreadyChosen[9] = { 0 };
        for (int i = 0; i < 3; i++)
        {
            while (true)
            {
                tempSetting.rotors[i][0] = rotorPossibilities[rotorDist(gen)];
                if (!rotorAlreadyChosen[tempSetting.rotors[i][0]])
                {
                    rotorAlreadyChosen[tempSetting.rotors[i][0]] = true;
                    break;
                }
            }
            tempSetting.rotors[i][1] = posDist(gen);
            tempSetting.rotors[i][2] = posDist(gen);
        }

        //Generate plugs
        bool plugAlreadyChosen[26] = { 0 };
        for (int i = 0; i < numberOfPlugs; i++)
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
            tempSetting.plug.push_back(tempPlug);
        }

        //Add to final vector
        result.push_back(tempSetting);
    }

    return result;
}

vector<vector<int>> encryptRandoms(int n, vector<int> plainnumbers, vector<int> reflectorPossibilites, vector<int> extraPossibilities, vector<int> rotorPossibilities, int numberOfPlugs, unsigned int seed)
{
    if (numberOfPlugs > 13) throw 21;

    vector<vector<int>> result;
    
    vector<enigmaSetting> settings = genRandomSettings(n, reflectorPossibilites, extraPossibilities, rotorPossibilities, numberOfPlugs, seed);

    for (unsigned int i = 0; i < settings.size(); i++)
    {
        //Code
        enigma machine;
        machine.set(settings[i]);
        vector<int> ciphernumbers = plainnumbers;
        machine.code(ciphernumbers);
        result.push_back(ciphernumbers);
    }
    return result;
}
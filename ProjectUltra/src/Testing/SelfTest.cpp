#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>

#include "SelfTest.h"
#include "TestingTools.h"
#include "../Enigma/Enigma.h"
#include "../UI/UI.h"

using namespace std;

void readSetting(string settingsString, int rotorSettings[][3], int reflectorSettings[], vector<array<int, 2>>& plug)
{
    istringstream iss(settingsString);
    string tempString;

    //Rotor setting
    for (int i = 0; i < 3; i++)
    {
        for (int x = 0; x < 3; x++)
        {
            getline(iss, tempString, ' ');
            rotorSettings[i][x] = stoi(tempString);
        }
    }

    //Reflector setting
    getline(iss, tempString, ' ');
    reflectorSettings[0] = tempString[0];
    getline(iss, tempString, ' ');
    reflectorSettings[1] = tempString[0];
    getline(iss, tempString, ' ');
    reflectorSettings[2] = stoi(tempString);
    getline(iss, tempString, ' ');
    reflectorSettings[3] = stoi(tempString);

    //Plug setting
    array<int, 2> tempArray;
    plug.clear();
    while (getline(iss, tempString, ' '))
    {
        tempArray[0] = stoi(tempString);
        getline(iss, tempString, ' ');
        tempArray[1] = stoi(tempString);
        plug.push_back(tempArray);
    }
}

void selfTest()
{
    cout << "\nSELF TEST\n\n";
    
    //Load in plaintext and settings
    string path = "Data/SelfTestData/";
    string settingsFilename = "Settings.txt";
    string settingsPath = path + settingsFilename;
    string plaintextPath = path;
    string ciphertextPath;

    //Set up machine and variables
    int rotorSetting[3][3];
    int reflectorSetting[4];
    vector<array<int, 2>> plug;
    enigma machine;
    int passFail;
    int testIndex = 0;
    int numberOfFails = 0;

    //Open settings
    ifstream settingsFile(settingsPath);
    string settingString;
    string fileName;
    string result;


    //Find plain text file
    getline(settingsFile, settingString);
    plaintextPath.append(settingString);

    //Read in plaintext
    vector<int> plainnumbers = stringToNumbers(readFile(plaintextPath));
    vector<int> ciphernumbers;

    //Loop for each line in file
    while (getline(settingsFile, settingString))
    {
        //Read in ciphertext
        ciphertextPath = path + to_string(testIndex) + ".txt";
        ciphernumbers = stringToNumbers(readFile(ciphertextPath));

        //Copy and encode
        readSetting(settingString, rotorSetting, reflectorSetting, plug);
        machine.initialise(rotorSetting, reflectorSetting, plug);
        machine.code(ciphernumbers);

        //Test
        passFail = compareNumbersPassFail(plainnumbers, ciphernumbers);
        if (passFail != -1)
        {
            cout << "Test " << testIndex << " FAILLED\nFirst deviation at index " << passFail << "\nPress enter to continue...";
            numberOfFails++;
            cin >> settingString;
        }
        else
        {
            cout << "Test " << paddedNumber(testIndex, 4) << " PASSED\n";
        }

        testIndex++;
    }
    //Close settings
    settingsFile.close();

    if (numberOfFails == 0) cout << "All tests passed\n\n";
    else cout << numberOfFails << " tests failled\n\n";
}
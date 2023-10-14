#pragma once

#include <vector>
#include <array>

#include "../UI/Log.h"

/*
plugboard stores plugboard setting and uses it to code ints
*/
struct plugboard
{
    //Mapping is an array that maps input int (index) to output int (value)
    int mapping[26];


    /*
    REQUIRED sets up plugboard with plugs between the pairs of ints in plug
    No checking is done to prevent a letter being plugged twice
    */
    void set(std::vector<std::array<int, 2>>& plug);

    /*
    Returns the result of passing plainNumber through the plugboard
    */
    int code(int plainNumber);
};

/*
reflector stores reflector mapping and uses it to code ints
In the case of b-thin and c-thin reflectors, reflector also stores the fourth rotors as these do not move
*/
struct reflector
{
    //Mapping is an array that maps input int (index) to output int (value)
    int mapping[26];

    /*
    REQUIRED sets up reflector
        relflectorType - 'A'/'B'/'C'/'b'/'c' selects reflector type, lower case indicates thin reflectors and requires beta or gamma rotors
        rotorType - 'T'/'G' additional fourth rotors type, ignored if not thin reflector
        rotorPosition - additional rotors position, ignored if not thin reflector
        ringSetting - additional rotors ring setting, note this is redundant for the fourth rotors here so for cracking should not be used as a variable, ignored if not thin reflector
    */
    void set(char reflectorType, char rotorType, int rotorPosition, int ringSetting);

    /*
    Returns the result of passing plainNumber through the reflector (and fourth rotors if applicable)
    */
    int code(int plainNumber);
};

/*
rotors stores the rotors mappings and positions, and uses them to code ints
*/
struct rotor
{
    //mapping[] is an array that maps input int (index) to output int (value)
    int mappingFwd[26];
    int mappingBack[26];
    //When rotors steps and starts on inNotch true the next rotors should also step
    bool inNotch[26];
    int position;
    int ringSetting;


    /*
    REQUIRED sets up the rotors
        rotorType - [1,8] selects rotors type
        startPosition - rotors start position
        startRingSetting - rotors ring setting
    */
    void set(int rotorType, int startPosition, int startRingSetting);

    /*
    Advances the rotors by one step
    */
    void step();

    /*
    Returns the result of passing plainNumber through the rotors right to left
    */
    int codeFwd(int plainNumber);


    /*
    Returns the result of passing plainNumber through the rotors left to right
    */
    int codeBack(int plainNumber);
};

/*
Lightweight struct to hold a setting
*/
struct enigmaSetting
{
    int rotors[3][3];
    int reflector[4];
    std::vector<std::array<int, 2>> plug;

    //Variables useful for attacks, computed indicates whether it has been tested and eval the evaluation
    bool computed;
    double eval;

    /*
    Logs the setting in record
    */
    void log(logbook& record, bool includeEval = true);
};

/*
Enigma contains rotors reflector and plugboard, uses them to encode/decode ints
*/
struct enigma
{
    //Rotors listed from left to right
    reflector R;
    rotor zero;
    rotor one;
    rotor two;
    plugboard P;


    /*
   REQUIRED sets enigma machine to these settings
   */
    void set(enigmaSetting);

    /*
    REQUIRED sets up the enigma machine with its settings
        rotors - size 3x3 array that contains {rotorType, startPosition, startRingSetting} for left rotors then middle then right rotors
        reflector - size 4 array that contains {reflectorType, rotorType, rotorPosition, ringSetting}
        plug - vector that contains pairs of plugboard plugs
    */
    void set(int rotorSetting[][3], int reflectorSetting[], std::vector<std::array<int, 2>>& plug);

    /*
    Steps the right rotors by one and steps the other rotors as appropriate
    */
    void stepRotors();

    /*
    Returns the result of passing a single number through the entire machine
    DOES NOT STEP THE ROTORS so equivalent to pressing a single key is stepRotors(); codeSingle();
    */
    int codeSingle(int plainNumber);

    /*
    Codes the entire vector of numbers using the machine's setting (stepping rotors as appropriate
    */
    void code(std::vector<int>& plainnumbers);

    /*
    Returns array that is result of passing every letter through the machine in current state without incrementing rotors
    */
    std::array<int, 26> makeMap();

    /*
    Function returns a settings arrray for the machine's current state
    Array is { left position, middle position, middle setting, right position, right setting }
    */
    std::array<int, 3> getSettingArray();
};

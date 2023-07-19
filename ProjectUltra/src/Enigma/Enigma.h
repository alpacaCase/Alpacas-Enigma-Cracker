#pragma once

#include <vector>
#include <array>

/*
Plugboard stores plugboard setting and uses it to code ints
*/
struct plugboard
{
    //Mapping is an array that maps input int (index) to output int (value)
    int mapping[26];


    /*
    REQUIRED sets up plugboard with plugs between the pairs of ints in plug
    No checking is done to prevent a letter being plugged twice
    */
    void initialise(std::vector<std::array<int, 2>>& plug);

    /*
    Returns the result of passing plainNumber through the plugboard
    */
    int code(int plainNumber);
};

/*
Reflector stores reflector mapping and uses it to code ints
In the case of b-thin and c-thin reflectors, reflector also stores the fourth rotor
*/
struct reflector
{
    //Mapping is an array that maps input int (index) to output int (value)
    int mapping[26];


    /*
    REQUIRED sets up reflector
        relflectorType - 'A'/'B'/'C'/'b'/'c' selects reflector type, lower case indicates thin reflectors and requires beta or gamma rotor
        rotorType - 'T'/'G' additional fourth rotor type, ignored if not thin reflector
        rotorPosition - additional rotor position, ignored if not thin reflector
        ringSetting - additional rotor ring setting, note this is entriely redundant for the fourth rotor here so for cracking should not be used as a variable, ignored if not thin reflector
    */
    void initialise(char reflectorType, char rotorType, int rotorPosition, int ringSetting);

    /*
    Returns the result of passing plainNumber through the reflector (and fourth rotor if applicable)
    */
    int code(int plainNumber);
};

/*
rotor stores the rotor mappings and positions, and uses them to code ints
*/
struct rotor
{
    //mapping[] is an array that maps input int (index) to output int (value)
    int mappingFwd[26];
    int mappingBack[26];
    //When rotor steps and starts on inNotch true the next rotor should also step
    bool inNotch[26];
    int position;
    int ringSetting;


    /*
    REQUIRED sets up the rotor
        rotorType - [1,8] selects rotor type
        startPosition - rotor start position
        startRingSetting - rotor ring setting
    */
    void initialise(int rotorType, int startPosition, int startRingSetting);

    /*
    Advances the rotor by one step
    */
    void step();

    /*
    Returns the result of passing plainNumber through the rotor right to left
    */
    int codeFwd(int plainNumber);


    /*
    Returns the result of passing plainNumber through the rotor left to right
    */
    int codeBack(int plainNumber);
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
    REQUIRED sets up the enigma machine with its settings
        rotorSetting - size 3x3 array that contains {rotorType, startPosition, startRingSetting} for left rotor then middle then right rotors
        reflectorSetting - size 4 array that contains {reflectorType, rotorType, rotorPosition, ringSetting}
        plug - vector that contains pairs of plugboard plugs
    */
    void initialise(int rotorSetting[][3], int reflectorSetting[], std::vector<std::array<int, 2>>& plug);

    /*
    Steps the right rotor by one and steps the other rotors as appropriate
    */
    void stepRotors();

    /*
    Returns the result of passing a single number through the entire machine
    DOES NOT STEP THE ROTORS so equivalent to pressing a single key is stepRotors(); codeSingle();
    */
    int codeSingle(int plainNumber);

    /*
    Codes the vector of numbers using the machine's setting
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

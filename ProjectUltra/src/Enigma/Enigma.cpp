#include <string>

#include "Enigma.h"

using namespace std;

/*
Function takes a string of 26 uppercase letters and turns it into an array of 26 numbers
stringMap - string of uppercase characters defining the map
mapping - the size 26 int array to output the map
*/
void stringToMapping(string stringMap, int mapping[])
{
    for (int i = 0; i < 26; i++) mapping[i] = stringMap[i] - 65;
}


void plugboard::initialise(vector<array<int, 2>>& plug)
{
    stringToMapping("ABCDEFGHIJKLMNOPQRSTUVWXYZ", mapping);
    for (unsigned int i = 0; i < plug.size(); i++)
    {
        mapping[plug[i][0]] = plug[i][1];
        mapping[plug[i][1]] = plug[i][0];
    }
}

int plugboard::code(int plainNumber)
{
    return mapping[plainNumber];
}


void reflector::initialise(char reflectorType, char rotorType, int rotorPosition, int ringSetting)
{
    //thin reflectors
    if (reflectorType == 'b')
    {
        int reflectorMapping[26];
        int rotorMappingFwd[26];
        int rotorMappingBack[26];
        stringToMapping("ENKQAUYWJICOPBLMDXZVFTHRGS", reflectorMapping);
        if (rotorType == 'T')
        {
            stringToMapping("LEYJVCNIXWPBQMDRTAKZGFUHOS", rotorMappingFwd);
            stringToMapping("RLFOBVUXHDSANGYKMPZQWEJICT", rotorMappingBack);
        }
        else
        {
            stringToMapping("FSOKANUERHMBTIYCWLQPZXVGJD", rotorMappingFwd);
            stringToMapping("ELPZHAXJNYDRKFCTSIBMGWQVOU", rotorMappingBack);
        }
        //combine reflector and rotor mappings
        for (int i = 0; i < 26; i++)
        {
            mapping[i] = (rotorMappingFwd[(i + rotorPosition - ringSetting + 26) % 26] - rotorPosition + ringSetting + 26) % 26;
            mapping[i] = reflectorMapping[mapping[i]];
            mapping[i] = (rotorMappingBack[(mapping[i] + rotorPosition - ringSetting + 26) % 26] - rotorPosition + ringSetting + 26) % 26;
        }
    }

    else if (reflectorType == 'c')
    {
        int reflectorMapping[26];
        int rotorMappingFwd[26];
        int rotorMappingBack[26];
        stringToMapping("RDOBJNTKVEHMLFCWZAXGYIPSUQ", reflectorMapping);
        if (rotorType == 'T')
        {
            stringToMapping("LEYJVCNIXWPBQMDRTAKZGFUHOS", rotorMappingFwd);
            stringToMapping("RLFOBVUXHDSANGYKMPZQWEJICT", rotorMappingBack);
        }
        else
        {
            stringToMapping("FSOKANUERHMBTIYCWLQPZXVGJD", rotorMappingFwd);
            stringToMapping("ELPZHAXJNYDRKFCTSIBMGWQVOU", rotorMappingBack);
        }
        //combine reflector and rotor mappings
        for (int i = 0; i < 26; i++)
        {
            mapping[i] = (rotorMappingFwd[(i + rotorPosition - ringSetting + 26) % 26] - rotorPosition + ringSetting + 26) % 26;
            mapping[i] = reflectorMapping[mapping[i]];
            mapping[i] = (rotorMappingBack[(mapping[i] + rotorPosition - ringSetting + 26) % 26] - rotorPosition + ringSetting + 26) % 26;
        }
    }

    //normal reflectors
    else 
    {
        switch (reflectorType)
        {
        case 'A':
            stringToMapping("EJMZALYXVBWFCRQUONTSPIKHGD", mapping);
            break;
        case 'B':
            stringToMapping("YRUHQSLDPXNGOKMIEBFZCWVJAT", mapping);
            break;
        case 'C':
            stringToMapping("FVPJIAOYEDRZXWGCTKUQSBNMHL", mapping);
            break;
        }
    }
}

int reflector::code(int plainNumber)
{
    return mapping[plainNumber];
}


void rotor::initialise(int rotorType, int startPosition, int startRingSetting)
{
    for (int i = 0; i < 26; i++) inNotch[i] = false;
    switch (rotorType)
    {
    case 1:
        stringToMapping("EKMFLGDQVZNTOWYHXUSPAIBRCJ", mappingFwd);
        stringToMapping("UWYGADFPVZBECKMTHXSLRINQOJ", mappingBack);
        inNotch[16] = true;
        break;
    case 2:
        stringToMapping("AJDKSIRUXBLHWTMCQGZNPYFVOE", mappingFwd);
        stringToMapping("AJPCZWRLFBDKOTYUQGENHXMIVS", mappingBack);
        inNotch[4] = true;
        break;
    case 3:
        stringToMapping("BDFHJLCPRTXVZNYEIWGAKMUSQO", mappingFwd);
        stringToMapping("TAGBPCSDQEUFVNZHYIXJWLRKOM", mappingBack);
        inNotch[21] = true;
        break;
    case 4:
        stringToMapping("ESOVPZJAYQUIRHXLNFTGKDCMWB", mappingFwd);
        stringToMapping("HZWVARTNLGUPXQCEJMBSKDYOIF", mappingBack);
        inNotch[9] = true;
        break;
    case 5:
        stringToMapping("VZBRGITYUPSDNHLXAWMJQOFECK", mappingFwd);
        stringToMapping("QCYLXWENFTZOSMVJUDKGIARPHB", mappingBack);
        inNotch[25] = true;
        break;
    case 6:
        stringToMapping("JPGVOUMFYQBENHZRDKASXLICTW", mappingFwd);
        stringToMapping("SKXQLHCNWARVGMEBJPTYFDZUIO", mappingBack);
        inNotch[25] = true;
        inNotch[12] = true;
        break;
    case 7:
        stringToMapping("NZJHGRCXMYSWBOUFAIVLPEKQDT", mappingFwd);
        stringToMapping("QMGYVPEDRCWTIANUXFKZOSLHJB", mappingBack);
        inNotch[25] = true;
        inNotch[12] = true;
        break;
    case 8:
        stringToMapping("FKQHTLXOCBJSPDZRAMEWNIUYGV", mappingFwd);
        stringToMapping("QJINSAYDVKBFRUHMCPLEWZTGXO", mappingBack);
        inNotch[25] = true;
        inNotch[12] = true;
        break;
    default:
        throw 9;
    }
    position = startPosition;
    ringSetting = startRingSetting;
}

void rotor::step()
{
    position = (position + 1) % 26;
}

int rotor::codeFwd(int plainNumber)
{
    return (mappingFwd[(plainNumber + position - ringSetting + 26) % 26] - position + ringSetting + 26) % 26;
}

int rotor::codeBack(int plainNumber)
{
    return (mappingBack[(plainNumber + position - ringSetting + 26) % 26] - position + ringSetting + 26) % 26;
}


void enigma::initialise(int rotorSetting[][3], int reflectorSetting[], vector<array<int, 2>>& plug)
{
    zero.initialise(rotorSetting[0][0], rotorSetting[0][1], rotorSetting[0][2]);
    one.initialise(rotorSetting[1][0], rotorSetting[1][1], rotorSetting[1][2]);
    two.initialise(rotorSetting[2][0], rotorSetting[2][1], rotorSetting[2][2]);
    R.initialise(reflectorSetting[0], reflectorSetting[1], reflectorSetting[2], reflectorSetting[3]);
    P.initialise(plug);
}

void::enigma::stepRotors()
{
    if (one.inNotch[one.position]) { zero.step(); one.step(); }
    else if (two.inNotch[two.position]) one.step();
    two.step();
}

int enigma::codeSingle(int plainNumber)
{
    plainNumber = P.code(plainNumber);
    plainNumber = two.codeFwd(plainNumber);
    plainNumber = one.codeFwd(plainNumber);
    plainNumber = zero.codeFwd(plainNumber);
    plainNumber = R.code(plainNumber);
    plainNumber = zero.codeBack(plainNumber);
    plainNumber = one.codeBack(plainNumber);
    plainNumber = two.codeBack(plainNumber);
    plainNumber = P.code(plainNumber);
    return plainNumber;
}

void enigma::code(vector<int>& plainnumbers)
{
    for (unsigned int i = 0; i < plainnumbers.size(); i++)
    {
        stepRotors();
        plainnumbers[i] = codeSingle(plainnumbers[i]);
    }
}

array<int, 26> enigma::makeMap()
{
    array<int, 26> result;
    for (int i = 0; i < 26; i++) result[i] = codeSingle(i);
    return result;
}

array<int, 3> enigma::getSettingArray()
{
    array<int, 3> result;
    result[0] = zero.position;
    result[1] = one.position;
    result[2] = two.position;
    return result;
}
#pragma once

#include <vector>

#include "../Enigma/Enigma.h"

/*
Compares two number vectors, if they are identical returns -1 otherwise returns index where they first deviate
*/
int compareNumbersPassFail(std::vector<int> plainnumbers, std::vector<int> testnumbers);

/*
Function encrypts plainnumbers n times using random settings chosen from possibilities vectors
All vectors should have at least one element
Throws error 21 if number of plugs > 13
*/
std::vector<std::vector<int>> encryptRandoms(int n, std::vector<int> plainnumbers, std::vector<int> reflectorPossibilites, std::vector<int> extraPossibilities, std::vector<int> rotorPossibilities,
	int numberOfPugs, unsigned int seed = 3141);

std::vector<enigmaSetting> genRandomSettings(int n, std::vector<int> reflectorPossibilities, std::vector<int> extraPossibilities, std::vector<int> rotorPossibilities, int numberOfPlugs, unsigned int seed);
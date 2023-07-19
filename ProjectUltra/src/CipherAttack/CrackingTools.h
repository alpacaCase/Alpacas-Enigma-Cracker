#pragma once

#include <array>
#include <vector>

/*
Function takes setting in format: arrangement, position, setting and converts to rotorSetting and reflectorSetting
*/
void apsToSetting(std::array<int, 5>& arrangement, std::array<int, 4>& position, std::array<int, 4>& setting, int rotorSetting[][3], int reflectorSetting[]);

/*
Function takes vectors of all possibilities for each rotor/reflector in arrangements
Returns vector of all arrangments without any rotors being used twice
Must have one extra rotor possibility even if not using thin reflectors
Array has form {right rotor, middle rotor, left rotor, extra rotor, reflector}
*/
std::vector<std::array<int, 5>> generatePossibleArrangements(std::vector<int> reflectorPossibilities, std::vector<int> extraPossibilities, std::vector<int> zeroPossibilities, std::vector<int> onePossibilities, std::vector<int> twoPossibilities);
#pragma once

#include <vector>
#include <array>

/*
Function returns all the possible arrangments of the given rotors and reflectors without using any of them twice
two is the rightmost rotor
Output array has form {right rotors, middle rotors, left rotors, extra rotors, reflector}
!!!The extra possibilities must not be empty even when not using a thin rotor!!!
*/
std::vector<std::array<int, 5>> generateArrangements(std::vector<int> reflectorPossibilities, std::vector<int> extraPossibilities, std::vector<int> zeroPossibilities,
	std::vector<int> onePossibilities, std::vector<int> twoPossibilities);

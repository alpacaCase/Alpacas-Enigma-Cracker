#pragma once

#include <vector>
#include <array>
#include <string>

#include "../UI/Log.h"

/*
Function performs a cipher text only style attack on ciphernumbers, in each round it will search for the best rotor positions, then ring settings, then plugboards. 
It is capable of performing multiple rounds of attack as defined by searchInstructions and it outputs a vector of (hopefully) plaintextNumbers when finished.
->searchInstruction contains arrays of the form {search type ['I' - index of coicidence], number of arrangements to be kept for the next stage}
	3 of these are used per round for position, setting, plugboard search
	Having large numbers going to the next stage and multiple rounds obviously takes longer but may enable better results in marginal cases
	searchInstruction[-1][2][1] should be 1 to ensure the best is chosen at the end
Following give all possible selections for search
->reflectorPossibilities ('A'/'B'/'C'/'b'/'c')
->extraPossibilities ('T'/'G') SHOULD ALWAYS HAVE AT LEAST ONE OF THESE EVEN IF NEITHER c or b IS A REFLECTOR POSSIBILITY
->zeroPossibilities (1,2,3,4,5,6,7,8) etc.
->startingPlugs any known starting plugs
->record, a logbook, this function records "Start Crack" and "Stop Crack" at the begining and end
*/
std::vector<int> crack(std::vector<std::array<std::array<int, 2>, 3>> searchInstructions, std::vector<int> ciphernumbers, std::vector<int> reflectorPossibilities, std::vector<int> extraPossibilities,
	std::vector<int> zeroPossibilities, std::vector<int> onePossibilities, std::vector<int> twoPossibilities, std::vector<std::array<int, 2>> startingPlugs, logbook& record);

#pragma once

#include <vector>
#include <array>
#include <string>

/*
Function performs a cipher text only style attack on cipher numbers
	searchInstruction contains arrays of the form {search type ['I' - index of coicidence], number of apspe to go on to next round}
		3 of these are used for position, setting, plugboard search
		[-1][-1][1] should be 1 to ensure best is chosen at end
	Following give all possible selections for search
	reflectorPosibilities ('A'/'B'/'C'/'b'/'c')
	extraPosibilities ('T'/'G') SHOULD ALWAYS HAVE AT LEAST ONE OF THESE EVEN IF NEITHER c or b IS A REFLECTOR POSSIBILITY
	zeroPosibilities (1,2,3,4,5,6,7,8) etc.
	startingPlugs any known starting plugs
	logging ['S' - Silent no console logging / 'L' - Lite some console logging / 'V' - Verbose lots of console logging]
*/
std::vector<int> crack(std::vector<std::array<std::array<int, 2>, 3>> searchInstructions, std::vector<int> ciphernumbers, std::vector<int> reflectorPosibilities, std::vector<int> extraPosibilities,
	std::vector<int> zeroPosibilities, std::vector<int> onePosibilities, std::vector<int> twoPosibilities, std::vector<std::array<int, 2>> startingPlugs = {}, char logging = 'S');

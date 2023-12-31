#pragma once

#include <string>
#include <vector>
#include <array>

/*
Function runs bombe style attack for given parameters
	instructions - {[D - debug, lots of logging, L - Lite logging, S - Silent no logging], [1 full search, 0 normal search]}
	plaintext should be shorter than ciphertext but may be padded with spaces
	extraPossibilities must not be empty
	two is the rightmost rotor
returns plainnumber vector
Error 1:No circuits found
Error 2:Would generate more than 10e6 possibilities
Error 3:Generated no possible settings
*/
std::vector<int> bombe(std::array<int, 2> instructions, std::string plaintext, std::string ciphertext, std::vector<int> reflectorPosibilities, std::vector<int> extraPosibilities, 
	std::vector<int> zeroPosibilities, std::vector<int> onePosibilities, std::vector<int> twoPosibilities);
#pragma once

#include <vector>
#include <array>

#include "../Enigma/Enigma.h"

/*
Function searches for best (evaluated by index of coincidence score) rotor positions for given setting and ciphernumbers
*/
void searchPositionsIOC(std::vector<int>& ciphernumbers, enigmaSetting& setting);

/*
Function searches for best (evaluated by index of coincidence score) ring settings for given setting and ciphernumbers
*/
void searchSettingsIOC(std::vector<int>& ciphernumbers, enigmaSetting& setting);

/*
Function searches for best (evaluated by index of coincidence score) plugboard for given setting and ciphernumbers
*/
void searchPlugboardIOC(std::vector<int>& ciphernumbers, enigmaSetting& setting);

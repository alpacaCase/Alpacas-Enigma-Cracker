#pragma once

#include <vector>
#include <array>

#include "../Enigma/Enigma.h"

/*
Function searches through all possible rotors start positions for given ciphernumbers, arragement, setting and plug, stores best in bestPosition and stores best score in eval
*/
void searchPositionsIOC(std::vector<int>& ciphernumbers, enigmaSetting& setting);

/*
Function searches through possible ring settings for rotors two and one and stores them in bestSetting, also stores best IOC score in eval
Ineffectient; rewrite?
*/
void searchSettingsIOC(std::vector<int>& ciphernumbers, enigmaSetting& setting);

/*
Function searches through all posible plugboard pairs for given ciphernumbers, arrangement, position, setting and plug, stores best in plug and returns best IOC score
*/
void searchPlugboardIOC(std::vector<int>& ciphernumbers, enigmaSetting& setting);

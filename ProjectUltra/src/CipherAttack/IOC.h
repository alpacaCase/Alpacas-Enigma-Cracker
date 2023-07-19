#pragma once

#include <vector>
#include <array>

/*
Function searches through all possible rotor start positions for given ciphernumbers, arragement, setting and plug, stores best in bestPosition and stores best score in eval
*/
void searchPositionsIOC(std::vector<int>& ciphernumbers, std::array<int, 5>& arrangement, std::array<int, 4>& bestPosition, std::array<int, 4>& setting, std::vector<std::array<int, 2>>& plug, double& eval);

/*
Function searches through possible ring settings for rotors two and one and stores them in bestSetting, also stores best IOC score in eval
NEEDS REDO
*/
void searchSettingsIOC(std::vector<int>& ciphernumbers, std::array<int, 5>& arrangement, std::array<int, 4>& position, std::array<int, 4>& bestSetting, std::vector<std::array<int, 2>>& plug, double& eval);

/*
Function searches through all posible plugboard pairs for given ciphernumbers, arrangement, position, setting and plug, stores best in plug and returns best IOC score
*/
void searchPlugboardIOC(std::vector<int>& ciphernumbers, std::array<int, 5>& arrangement, std::array<int, 4>& position, std::array<int, 4>& setting, std::vector<std::array<int, 2>>& plug, double& eval);

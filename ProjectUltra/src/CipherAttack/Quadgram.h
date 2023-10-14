#pragma once

#include <string>
#include <vector>

/*
Function loads adn returns quadgrams from file at filename
*/
std::vector <std::vector<std::vector<std::vector<double>>>> loadQuadgrams(std::string filename);

/*
Function returns the qudgram fitness of numbers
*/
double quadScore(std::vector<int> numbers, std::vector<std::vector<std::vector<std::vector<double>>>>& quadgrams);
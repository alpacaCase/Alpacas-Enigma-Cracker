#pragma once

#include <string>
#include <vector>

/*
Function loads and returns quadgrams from file at filename
*/
std::vector <std::vector<std::vector<std::vector<double>>>> loadQuadgrams(std::string filename);

/*
Function returns the qudgram fitness of a vector of numbers
*/
double quadScore(std::vector<int> numbers, std::vector<std::vector<std::vector<std::vector<double>>>>& quadgrams);
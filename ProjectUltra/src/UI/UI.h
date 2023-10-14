#pragma once

#include <string>
#include <vector>
#include <array>

/*
Function returns true if input is upper case letter
If input is lower case letter function converts it to uppercase letter and returns true
Otherwise function returns false
*/
bool alphaToUpperCase(char& input);

/*
Function returns true if input is a letter
*/
bool isAlpha(char input);

/*
Function returns corresponding number for an upper or lower case character, or 26 if letter is not in the alphabet
*/
int letterToInt(char letter);

/*
Function returns user input of length numberOfCharacters
If user inputs wrong number of characters or doesnt only input letters they are prompted to try again
If allUpper is true all characters will be converted to uppercase
*/
std:: string getAlphaCharacters(int numberOfCharacters, bool allUpper = true);

/*
Function returns user input char
User is prompted to retry if they input more than one character or their choice is not in options
If allUpper options should only contain uppercase letters as any lower case user inputs will be treated as upper case
*/
char getOption(std::vector<char> options, bool allUpper = true);

/*
Function returns the entire contents of a file as a string, filename is taken as input from the user
*/
std::string getUserFile();

/*
Function returns a string only containing the letters in dirty string and all letters as uppercase
*/
std::string cleanString(std::string dirtyString);

/*
Function takes string and returns vector of corresponding numbers for every letter (will ignore any non letter characters)
*/
std::vector<int> stringToNumbers(std::string plainString);

/*
Function takes input from user, either letter or int in range [0,25] and returns int in range [0,25]
Will reprompt user for setting if the user makes a mistake
*/
int getSetting();

/*
Function int input from user in range [lowerBound, upperBound)
Prompts user to renenter if there is an error in the input
*/
int getNumber(int lowerBound, int upperBound);

/*
Function gets user to input two characters and adds them to plug array
Checks letters aren't plugged twice and prompts user to renter if there are any errors
*/
void getPlug(std::vector<std::array<int,2>>& plug);

/*
Function returns string of all corresponding uppercase letters for ciphernumbers
*/
std::string numberVectorToString(std::vector<int> ciphernumbers);

/*
Function returns string with a space every 5 letters
*/
std::string formatForOutput(std::string inputString);

/*
Function returns n as a string, if string is shorter than padTo string has spaces added to the front to make up the difference
*/
std::string paddedNumber(int n, int padTo = 2);

/*
Function reads entire file at filename and returns it as a string
Discards all \n characters and no error checking
*/
std::string readFile(std::string filename);

/*
Function writes contents to file at filename
*/
void saveFile(std::string filename, std::string contents);
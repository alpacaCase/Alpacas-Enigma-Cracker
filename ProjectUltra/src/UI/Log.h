#pragma once

#include <chrono>
#include <vector>
#include <string>

/*
######
The idea is that all messages to the console are entered as logs (at least during computation, for the moment I don't think it's necessary during user input), allowing easy timing of various aspects for benchmarking and an easy to activate debugging mode
######
*/

/*
A struct records a point in time and a string to indicate what happened at that time, and at what "logging level" [D,L,S] it should be outputted to console)
*/
struct logPoint
{
	std::chrono::steady_clock::time_point time;
	std::string descripton;
	char level;
};

/*
A log that records a vector of all logpoints, the times they were made, and allows calculations of the times between them
printingLevel [D,L,S], if set to D all logs are printed to the console, if set to S only logs flagged as S are printed to the conso
The first entry in the logbook is always "starting log" and its defualt printingLevel is 'L'
*/
struct logbook
{
	std::vector<logPoint> logPoints;
	char printingLevel;

	logbook();

	/*
	Adds a log point with description and the level at which it should be outputted to console
	*/
	void log(std::string description, char level = 'L');
	
	/*
	Returns the time interval in milliseconds between the most recent logPoint with end description and the previous logPoint with start as its description
	throws 50 if no such interval exists
	*/
	double timeInterval(std::string start, std::string end);

};

#pragma once

#include <chrono>
#include <vector>
#include <string>

/*
A struct records a point in time and a string to indicate what happened at that time, and at what "logging level" (D,L,S) it should be outputted to console)
*/
struct logPoint
{
	std::chrono::steady_clock::time_point time;
	std::string descripton;
	char level;
};

/*
A log that records a vector of all logpoints, the times they were made, and allows calculations of the times between them
Also automatically outputs log descriptions to the console depending on prinitingLevel (D - all logs are printed, L - light/some logs are printed, S - silent/only messages that should always display are printed)
*/
struct logbook
{
	std::vector<logPoint> logPoints;
	char printingLevel;

	/*
	Adds a log point with descritption and the level at which it should be outputted to console
	*/
	void log(std::string description, char level = 'L');
	
	/*
	Returns the time interval in milliseconds between the most recent logPoint with end description and the previous logPoint with start as its description
	throws 50 if no such interval exists
	*/
	double timeInterval(std::string start, std::string end);

};

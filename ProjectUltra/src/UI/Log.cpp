#include <iostream>

#include "Log.h"
#include "UI.h"

using namespace std;

void logbook::log(string description, char level)
{
	//Create temporary logpoint, store data then add to book
	logPoint result;
	result.time = chrono::high_resolution_clock::now();
	result.description = description;
	result.level = level;
	logPoints.push_back(result);

	//Print to console
	if (level >= printingLevel) cout << description << "\n";
}


int logbook::findIndex(string description)
{
	for (int index = signed(logPoints.size()) - 1; index >= 0; index--)
		if (logPoints[index].description == description)
			return index;
	throw 50;
}

double logbook::timeInterval(string start, string end)
{
	chrono::duration<double, milli> interval;
	interval = logPoints[findIndex(end)].time - logPoints[findIndex(start)].time;
	return interval.count();
}

void logbook::saveLog(string fileName, string start, string end)
{
	string contents = "";
	int startIndex = findIndex(start);
	int endIndex = findIndex(end);

	if (startIndex > endIndex) throw 51;

	for (int i = startIndex; i <= endIndex; i++)
	{
		contents += logPoints[i].description;
		contents += "\n";
	}

	saveFile(fileName, contents);
}

logbook::logbook()
{
	printingLevel = 'L';
	log("Starting log", 'D');
}
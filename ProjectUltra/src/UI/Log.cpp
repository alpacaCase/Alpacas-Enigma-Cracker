#include <iostream>

#include "Log.h"

using namespace std;

void logbook::log(string description, char level)
{
	logPoint result;
	result.time = chrono::high_resolution_clock::now();
	result.descripton = description;
	result.level = level;
	logPoints.push_back(result);

	if (level >= printingLevel) cout << description << "\n";
}

double logbook::timeInterval(std::string start, std::string end)
{
	chrono::duration<double, milli> interval;
	for (int i = logPoints.size() - 1; i > 0; i--)
	{
		if (logPoints[i].descripton == end)
		{
			for (int j = i - 1; j >= 0; j--)
			{
				if (logPoints[j].descripton == start)
				{
					interval = logPoints[i].time - logPoints[j].time;
					return interval.count();
				}
			}
			throw 50;
		}
	}
	throw 50;
}


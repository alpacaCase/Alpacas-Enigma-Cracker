#include <algorithm>

#include "CipherGraph.h"
#include "../UI/UI.h"

using namespace std;

/*
Function takes vector from dfs tree and starts at the end
If it finds a repeat of the last node label it cuts the vector there and removes the end to give a loop vector
Otherwise if there is no loop in the vector it makes the vector {{-1,-1}{-1,-1}}
*/
void cutAtLoop(vector<array<int, 2>>& loop)
{
	int i = loop.size() - 1;
	int loopStartCharacter = loop[i][0];
	do
	{
		i--;
	} while (i > -1 && loop[i][0] != loopStartCharacter);

	if (i > -1)
	{
		loop.erase(loop.begin(), loop.begin() + i);
		loop.pop_back();
	}
	else
	{
		loop = { {-1,-1}, {-1,-1} };
	}
}

/*
Function returns true if j is bigger than i
*/
bool isLongerLoop(vector<array<int, 2>> i, vector<array<int, 2>> j)
{
	return (i.size() < j.size());
}


char numberToLetter(int number)
{
	return number + 65;
}

int letterToNumber(char letter)
{
	return letter - 65;
}

void cipherGraph::addLink(int index, int a, int b)
{
	nodes[a].links.push_back({ b, index });
	nodes[b].links.push_back({ a, index });
}

void cipherGraph::decolour()
{
	for (int i = 0; i < 26; i++) nodes[i].coloured = 0;
}

void cipherGraph::DFS(int currentNode, vector<array<int, 2>>& currentPath)
{
	//If have already visited node add tree branch to circuits and then go back one level of recursion
	if (nodes[currentNode].coloured)
	{
		currentPath.push_back({ currentNode, -1 });
		circuits.push_back({ currentPath });
		currentPath.pop_back();
	}
	
	//Otherwise
	else
	{
		//Mark node as visited
		nodes[currentNode].coloured = 1;

		//Loop through all links from this node
		for (unsigned int i = 0; i < nodes[currentNode].links.size(); i++)
		{
			currentPath.push_back({ currentNode, nodes[currentNode].links[i][1] });
			DFS(nodes[currentNode].links[i][0], currentPath);
			currentPath.pop_back();
		}
	}
}

void cipherGraph::findCircuits()
{
	decolour();
	circuits.clear();
	vector<array<int, 2>> currentPath;
	vector<vector<array<int, 2>>> tempLoops;

	//Perform DFS to create tree in circuits
	for (int i = 0; i < 26; i++)
	{
		if (!nodes[i].coloured) DFS(i, currentPath);
	}

	//Cuts tree branches so they are circuits
	for (unsigned int i = 0; i < circuits.size(); i++)
	{
		cutAtLoop(circuits[i]);
	}

	//Remove all non circuits
	tempLoops.clear();
	for (unsigned int i = 0; i < circuits.size(); i++)
	{
		if (circuits[i][0][1] != circuits[i][1][1]) tempLoops.push_back(circuits[i]);
	}
	circuits = tempLoops;

	//Sort circuits so shortest is first
	sort(circuits.begin(), circuits.end(), isLongerLoop);
}

void cipherGraph::buildGraph(string plainText, string cipherText)
{
	int a;
	int b;
	for (int i = 0; i < signed(plainText.length()); i++)
	{
		//Avoid padding spaces
		if (plainText[i] != ' ')
		{
			a = letterToNumber(plainText[i]);
			b = letterToNumber(cipherText[i]);
			addLink(i, a, b);
		}
	}
	cribLength = signed(plainText.length());

	findCircuits();
}

string cipherGraph::debugLinksString()
{
	string result = "";
	for (int i = 0; i < 26; i++)
	{
		result += "Letter ";
		result += numberToLetter(i);
		result += " is linked with:\n";
		for (unsigned int j = 0; j < nodes[i].links.size(); j++)
		{
			result += numberToLetter(nodes[i].links[j][0]);
			result += " at ";
			result += paddedNumber(nodes[i].links[j][1]);
			result += "\n";
		}
		result += "\n";
	}
	return result;
}

string cipherGraph::debugCircuitsString()
{
	string result = "";
	for (unsigned int i = 0; i < circuits.size(); i++)
	{
		result += "Loop ";
		result += paddedNumber(i);
		result += ": ";
		for (unsigned int j = 0; j < circuits[i].size(); j++)
		{
			result += numberToLetter(circuits[i][j][0]);
			result += " ";
			result += paddedNumber(circuits[i][j][1]);
			result += "  ";
		}
		result += "\n";
	}
	return result;
}



#pragma once

#include <vector>
#include <array>
#include <string>
#include <deque>

struct node
{
	//Link arrays are of the form { Node this links to, edge label }
	//Nodes are labeled [0,25]
	std::vector<std::array<int, 2>> links = {};
	
	//1 if coloured, 0 if not, used to make sure nodes aren't visited twice
	int coloured = 0;
};

/*
Struct used to find all the circuits in a given plaintext ciphertext combo using buildGraph
Contains 26 nodes, 1 for each letter and a vector of all circuits generated when buildGraph is run
*/
struct cipherGraph
{
	std::array<node, 26> nodes;
	std::vector<std::vector<std::array<int, 2>>> circuits;
	int cribLength = 0;

	/*
	Function adds a link with edge label index between nodes a and b
	Links are bidirectional and stored in both nodes
	*/
	void addLink(int index, int a, int b);

	/*
	Decolours all nodes in cipherGraph
	*/
	void decolour();

	/*
	Function performs recursive depth first search on graph to build a tree and adds a vector to circuits for each branch of tree
	circuits should be cleared before use
	Final link will have edge label -1
	*/
	void DFS(int currentNode, std::vector<std::array<int, 2>>& currentPath);

	/*
	Function fills vector circuits with all circuits in the cipherGraph
	*/
	void findCircuits();

	/*
	Function creates a new cipherGraph for plaintext and ciphertext combination
	plaintext should be smaller than ciphertext and function does not check for crashes
	plaintext may be padded by spaces at the start
	*/
	void buildGraph(std::string plainText, std::string cipherText);

	/*
	Function returns a string containing all the linking information of the cipherGraph
	*/
	std::string debugLinksString();

	/*
	Function returns a string containing all the circuits information in the cipherGraph
	*/
	std::string debugCircuitsString();

};
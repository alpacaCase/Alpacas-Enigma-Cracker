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
	//1 if coloured, 0 if not
	int coloured = 0;
};

struct cipherGraph
{
	std::array<node, 26> nodes;
	std::vector<std::vector<std::array<int, 2>>> loops;
	int cribLength = 0;

	/*
	Function adds a link with edge label index between nodes a and b
	*/
	void addLink(int index, int a, int b);

	/*
	Decolours all nodes in cipherGraph
	*/
	void decolour();

	/*
	Function performs recursive depth first search on graph and adds vector to loops for each branch of tree
	loops should be cleared before use
	Final link will have edge label -1
	*/
	void DFS(int currentNode, std::vector<std::array<int, 2>>& currentPath);

	/*
	Function fills vector loops with all loops in the cipherGraph
	*/
	void findLoops();

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
	Function returns a string containing all the loops information in the cipherGraph
	*/
	std::string debugLoopsString();

};
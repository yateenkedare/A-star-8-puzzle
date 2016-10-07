#include "stdafx.h"
#include <iostream>
#include <deque>
#include <algorithm>
#include <vector>
using namespace std;
int initialState[3][3] = { { 0,0,0 },
							{ 0,0,0 },
							{ 0,0,0 } };

int goalState[3][3] = { { 0,0,0 },
						{ 0,0,0 },
						{ 0,0,0 } };

/*
	Description : A node describes tile positions of a particular 
	state and it also has its 'g' and 'h' values and the path from root
*/
class node {
public:
	int g, h;
	int puzzleBoard[3][3];
	vector<int> pathFromRoot;
	node() {}
	node(int board[3][3], int depth,vector<int> parentPathVector,int swapDirection) {
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				puzzleBoard[i][j] = board[i][j];
		g = depth;
		calculateH();
		pathFromRoot.swap(parentPathVector);
		pathFromRoot.emplace_back(swapDirection);
	}
	node(int board[3][3], int depth, vector<int> parentPathVector) {
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				puzzleBoard[i][j] = board[i][j];
		g = depth;
		calculateH();
		pathFromRoot.swap(parentPathVector);
	}
	node(int board[3][3], int depth) {
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				puzzleBoard[i][j] = board[i][j];
		g = depth;
		calculateH();
	}
	void calculateH() {
		h = 0;
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				h += ManhattanDistance(i, j);
	}

	int ManhattanDistance(int posI, int posJ) {
		if (puzzleBoard[posI][posJ] == 0)
			return 0;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (puzzleBoard[posI][posJ] == goalState[i][j])
				{
					return (abs(posI - i) + abs(posJ - j));
				}
			}
		}
		return 0;
	}
	int getF() {
		return g + h;
	}
	int getH() {
		calculateH();
		return h;
	}
};

/*
	Description : The solve function in this class can be called in order to 
	solve the puzzle and find the solution.
*/
class A_Star {
public:
	int fBestNode;
	A_Star()
	{
		fBestNode = 0;
	}
	deque<node> closedList;
	vector<node> openList;
	node tempNode, BestNode;
	void solvePuzzle()
	{
		int posOfBestNode = 0, posOfIterator = 0;
		node rootNode(initialState, 0);
		openList.emplace_back(rootNode);
		while (!openList.empty())
		{
			//Find best node in openList
			posOfBestNode = 0;
			posOfIterator = 0;
			fBestNode = openList.front().getF();
			for (auto &j : openList)
			{
				if (j.getF() <= fBestNode)
				{
					fBestNode = j.getF();
					posOfBestNode = posOfIterator;
					BestNode = node(j.puzzleBoard, j.g, j.pathFromRoot);
				}
				posOfIterator++;
			}
			openList.erase(openList.begin() + posOfBestNode);	//poping best node from open list
			closedList.emplace_back(BestNode);					//pushing best node in closed list
			//checking if the best node is the goal
			if (checkSimilarity(closedList.back().puzzleBoard, goalState))
			{
				//just some stuff to display the output in fancy way
				cout << " :: Total Child nodes generated"<<endl;
				cout << "Path cost to solution :: " << closedList.back().pathFromRoot.size() << endl;
				displayBoard(initialState);
				for (int k = 0; k < closedList.back().pathFromRoot.size(); k++)
				{
					findZeroPosition(initialState);
					if (closedList.back().pathFromRoot[k] == 1)
					{
						cout << " swap up::\n";
						swap(initialState[iZero][jZero], initialState[iZero - 1][jZero]);
					}
					else if (closedList.back().pathFromRoot[k] == 2)
					{
						cout << " swap right::\n";
						swap(initialState[iZero][jZero], initialState[iZero][jZero + 1]);
					}
					else if (closedList.back().pathFromRoot[k] == 3)
					{
						cout << " swap down::\n";
						swap(initialState[iZero][jZero], initialState[iZero + 1][jZero]);
					}
					else if (closedList.back().pathFromRoot[k] == 4)
					{
						cout << " swap left::\n";
						swap(initialState[iZero][jZero], initialState[iZero][jZero - 1]);
					}
					displayBoard(initialState);
				}
				return;
			}

			//Generating successors
			findZeroPosition(closedList.back().puzzleBoard);
			if (iZero > 0)		//swap up
			{
				copyToTemp();
				swap(tempBoard[iZero][jZero], tempBoard[iZero - 1][jZero]);
				childNodeProcess(1);
			}
			if (iZero < 2)		//swap down
			{
				copyToTemp();
				swap(tempBoard[iZero][jZero], tempBoard[iZero + 1][jZero]);
				childNodeProcess(3);
			}
			if (jZero > 0)		//swap left
			{
				copyToTemp();
				swap(tempBoard[iZero][jZero], tempBoard[iZero][jZero - 1]);
				childNodeProcess(4);
			}
			if (jZero < 2)		//swap right
			{
				copyToTemp();
				swap(tempBoard[iZero][jZero], tempBoard[iZero][jZero + 1]);
				childNodeProcess(2);
			}
		}
	}

private:
	int iZero, jZero, totalChildNodes;
	int tempBoard[3][3];
	//finding the position of zero for 
	void findZeroPosition(int board[3][3])
	{
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				if (board[i][j] == 0)
				{
					iZero = i;
					jZero = j;
					return;
				}
	}
	//used for displaying a particular puzzle board
	void displayBoard(int puzBoard[3][3])
	{
		for (int z = 0; z < 3; z++)
		{
			for (int y = 0; y < 3; y++)
				cout << "  " << puzBoard[z][y];
			cout << endl;
		}
		cout << endl;
	}
	//this function is used while swapping for maintaining the parent 
	//(not really usefull its just written to make the code shorter)
	void copyToTemp()
	{
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				tempBoard[i][j] = closedList.back().puzzleBoard[i][j];
	}
	//Generates the successor nodes and checks them with nodes in open and closed list
	//inserts only leagal nodes in the open list
	void childNodeProcess(int swapDir)
	{
		
		int pos = 0;
		tempNode = node(tempBoard, closedList.back().g + 1, closedList.back().pathFromRoot, swapDir);
		if (closedList.size() >= 2)		//check parent--here parent is not being checked and repeated states are being generated
			if (checkSimilarity(tempBoard, closedList.rbegin()[1].puzzleBoard))			//checking if generated node and parent node are equal
				return;
		for (auto &j : openList)		//if new node is already present in open list then check which path is shorter
		{
			if (checkSimilarity(tempNode.puzzleBoard, j.puzzleBoard))
			{
				if (tempNode.g < j.g)
				{
					openList.erase(openList.begin() + pos);	//check similar boards in the open list
					break;
				}
				else
					return;									//if similar node found in open list and value is 'f' more
			}
			pos++;
		}
		openList.emplace_back(tempNode);
	//	totalChildNodes++;
	//	cout << '\r' << totalChildNodes << flush;
	}

	//checks if two arrays are equal
	bool checkSimilarity(int a[3][3], int b[3][3])
	{
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				if (a[i][j] != b[i][j])
					return false;
		return true;
	}
};

int main()
{
	cout << "Enter Initial State :: ";

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			cin >> initialState[i][j];

	cout << "Enter Goal State :: ";

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			cin >> goalState[i][j];

	cout << "--- Exploring Child nodes ---"<< endl;
	A_Star puzzle;
	puzzle.solvePuzzle();

	system("pause");
	return 0;
}
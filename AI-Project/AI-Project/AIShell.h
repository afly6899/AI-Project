#ifndef AISHELL_H
#define AISHELL_H

#pragma once
#include "Move.h"

//#include <fstream>
#include <vector>
#include <map>
#include <ctime>

// Outside of class AIShell:
extern int INF;

struct moveCost
{
	int util;
	int col;
	int row;
};

struct Winner
{
	bool endGame;
	int winner;
};

int checkColumn(int** state, int numCols, int numRows, int depth, int k, bool gravityOn, int testedPlayer);
int checkRow(int** state, int numCols, int numRows, int depth, int k, bool gravityOn, int testedPlayer);
int checkLeftDiagonal(int** state, int numCols, int numRows, int depth, int k, bool gravityOn, int testedPlayer);
int checkRightDiagonal(int** state, int numCols, int numRows, int depth, int k, bool gravityOn, int testedPlayer);
int calculateUtility(int* AICounts, int* OPCounts, int depth, int k, int testedPlayer);


void printGame(int** state, int numCols, int numRows, int HUMAN_PIECE);//, std::ofstream &debugInfo);
void printState(int** state, int numCols, int numRows, int HUMAN_PIECE, int alpha, int beta, int col, int row, int utility);
// end Outside of class AIShell

// A new AIShell will be created for every move request.
class AIShell{

public:
	//these represent the values for each piece type.
	static const int AI_PIECE=1;
	static const int HUMAN_PIECE = -1;
	static const int NO_PIECE=0;


private:
	//Do not alter the values of numRows or numcols.
	//they are used for deallocating the gameState variable.
	int numRows; //the total number of rows in the game state.
	int numCols; //the total number of columns in the game state.
	int **gameState; //a pointer to a two-dimensional array representing the game state.
	bool gravityOn; //this will be true if gravity is turned on. It will be false if gravity is turned off.
	Move lastMove; //this is the move made last by your opponent. If your opponent has not made a move yet (you move first) then this move will hold the value (-1, -1) instead.


	// Trying to add hash table things - using Zobrist hashing
	static bool zobristNotInitialized;
	static unsigned long long currentHash;
	static unsigned long long* zobristNumbers[];
	static std::map<unsigned long long, int> calculatedUtilities;

	void initializeZobrist();
	unsigned long long zobristHash(int** state);
	unsigned long long zobristHash(int col, int row, int player, bool undo = false);

	// Trying to add time counting stuff
	time_t begin;

public:
	int deadline; //this is how many milliseconds the AI has to make move.
	int k;        // k is the number of pieces a player must get in a row/column/diagonal to win the game. IE in connect 4, this variable would be 4

	// Edited
	int numPieces;// this is how many pieces EACH player has played
	Move testedMove; // the move that the minimax is currently testing
	int testedPlayer;
	// end Edited

	AIShell(int numCols, int numRows, bool gravityOn, int** gameState, Move lastMove);
	~AIShell();

	// Edited
	struct Winner terminalTest(int** state, int lastPlayer);
	int utility(int** state, int depth);
	// end Edited

	// Alpha Beta Pruning Edits: adding alpha and beta as parameters
	//		and/or put the checking/pruning in
	Move minimax(int** state);
	int maxValue(int** state,int depth, int alpha, int beta);
	int minValue(int** state,int depth, int alpha, int beta);
	// end Alpha Beta Pruning Edits



	static unsigned long long primeNumbers[];
	static std::vector<unsigned long long> usedPrimeMultiples;


	Move makeMove();
};

#endif //AISHELL_H

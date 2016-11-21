#include "AIShell.h"
#include <algorithm>
#include <limits>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <math.h>
#pragma region Heuristics
// Adding this because it's a lot more concise than std::numeric_limits<int>::max()
int INF = std::numeric_limits<int>::max();
int numPieces;
time_t begin;
// Edited: the total count is calculated by calculateUtility (so we don't have to change so many
//	  functions at once), also added thing where if someone gets k in a row, only their utility
//	  matters. Also, depth is somewhat accounted for as well (see calculateUtility)
int checkColumn(int** state, int numCols, int numRows, int depth, int k, bool gravityOn, int testedPlayer)
{
	int totalColUtility = 0;
	if (numCols >= k) {

		int* AIcolCounts = new int[k];
		int* OPcolCounts = new int[k];
		for (int i = 0; i < k; i++) {
			AIcolCounts[i] = 0;
			OPcolCounts[i] = 0;
		}

		int numKCols = numRows - k + 1;
		for (int col = 0; col < numCols; col++) {
			for (int startRow = 0; startRow < numKCols; startRow++) {
				int AIcount = 0;
				int OPcount = 0;
				for (int spread = 0; spread < k; spread++) {
					/*if (gravityOn)
					{
					if (startRow + spread == numRows - 1) {
					if (AIcount < k) {
					AIcount = 0;
					}
					if (OPcount < k) {
					OPcount = 0;
					}
					break;
					}
					}*/
					if (state[col][startRow + spread] == 1) {
						if (OPcount > 0) { AIcount = OPcount = 0; break; }
						AIcount++;
						if (AIcount == 4 && state[col][startRow + spread-1] == 0 && state[col][startRow + spread+1] == 0)
						{
							//totalColUtility += 10000;
						}
					}
					else if (state[col][startRow + spread] == -1) {

						if (AIcount > 0) { AIcount = OPcount = 0; break; }
						OPcount++;
					}
				}
				if (AIcount > 0) { AIcolCounts[AIcount - 1]++; AIcount = 0; }
				if (OPcount > 0) { OPcolCounts[OPcount - 1]++; OPcount = 0; }
			}
		}
		totalColUtility = calculateUtility(AIcolCounts, OPcolCounts, depth, k, testedPlayer);

		delete AIcolCounts;
		delete OPcolCounts;
	}

	return totalColUtility;
}

int checkRow(int** state, int numCols, int numRows, int depth, int k, bool gravityOn, int testedPlayer)
{
	int totalRowUtility = 0;
	if (numRows >= k) {

		int* AIrowCounts = new int[k];
		int* OProwCounts = new int[k];
		for (int i = 0; i < k; i++) {
			AIrowCounts[i] = 0;
			OProwCounts[i] = 0;
		}

		int numKRows = numCols - k + 1;
		for (int row = 0; row < numRows; row++) {
			for (int startCol = 0; startCol < numKRows; startCol++) {
				int AIcount = 0;
				int OPcount = 0;
				for (int spread = 0; spread < k; spread++) {
					/*if (gravityOn)
					{
					if (startCol + spread == numCols - 1) {
					if (AIcount < k) {
					AIcount = 0;
					}
					if (OPcount < k) {
					OPcount = 0;
					}
					break;
					}
					}*/
					if (state[startCol + spread][row] == 1) {
						if (OPcount > 0) { AIcount = OPcount = 0; break; }
						AIcount++;
						if (AIcount == 4 && state[startCol + spread-1][row] == 0 && state[startCol + spread-1][row] == 0)
						{
							//totalRowUtility += 10000;
						}
					}
					else if (state[startCol + spread][row] == -1) {

						if (AIcount > 0) { AIcount = OPcount = 0; break; }
						OPcount++;
					}
				}
				if (AIcount > 0) { AIrowCounts[AIcount - 1]++; AIcount = 0; }
				if (OPcount > 0) { OProwCounts[OPcount - 1]++; OPcount = 0; }
			}
		}

		//if (AIrowCounts[k - 1] > 0) {
		//	for (int i = 0; i < k; i++) {
		//		totalRowUtility += ((AIrowCounts[i] * (i + 1)));
		//	}
		//}
		//else {
		//	for (int i = 0; i < k; i++) {
		//		totalRowUtility += ((AIrowCounts[i] * (i + 1))) - (OProwCounts[i] * (i + 1));
		//	}
		//}

		totalRowUtility = calculateUtility(AIrowCounts, OProwCounts, depth, k, testedPlayer);

		delete AIrowCounts;
		delete OProwCounts;
	}

	return totalRowUtility;
}

int checkLeftDiagonal(int** state, int numCols, int numRows, int depth, int k, bool gravityOn, int testedPlayer)
{
	int totalLtdUtility = 0;
	if (numCols >= k) {

		int* AIltdCounts = new int[k];
		int* OPltdCounts = new int[k];
		for (int i = 0; i < k; i++) {
			AIltdCounts[i] = 0;
			OPltdCounts[i] = 0;
		}


		int numKCols;

		for (int col = 0; col < numCols; col++) {
			numKCols = std::min(numRows, col + 1) - k + 1;
			if ((col + 1) >= k && (col + 1) <= numRows)
			{
				numKCols = (col + 1) - k + 1;
			}
			else if ((col + 1) >= k)
			{
				numKCols = numRows - k + 1;
			}
			int colMem = col;
			for (int startRow = 0; startRow < numKCols; startRow++)
			{
				int AIcount = 0;
				int OPcount = 0;
				for (int spread = 0; spread < k; spread++)
				{
					/*if (gravityOn)
					{
					if (col == 0 || startRow + spread == numRows - 1) {
					if (AIcount < k) {
					AIcount = 0;
					}
					if (OPcount < k) {
					OPcount = 0;
					}
					break;
					}
					}*/
					if (state[col][startRow + spread] == 1)
					{
						if (OPcount > 0) { AIcount = OPcount = 0; break; }
						AIcount++;
						if (col != 0 && col != numCols - 1 && startRow + spread != 0 && startRow + spread != numRows - 1)
						{
							if (AIcount == 4 && state[col - 1][startRow + spread - 1] == 0 && state[col + 1][startRow + spread + 1] == 0)
							{
								//totalLtdUtility += 10000;
							}
						}
					}
					else if (state[col][startRow + spread] == -1)
					{

						if (AIcount > 0) { AIcount = OPcount = 0; break; }
						OPcount++;
					}
					if (col > 0)
						col--;
				}
				col = colMem - (startRow + 1);
				if (col < 0)
					break;

				if (AIcount > 0) { AIltdCounts[AIcount - 1]++; AIcount = 0; }
				if (OPcount > 0) { OPltdCounts[OPcount - 1]++; OPcount = 0; }
			}
			col = colMem;

		}

		for (int row = 1; row < numRows; row++)
		{
			int col = numCols - 1;
			int colMem = col;
			numKCols = numRows - row - k + 1;
			if (numKCols > 0)
			{
				for (int startRow = row; startRow <= numKCols + 1; startRow++)
				{
					int AIcount = 0;
					int OPcount = 0;
					for (int spread = 0; spread < k; spread++)
					{
						/*if (gravityOn)
						{
						if (col == 0 || startRow + spread == numRows - 1) {
						if (AIcount < k) {
						AIcount = 0;
						}
						if (OPcount < k) {
						OPcount = 0;
						}
						break;
						}
						}*/
						if (state[col][startRow + spread] == 1)
						{
							if (OPcount > 0) { AIcount = OPcount = 0; break; }
							AIcount++;
							if (col != 0 && col != numCols - 1 && startRow + spread != 0 && startRow + spread != numRows - 1)
							{
								if (AIcount == 4 && state[col - 1][startRow + spread - 1] == 0 && state[col + 1][startRow + spread + 1] == 0)
								{
									//totalLtdUtility += 10000;
								}
							}
						}
						else if (state[col][startRow + spread] == -1)
						{

							if (AIcount > 0) { AIcount = OPcount = 0; break; }
							OPcount++;
						}

						if (col > 0)
							col--;
					}

					col = colMem - (startRow);
					if (col < 0)
						break;

					if (AIcount > 0) { AIltdCounts[AIcount - 1]++; AIcount = 0; }
					if (OPcount > 0) { OPltdCounts[OPcount - 1]++; OPcount = 0; }
				}

				col = colMem;
			}
			else
				break;
		}

		//if (AIltdCounts[k - 1] > 0) {
		//	for (int i = 0; i < k; i++) {
		//		totalLtdUtility += ((AIltdCounts[i] * (i + 1)));
		//	}
		//}
		//else {
		//	for (int i = 0; i < k; i++) {
		//		totalLtdUtility += ((AIltdCounts[i] * (i + 1))) - (OPltdCounts[i] * (i + 1));
		//	}
		//}

		totalLtdUtility = calculateUtility(AIltdCounts, OPltdCounts, depth, k, testedPlayer)*2;

		delete AIltdCounts;
		delete OPltdCounts;
	}

	return totalLtdUtility;
}

int checkRightDiagonal(int** state, int numCols, int numRows, int depth, int k, bool gravityOn, int testedPlayer)
{
	int totalRtdUtility = 0;
	if (numCols >= k) {

		int* AIrtdCounts = new int[k];
		int* OPrtdCounts = new int[k];
		for (int i = 0; i < k; i++) {
			AIrtdCounts[i] = 0;
			OPrtdCounts[i] = 0;
		}


		int numKCols;
		int diagAmount = 1;

		for (int col = numCols - 1; col >= 0; col--)
		{
			numKCols = diagAmount - k + 1;
			int colMem = col;
			for (int startRow = 0; startRow < numKCols; startRow++)
			{
				int AIcount = 0;
				int OPcount = 0;
				for (int spread = 0; spread < k; spread++)
				{
					/*if (gravityOn)
					{
					if (col == 0 || startRow + spread == numRows - 1) {
					if (AIcount < k) {
					AIcount = 0;
					}
					if (OPcount < k) {
					OPcount = 0;
					}
					break;
					}
					}*/
					if (state[col][startRow + spread] == 1)
					{
						if (OPcount > 0) { AIcount = OPcount = 0; break; }
						AIcount++;
						if (col != 0 && col != numCols - 1 && startRow + spread != 0 && startRow + spread != numRows - 1)
						{
							if (AIcount == 4 && state[col - 1][startRow + spread + 1] == 0 && state[col + 1][startRow + spread - 1] == 0)
							{
								//totalRtdUtility += 10000;
							}
						}
					}
					else if (state[col][startRow + spread] == -1)
					{

						if (AIcount > 0) { AIcount = OPcount = 0; break; }
						OPcount++;
					}
					if (col < numCols - 1)
						col++;
				}
				col = colMem + (startRow + 1);
				if (col < 0)
					break;

				if (AIcount > 0) { AIrtdCounts[AIcount - 1]++; AIcount = 0; }
				if (OPcount > 0) { OPrtdCounts[OPcount - 1]++; OPcount = 0; }
			}
			col = colMem;
			if (diagAmount < numRows)
				diagAmount++;
		}

		for (int row = 1; row < numRows; row++)
		{
			int col = 0;
			int colMem = col;
			numKCols = numRows - row - k + 1;
			if (numKCols > 0)
			{
				for (int startRow = row - 1; startRow < numKCols + 1; startRow++)
				{
					int AIcount = 0;
					int OPcount = 0;
					for (int spread = 0; spread < k; spread++)
					{
						if (state[col][startRow + spread] == 1)
						{
							if (OPcount > 0) { AIcount = OPcount = 0; break; }
							AIcount++;
							if (col != 0 && col != numCols - 1 && startRow + spread != 0 && startRow + spread != numRows - 1)
							{
								if (AIcount == 4 && state[col - 1][startRow + spread + 1] == 0 && state[col + 1][startRow + spread - 1] == 0)
								{
									//totalRtdUtility += 10000;
								}
							}
						}
						else if (state[col][startRow + spread] == -1)
						{

							if (AIcount > 0) { AIcount = OPcount = 0; break; }
							OPcount++;
						}

						col++;
						if (col >= numCols)
							break;
					}
					col = colMem + (startRow);
					if (col < 0)
						break;

					if (AIcount > 0) { AIrtdCounts[AIcount - 1]++; AIcount = 0; }
					if (OPcount > 0) { OPrtdCounts[OPcount - 1]++; OPcount = 0; }
				}

				col = colMem;
			}
			else
				break;
		}

		//if (AIrtdCounts[k - 1] > 0) {
		//	for (int i = 0; i < k; i++) {
		//		totalRtdUtility += ((AIrtdCounts[i] * (i + 1)));
		//	}
		//}
		//else {
		//	for (int i = 0; i < k; i++) {
		//		totalRtdUtility += ((AIrtdCounts[i] * (i + 1))) - (OPrtdCounts[i] * (i + 1));
		//	}
		//}

		totalRtdUtility = calculateUtility(AIrtdCounts, OPrtdCounts, depth, k, testedPlayer)*2;

		delete AIrtdCounts;
		delete OPrtdCounts;
	}

	return totalRtdUtility;
}

int calculateUtility(int* AICounts, int* OPCounts, int depth, int k, int testedPlayer) {
	//printUtilities(AICounts, OPCounts, k);
	int total = 0;
	int AItotal = 0;
	int OPtotal = 0;
	for (int i = 0; i < k; i++)
	{
		AItotal += AICounts[i] * pow(10, (i + 0));
	}
	//total *= 1.5;
	for (int i = 0; i < k; i++)
	{
		OPtotal += OPCounts[i] * pow(10, (i + 0));
	}
	AItotal *= 1;
	OPtotal *= 1.7;


	if (testedPlayer == 1) {
		if (AICounts[k - 1] > 0) {
			AItotal *= 1.2;
		}
		else if (OPCounts[k-2] > 1) {
			OPtotal *= 1.2;
		}
	}
	else {
		if (OPCounts[k-1] == 0 && AICounts[k - 2] > 0) {
			AItotal *= 1.2;
		}
	}


	total = AItotal - OPtotal;
	/*if (OPCounts[k - 1] > 0) {
		for (int i = 0; i < k; i++) {
			total -= ceil(((OPCounts[i] * pow(12, i+1))));
			total -= pow(10, 5);
		}
	}
	else if (AICounts[k - 1] > 0) {
		for (int i = 0; i < k; i++) {
			total += ceil(((AICounts[i] * pow(12, i))));
		}
	}
	else {
		for (int i = 0; i < k - 1; i++) {
			total += ceil((AICounts[i] * pow(10, i))*2 - (OPCounts[i] * pow(10, i)));
		}
	}*/
	/*if (OPCounts[k - 2] > 1)
	{
		//total -= 1000;
	}
	if (AICounts[k - 2] > 1)
	{
		total += 10000;
	}*/
	//printf("here\n");
	return total;
}
// end Edited
#pragma endregion

#pragma region Printing
void printGame(int** state, int numCols, int numRows, int HUMAN_PIECE) {//, std::ofstream &debugInfo) {
																		//debugInfo << "\n";
	printf("\n");
	for (int row = numRows - 1; row >= 0; row--) {
		for (int col = 0; col < numCols; col++) {
			if (state[col][row] == HUMAN_PIECE) {
				//debugInfo << state[col][row];
				printf("%d ", state[col][row]);
			}
			else {
				//debugInfo << " " << state[col][row];
				printf(" %d ", state[col][row]);
			}
		}
		//debugInfo << "\n";
		printf("\n");
	}
	//debugInfo.close();
}

void printState(int** state, int numCols, int numRows, int HUMAN_PIECE, int alpha, int beta, int col, int row, int utility) {
	//printGame(state, numCols, numRows, HUMAN_PIECE);
	printf("(%d,%d) [", col, row);
	if (alpha == -INF) { printf("-INF,"); }
	else { printf("%d,", alpha); }
	if (beta == INF) { printf("INF]\n"); }
	else { printf("%d]  ", beta); }
	printf("%d\n", utility);
	printf("\n");
}
#pragma endregion

#pragma region Borrowed_Random_64_Bit_Generator
// Borrowed this from http://stackoverflow.com/a/8120355, James Kanze (also did some slight edits, such as making it rand56() instead of rand256() and getting values
//  from a list of prime numbers between 1 to 251
unsigned  rand56()
{
	static unsigned const limit = RAND_MAX - RAND_MAX % 56;
	unsigned result = rand();
	while (result >= limit) {
		result = rand();
	}
	return result % 56;
}

unsigned long long rand64bits()
{
	unsigned long long results;
	do {
		results = 1ULL;
		for (int count = 8; count > 0; --count) {
			results *= AIShell::primeNumbers[rand56()];
			if (results == 0) {
				results = 1ULL;
			}
		}
	} while (std::find(AIShell::usedPrimeMultiples.begin(), AIShell::usedPrimeMultiples.end(), results) != AIShell::usedPrimeMultiples.end());
	//printf("Result = %llu\n", results);
	AIShell::usedPrimeMultiples.push_back(results);
	return results;
}
#pragma endregion

bool AIShell::zobristNotInitialized = true;
unsigned long long AIShell::currentHash = 0;
unsigned long long AIShell::primeNumbers[] = { 1ULL, 2ULL,   3ULL,   5ULL,   7ULL,  11ULL,  13ULL,
												17ULL,  19ULL,  23ULL,  29ULL,  31ULL,  37ULL,
												41ULL,  43ULL,  47ULL,  53ULL,  59ULL,  61ULL,
												67ULL,  71ULL,  73ULL,  79ULL,  83ULL,  89ULL,
												97ULL, 101ULL, 103ULL, 107ULL, 109ULL, 113ULL,
												127ULL, 131ULL, 137ULL, 139ULL, 149ULL, 151ULL,
												157ULL, 163ULL, 167ULL, 173ULL, 179ULL, 181ULL,
												191ULL, 193ULL, 197ULL, 199ULL, 211ULL, 223ULL,
												227ULL, 229ULL, 233ULL, 239ULL, 241ULL, 251ULL };
unsigned long long* AIShell::zobristNumbers[2];
std::vector<unsigned long long> AIShell::usedPrimeMultiples;
std::map<unsigned long long, int> AIShell::calculatedUtilities;

void AIShell::initializeZobrist() {
	int product = numCols*numRows;
	// initializing AI_PIECE part
	zobristNumbers[0] = new unsigned long long[product];
	// initializing HUMAN_PIECE part
	zobristNumbers[1] = new unsigned long long[product];

	// now to initialize
	for (int i = 0; i < product; i++) {
		zobristNumbers[0][i] = rand64bits();
		zobristNumbers[1][i] = rand64bits();
		printf("Added a number to i = %d\n", i);
	}
	printf("Initialized the Zobrist numbers\n");
}

unsigned long long AIShell::zobristHash(int ** state)
{
	unsigned long long hash = 0;
	int place = 0;
	for (int col = 0; col < numCols; col++) {
		for (int row = 0; row < numRows; row++) {
			if (state[col][row] == AI_PIECE) {
				hash = hash ^ (zobristNumbers[0][place]);
			}
			else if (state[col][row] == HUMAN_PIECE) {
				hash = hash ^ (zobristNumbers[1][place]);
			}
			place++;
		}
	}
	return hash;
}

unsigned long long AIShell::zobristHash(int col, int row, int player, bool undo)
{
	int index = col*numRows + row;
	int pindex = (int)(player != AI_PIECE);
	unsigned long long newHash = currentHash ^ zobristNumbers[pindex][index];
	//printf("index = %d, player = %d, currentHash = %llu, newHash = %llu\n", index, pindex, currentHash, newHash);
	//printf("oldHash = %llu, newHash = %llu\n", currentHash, newHash);
	return newHash;
}



AIShell::AIShell(int numCols, int numRows, bool gravityOn, int** gameState, Move lastMove)
{
	this->deadline = 0;
	this->numRows = numRows;
	this->numCols = numCols;
	this->gravityOn = gravityOn;
	this->gameState = gameState;
	this->lastMove = lastMove;

	if (zobristNotInitialized) {
		initializeZobrist();
		zobristNotInitialized = false;
	}
}

AIShell::~AIShell()
{

	for (int i = 0; i < numCols; i++) {
		delete[] gameState[i];
	}
	delete[] gameState;

}

// Edited: filled it out :) It'll only check if a player has k many pieces on the board
struct Winner AIShell::terminalTest(int** state, int lastPlayer)
{
	if (numPieces >= k) {
		//printf("Going to check for terminating sequence: numPieces = %d (k = %d)\n", numPieces, k);
		bool beginCol = testedMove.row > 0, endCol = testedMove.row < numRows - 1;
		bool beginRow = testedMove.col > 0, endRow = testedMove.col < numCols - 1;
		bool beginRtd = beginCol && beginRow, endRtd = endCol && endRow;
		bool beginLtd = endCol && beginRow, endLtd = beginCol && endRow;
		int colCount = 1, rowCount = 1, rtdCount = 1, ltdCount = 1;
		beginCol = beginCol && !gravityOn;
		for (int spread = 1; spread < k; spread++) {

			if (colCount >= k || rowCount >= k || rtdCount >= k || ltdCount >= k) {
				Winner toReturn = { true, lastPlayer };
				return toReturn;
			}
			if (!(beginCol || beginRow || beginRtd || beginLtd || endCol || endRow || endRtd || endLtd)) {
				//printf("Have to end because no outlets\n");
				break;
			}

			// Check columns
			if (beginCol && state[testedMove.col][testedMove.row - spread] == lastPlayer) {
				colCount++; beginCol = testedMove.row - spread > 0;
			}
			else { beginCol = false; }
			if (endCol && state[testedMove.col][testedMove.row + spread] == lastPlayer) {
				colCount++; endCol = testedMove.row + spread < numRows - 1;
			}
			else { endCol = false; }

			// Check rows
			if (beginRow && state[testedMove.col - spread][testedMove.row] == lastPlayer) {
				rowCount++; beginRow = testedMove.col - spread > 0;
			}
			else { beginRow = false; }
			if (endRow && state[testedMove.col + spread][testedMove.row] == lastPlayer) {
				rowCount++; endRow = testedMove.col + spread < numCols - 1;
			}
			else { endRow = false; }

			// Check Right Diagonal
			if (beginRtd && state[testedMove.col - spread][testedMove.row - spread] == lastPlayer) {
				rtdCount++; beginRtd = testedMove.row - spread > 0 && testedMove.col - spread > 0;
			}
			else { beginRtd = false; }
			if (endRtd && state[testedMove.col + spread][testedMove.row + spread] == lastPlayer) {
				rtdCount++; endRtd = testedMove.row + spread < numRows - 1 && testedMove.col + spread < numCols - 1;
			}
			else { endRtd = false; }

			// Check Left Diagonal
			if (beginLtd && state[testedMove.col - spread][testedMove.row + spread] == lastPlayer) {
				ltdCount++; beginLtd = testedMove.row + spread < numRows - 1 && testedMove.col - spread > 0;
			}
			else { beginLtd = false; }
			if (endLtd && state[testedMove.col + spread][testedMove.row - spread] == lastPlayer) {
				ltdCount++; endLtd = testedMove.row - spread > 0 && testedMove.col + spread < numCols - 1;
			}
			else { endLtd = false; }
		}
	}
	Winner toReturn = { false, 0 };
	return toReturn;
}
// Edited: added depth so the utility functions could have it
int AIShell::utility(int** state, int depth)
{
	//printGame(state);
	int colUtility = checkColumn(state, numCols, numRows, depth, k, gravityOn, testedPlayer);
	int rowUtility = checkRow(state, numCols, numRows, depth, k, gravityOn, testedPlayer);
	int leftDiaUtility = checkLeftDiagonal(state, numCols, numRows, depth, k, gravityOn, testedPlayer);
	int rightDiaUtility = checkRightDiagonal(state, numCols, numRows, depth, k, gravityOn, testedPlayer);
	int totalUtility = colUtility + rowUtility + leftDiaUtility + rightDiaUtility;
	return totalUtility;

}



bool moveSet_sorter(struct moveCost const& lhs, struct moveCost const& rhs) {
	/*if (lhs.util > rhs.util || lhs.col < rhs.col || lhs.row < rhs.row) {
		return true;
	}
	return false;*/
	if (lhs.util > rhs.util)
	{
		return true;
	}
	else if (lhs.util < rhs.util)
	{
		return false;
	}
	else if (lhs.col > rhs.col)
	{
		return true;
	}
	else if (lhs.col < rhs.col)
	{
		return false;
	}
	else if (lhs.row < rhs.row)
	{
		return true;
	}
	else
	{
		return false;
	}
}


// Alpha Beta Pruning Edits : adding the alpha beta checking to the loop
Move AIShell::minimax(int** state)
{
	time_t end;
	// Adding IDS and time stuffs
	time(&begin);
	//printf("Beginning time: %d\n", int(begin));
	int depth = 1;


	struct moveCost moves[numCols * numRows];
	//std::vector<moveCost> moveSet; //struct moveCost moveSet[(numCols + 1)*(numRows + 1)];
	int moveSetSize = (numCols)*(numRows);

	// Adding IDS and time stuffs
	int index = 0;
	for (int i = floor((numCols / 2)); i >= 0; i--)
	{
		for (int j = 0; j<numRows; j++)
		{
			struct moveCost temp = { -INF, i, j };
			if (state[i][j] == NO_PIECE)
			{
				temp.util += 1;
			}
			moves[index] = temp;
			index++;
		}
	}
	for (int i = floor((numCols / 2))+1; i <numCols; i++)
	{
		for (int j = 0; j<numRows; j++)
		{
			struct moveCost temp = { -INF, i, j };
			if (state[i][j] == NO_PIECE)
			{
				temp.util += 1;
			}
			moves[index] = temp;
			index++;
		}
	}
	printf("Index = %d\n", index - 1);
	/*for (int i = 0; i < numCols; i++)
	{
		for (int j = 0; j < numRows; j++)
		{
			struct moveCost temp = { -INF, i, j };
			if (state[i][j] == NO_PIECE)
			{
				temp.util += 1;
			}
			moves[index] = temp;
			index++;
		}
	}*/
	std::vector<moveCost> moveSet(moves, moves + sizeof(moves) / sizeof(moveCost));

	if (lastMove.col != -1) {
		currentHash = zobristHash(lastMove.col, lastMove.row, HUMAN_PIECE); // adding to the hash?
	}

	//for (int i = 0; i < moveSetSize; i++)
	//{
	//	struct moveCost temp = { -INF,0,0 };
	//	moveSet[i] = temp;
	//	//moveSet.push_back(temp);
	//}
	int cursor = 0;
	int max = -INF;

	int alpha = -INF;
	int beta = INF;

	while (difftime(time(&end), begin) < (deadline / 1000)-1) {
		printf("Depth: %d\n", depth);
		//for (int i = 0; i < numCols; i++)
		//{
		//	for (int j = 0; j < numRows; j++)
		//	{
	for (int i = 0; i < moveSetSize; i++) {
		//if (difftime(time(NULL), begin) < (deadline / 1000)) {
			if (state[moveSet[i].col][moveSet[i].row] == NO_PIECE)//state[i][j] == NO_PIECE)
			{
				//printf("i = %d\n", i);
				numPieces++;
				testedMove.col = moveSet[i].col;
				testedMove.row = moveSet[i].row;
				testedPlayer = AI_PIECE;
				state[moveSet[i].col][moveSet[i].row] = 1;//changing all state[i][j] to state[moveSet[i].col][moveSet[i].row

				currentHash = zobristHash(moveSet[i].col, moveSet[i].row, AI_PIECE); // adding to the hash?

				moveSet[i].util = minValue(state, depth, alpha, beta);


				state[moveSet[i].col][moveSet[i].row] = 0;//stateCopy[i][j] = 0;
				currentHash = zobristHash(moveSet[i].col, moveSet[i].row, AI_PIECE); // removing from the hash?

				numPieces--;
				// Alpha Beta Pruning Edits
				if (moveSet[i].util > alpha) {
					alpha = moveSet[i].util;
				}

				//printState(state, alpha, beta, i, j, moveSet[cursor-1].util);

				if (alpha >= beta) {
					break;
				}
				// end Alpha Beta Pruning Edits

				// GravityOn Edits
				if (gravityOn) {
					//printf("Breaking b/c at top of col\n");
					break;
				}
				// end GravityOn Edits
			}
		//}
		// Alpha Beta Pruning Edits
		if (alpha >= beta) {
			break;
		}
		// end Alpha Beta Pruning Edits
	}
	depth++;
	std::sort(moveSet.begin(), moveSet.end(), &moveSet_sorter);
	//cursor = 0;
	printf("Time: %f\n", difftime(time(&end), begin));
	}
//cursor = 0;
	for (int i = 0; i < moveSetSize; i++)
	{
		printf("Util: (%d,%d,%d)\n", moveSet[i].util, moveSet[i].col, moveSet[i].row);
		if (moveSet[i].util > max && state[moveSet[i].col][moveSet[i].row] == NO_PIECE)
		{
			//printf("Coords: (%d,%d)\n", moveSet[i].col, moveSet[i].row);
			max = moveSet[i].util;
			cursor = i;
		}
	}

	if (state[moveSet[cursor].col][moveSet[cursor].row] != NO_PIECE)
	{
		for (int i = 0; i < moveSetSize; i++)
		{
			if (state[moveSet[i].col][moveSet[i].row] == NO_PIECE)
			{
				cursor = i;
				break;
			}
		}
	}

	//delete stateCopy;

	currentHash = zobristHash(moveSet[cursor].col, moveSet[cursor].row, AI_PIECE);
	numPieces++;
	return Move(moveSet[cursor].col, moveSet[cursor].row);
}

// Alpha Beta Pruning Edits : adding the alpha beta as parameter
int AIShell::maxValue(int** state, int depth, int alpha, int beta)
{
	time_t end;
	int val = -INF;
	Winner win = terminalTest(state, HUMAN_PIECE);
	if (/*win.endGame ||*/ depth == 0) {
		std::map<unsigned long long, int>::iterator result = calculatedUtilities.find(currentHash);
		if (result != calculatedUtilities.end()) {
		//	//printf("Found a redundant thing! (%llu)", hashValue);
		//	//std::ofstream debugInfo;
		//	//debugInfo.open("debugging_info.txt", std::ios::app);
		//	//debugInfo << "Found a redundant thing! " << hashValue;
		//	//printGame(state, numCols, numRows, HUMAN_PIECE, debugInfo);
		//	//debugInfo.close();
			//printf("Utility = %d\n", calculatedUtilities[currentHash]);
			return calculatedUtilities[currentHash];
		}
		else {
			int u = utility(state, depth);
			calculatedUtilities[currentHash] = u;
			return u;
		}
	}
	for (int i = 0; i < numCols; i++)
	{
		for (int j = 0; j < numRows; j++)
		{
			//if (difftime(time(NULL), begin) < (deadline / 1000))
			//{
				if (state[i][j] == NO_PIECE)
				{
					state[i][j] = 1;

					currentHash = zobristHash(i,j, AI_PIECE); // adding to the hash?
					numPieces++;
					testedMove.col = i; testedMove.row = j;
					testedPlayer = AI_PIECE;
					//val = std::max(val, minValue(state, depth - 1, alpha, beta));

					int u = minValue(state, depth - 1, alpha, beta);
					val = std::max(val, u);
					numPieces--;
					currentHash = zobristHash(i, j, AI_PIECE); // removing from the hash?
					// Alpha Beta Pruning Edits
					if (val > alpha) {
						alpha = val;
					}

					//printState(state, alpha, beta, i, j, u);

					state[i][j] = 0;

					if (alpha >= beta) {
						//printf("Pruning in Max: [%d,%d]\n", alpha, beta);
						return val;
					}
					// end Alpha Beta Pruning Edits

					// GravityOn Edits
					if (gravityOn) {
						//printState(state, alpha, beta, i, j, u);
						//printf("Breaking b/c at top of col\n");
						break;
					}
					// end GravityOn Edits
				}
			//}
			//else
			//{
			//	return val;
			//}
		}
		if (difftime(time(&end), begin) >= (deadline / 1000)) {
			if (val == -INF)
				return alpha;
			return val;
		}
	}
	//printf("Reached the end of MAX\n");
	return val;
}

// Alpha Beta Pruning Edits : adding the alpha beta as parameter
int AIShell::minValue(int** state, int depth, int alpha, int beta)
{
	time_t end;
	int val = INF;
	Winner win = terminalTest(state, AI_PIECE);
	if (/*win.endGame ||*/ depth == 0) {
		std::map<unsigned long long, int>::iterator result = calculatedUtilities.find(currentHash);
		if (result != calculatedUtilities.end()) {
		//	//std::ofstream debugInfo;
		//	//debugInfo.open("debugging_info.txt", std::ios::app);
		//	//debugInfo << "Found a redundant thing! " << hashValue;
		//	//printGame(state, numCols, numRows, HUMAN_PIECE, debugInfo);
		//	//debugInfo.close();
			return calculatedUtilities[currentHash];
		}
		else {
			int u = utility(state, depth);
			calculatedUtilities[currentHash] = u;
			return u;
		}
	}
	for (int i = 0; i < numCols; i++)
	{
		for (int j = 0; j < numRows; j++)
		{
			//if (difftime(time(NULL), begin) < (deadline / 1000)) {
				if (state[i][j] == NO_PIECE)
				{
					state[i][j] = -1;
					currentHash = zobristHash(i, j, HUMAN_PIECE); // adding to the hash?
					testedMove.col = i; testedMove.row = j;
					testedPlayer = HUMAN_PIECE;
					//val = std::min(val, maxValue(state, depth - 1, alpha, beta));

					int u = maxValue(state, depth - 1, alpha, beta);
					val = std::min(val, u);
					currentHash = zobristHash(i, j, HUMAN_PIECE); // removing from the hash?
					// Alpha Beta Pruning Edits
					if (val < beta) {
						beta = val;
					}
					//printState(state, alpha, beta, i, j, u);

					state[i][j] = 0;

					if (beta <= alpha) {
						//printf("Pruning in Min: [%d,%d]\n", alpha, beta);
						return val;
					}
					// end Alpha Beta Pruning Edits


					// GravityOn Edits
					if (gravityOn) {
						break;
					}
					// end GravityOn Edits
				}
			//}
			//else { return val; }
		}
		if (difftime(time(&end), begin) >= (deadline / 1000))
		{
			if (val == INF)
				return beta;
			return val;
		}
	}

	//printf("Reached the end of MIN\n");
	return val;

}
// end Edited

Move AIShell::makeMove()
{
	return minimax(gameState);
}

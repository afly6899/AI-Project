#include "AIShell.h"
#include "Move.h"

const int numCols = 9;
const int numRows = 7;

const int NO_PIECE = 0;
const int AI_PIECE = 1;
const int HUMAN_PIECE = -1;

const int k = 3;
const bool gravityOn = false;

int numEachPlayed = 0;

void play(int** game) {
	int col = -1, row = -1;


	do {
		printf("\nOfficial Game State!");
		printGame(game, numCols, numRows, HUMAN_PIECE);

		// // For manual play
		//printf("Input [col, row]: ");
		//std::cin >> col >> row;
		//if (col == -1) { break; }

		// For random play - gravity off
		do {
			col = rand() % numCols;
			row = rand() % numRows;
		} while (game[col][row] != NO_PIECE);
		printf("Playing Move [%d,%d]\n", col, row);

		//// For random play - gravity on
		//col = rand() % numCols;
		//row = 0;
		//while (game[col][row] != NO_PIECE) {
		//	row++;
		//	if (row > numRows) {
		//		col = rand() % numCols;
		//		row = 0;
		//	}
		//}
		//printf("Playing Move [%d,%d]\n", col, row);
		getchar();

		game[col][row] = HUMAN_PIECE;

		int** copy = new int*[numCols];
		for (int i = 0; i < numCols; i++) {
			copy[i] = new int[numRows];
			for (int j = 0; j < numRows; j++) { copy[i][j] = game[i][j]; }
		}

		AIShell AI(numCols, numRows, gravityOn, copy, Move(col, row));
		AI.k = k;
		AI.deadline = 5000;
		AI.numPieces = numEachPlayed;
		Move AI_move = AI.makeMove();
		printf("Returned move:  (%d,%d)\n", AI_move.col, AI_move.row);
		if (AI_move.col == -1) {
			break;
		}
		game[AI_move.col][AI_move.row] = AI_PIECE;
		numEachPlayed++;
	} while (col != -1);

	printf("Game has ended\n");
}


int main() {
	int** game = new int*[numCols];
	for (int i = 0; i < numCols; i++) {
		game[i] = new int[numRows];
		for (int j = 0; j < numRows; j++) { game[i][j] = NO_PIECE; }
	}
	play(game);

	getchar();

	for (int i = 0; i < numCols; i++) {
		delete game[i];
	}
	delete[] game;
	return 0;
}
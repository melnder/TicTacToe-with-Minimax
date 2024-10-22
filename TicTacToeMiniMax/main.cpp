#include <iostream>
#include <Windows.h>
#include <string>

enum TileState {
	EMPTY,
	X,
	O
};

TileState board[9];
TileState searchBoard[9];
bool isOTurn = true;
bool playerIsO = false;
int winner = 0;

void RedrawBoard() {
	system("cls");
	for (int i = 0; i < 9; i++) {
		char tileState = ' ';
		switch (board[i]) {
		case X:
			tileState = 'X';
			break;
		case O:
			tileState = 'O';
			break;
		}
		std::cout << "[" << tileState << "] ";

		if ((i + 1) % 3 == 0)
			std::cout << "\n";

	}
		std::cout << "\nPick move 1-9\n[1][2][3]\n[4][5][6]\n[7][8][9]\n\n";
}

void FillNewBoard(TileState* newBoard, bool search) {
	for (int i = 0; i < 9; i++) {
		if (search)
			newBoard[i] = searchBoard[i];
		else
			newBoard[i] = board[i];
	}
}

void EmptyBoard(TileState* board) {
	for (int i = 0; i < 9; i++) {
		board[i] = EMPTY;
	}
}

bool BoardIsFull(bool search = false) {
	TileState b[9];
	FillNewBoard(b, search);

	for (int i = 0; i < 9; i++) {
		if (b[i] == EMPTY)
			return false;
	}

	return true;
}

int CheckGameOver(bool search = false) {
	TileState check[9];
	FillNewBoard(check, search);

	if ((check[0] == check[3] && check[0] == check[6]) && check[0] != EMPTY)
		return check[0] == O ? 1 : -1;
	if ((check[1] == check[4] && check[1] == check[7]) && check[1] != EMPTY)
		return check[1] == O ? 1 : -1;
	if ((check[2] == check[5] && check[2] == check[8]) && check[2] != EMPTY)
		return check[2] == O ? 1 : -1;
	if ((check[0] == check[1] && check[0] == check[2]) && check[0] != EMPTY)
		return check[0] == O ? 1 : -1;
	if ((check[3] == check[4] && check[3] == check[5]) && check[3] != EMPTY)
		return check[3] == O ? 1 : -1;
	if ((check[6] == check[7] && check[6] == check[8]) && check[6] != EMPTY)
		return check[6] == O ? 1 : -1;
	if ((check[0] == check[4] && check[0] == check[8]) && check[0] != EMPTY)
		return check[0] == O ? 1 : -1;
	if ((check[2] == check[4] && check[2] == check[6]) && check[2] != EMPTY)
		return check[2] == O ? 1 : -1;

	return 0;
}

int MiniMax(bool isMaximising) {
	int x = CheckGameOver(true);
	if (x != 0) {
		return x;
	}

	if (BoardIsFull(true))
		return 0;

	if (isMaximising) {
		int bestEval = -9999;
		for (int i = 0; i < 9; i++) {
			if (searchBoard[i] == EMPTY) {
				searchBoard[i] = X;
				int eval = MiniMax(false);
				searchBoard[i] = EMPTY;
				if (eval > bestEval) {
					bestEval = eval;
				}
			}
		}
		return bestEval;
	}
	else {
		int bestEval = 9999;
		for (int i = 0; i < 9; i++) {
			if (searchBoard[i] == EMPTY) {
				searchBoard[i] = O;
				int eval = MiniMax(true);
				searchBoard[i] = EMPTY;
				if (eval < bestEval) {
					bestEval = eval;
				}
			}
		}
		return bestEval;
	}
}

void AIMove() {
	int bestEval = playerIsO ? 9999 : -9999;
	int bestMove = -1;
	for (int i = 0; i < 9; i++) {
		if (board[i] == EMPTY) {
			searchBoard[i] = playerIsO ? X : O; 
			int eval = MiniMax(!playerIsO);
			searchBoard[i] = EMPTY;;

			if (playerIsO && eval < bestEval)
			{
				bestEval = eval;
				bestMove = i;
			}
			else if (!playerIsO && eval > bestEval) {
				bestEval = eval;
				bestMove = i;
			}
		}
	}

	if (bestMove != -1) {
		board[bestMove] = playerIsO ? X : O;
		searchBoard[bestMove] = playerIsO ? X : O;
	}
}

bool TestMoveAvailable(std::string& move) {
	for (int i = 0; i < move.length(); i++) {
		if (!std::isdigit(move[i])) {
			return false;
		}
	}

	int tile = std::stoi(move) - 1;

	if (tile < 0 || tile > 8)
		return false;

	if (board[tile] != EMPTY)
		return false;

	return true;
}

void MainLoop() {
	bool shouldMove = isOTurn == playerIsO;
	if(shouldMove)
		RedrawBoard();

	std::string move;
	bool moveChosen = false;
	if (shouldMove) {
		while (!moveChosen) {
			std::getline(std::cin, move);

			if (!move.empty() && TestMoveAvailable(move)) {
				int moveTile = std::stoi(move) - 1;
				board[moveTile] = playerIsO ? O : X;
				searchBoard[moveTile] = playerIsO ? O : X;
				moveChosen = true;
			}
			else
				std::cout << "Invalid move. Try again.\n";;
		}
	}
	else
		AIMove();

	isOTurn = !isOTurn;

	int gameOver = CheckGameOver();

	if (gameOver == 0)
		if (!BoardIsFull())
			MainLoop();
}

void RunGame() {
	std::cout << "Play as x or o?\n";
	std::string input;
	bool sideChosen = false;
	while (!sideChosen) {
		std::getline(std::cin, input);

		if (input == "o") {
			playerIsO = true;
			sideChosen = true;
		} else if (input == "x") {
			playerIsO = false;
			sideChosen = true;
		}

		std::cout << "No\n";
	}

	MainLoop();

	RedrawBoard();
	std::cout << "Game Over!\n";

	winner = CheckGameOver();
	if (winner != 0)
		std::cout << (winner == 1 ? "O wins\n" : "X wins\n");
	else
		std::cout << "Draw\n";

	std::string action;
	std::cout << "Type \'r\' to restart or anything else to quit\n";
	std::getline(std::cin, action);

	if (action == "r") {
		system("cls");
		EmptyBoard(board);
		EmptyBoard(searchBoard);
		isOTurn = true;
		winner = 0;
		RunGame();
	}
}

int main() {
	RunGame();
	return 0;
}
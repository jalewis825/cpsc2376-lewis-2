
#include <iostream>
#include <vector>
#include <limits>
#include <string>

const int size = 3;

enum class Player {none, X, O};
enum class Direction { top, bottom, left, right};

struct Cell {
	bool top = false;
	bool bottom = false;
	bool left = false;
	bool right = false;//initilizes each side of the cell to false (not played)
	Player owner = Player::none; //initilizes the owner of the cell to none.
};

//creates a vector of vectors to access each cell within the board
std::vector<std::vector<Cell>> board(size, std::vector<Cell>(size));
int player1Score = 0;
int player2Score = 0;
Player currentPlayer = Player::X;

void printRules() {
	std::cout << "Welcome to Dots & Boxes: Players take turns connecting the dots." << std::endl;
	std::cout << "When a player completes a box, they earn a point and take another turn." << std::endl;
	std::cout << "The game ends when all boxes are claimed, and the player with the most points wins!" << std::endl;
}


//prints out the game grid
void makeBoard() {
	std::cout << std::endl;

	//print column numbers
	std::cout << "    ";
	for (int col = 1; col <= size; col++) {
		std::cout << col << "   ";
	}
	std::cout << std::endl;

	for (int row = 0; row < size; row++) {
		//print dots & horizontal lines when move is made for that side
		std::cout << "  ";
		for (int col = 0; col < size; col++) {
			std::cout << "." << (board[row][col].top ? "---" : "   ");
		}
		std::cout << "." << std::endl;
		
		//box owner & vertical lines
		std::cout << row +1 << " ";
		for (int col = 0; col < size; col++) {
			std::cout << (board[row][col].left ? "| " : "  "); //if left side is true display | other wise show nothing
			if (board[row][col].owner == Player::X) {
				std::cout << "X ";
			}
			else if (board[row][col].owner == Player::O) {
				std::cout << "O ";
			}
			else {
				std::cout << "  "; //empty box til owned
			}

		}
		std::cout << "|" << std::endl;
		}

	//print last row of dots & horizontal lines
	std::cout << "  ";
	for (int col = 0; col < size; col++) {
		std::cout << "." << (board[size-1][col].bottom ? "___" : "   ");
	}
	std::cout << "." << std::endl;
	std::cout << std::endl;
}

//update board when player marks a side
void makeMove(int row, int col, Direction direction) {
	switch (direction) {
		case Direction::top:
			board[row][col].top = true;
			if (row > 0) {
				board[row - 1][col].bottom = true;
			}
			break;
		case Direction::bottom:
			board[row][col].bottom = true;
			if (row < size - 1) {
				board[row - 1][col].bottom = true;
			} 
			break;
		case Direction::left:
			board[row][col].left = true;
			if (col > 0) {
				board[row][col].right = true;
			}
			break;
		case Direction::right:
			board[row][col].right = true;
			if (col < size - 1) {
				board[row][col +1].left = true;
			}
			break;
	}
}

//verifies the move is valid by checking availability of users input
bool isMoveValid(int row, int col, Direction direction) {

	//makes sure row and col designation provided by user with within range of the board size
	if (row < 0 || row >= size || col < 0 || col >= size) {
		return false;
	}

	//checks if side of the cell has already been claimed
	switch (direction) {
		case Direction::top: 
			return !board[row][col].top;
		case Direction::bottom:
			return !board[row][col].bottom;
		case Direction::left:
			return !board[row][col].left;
		case Direction::right:
			return !board[row][col].right;
		default:
			return false;
	}
}

//checks for a complete box and assigns points to the player that has a complete box
bool completeBox(int row, int col) {
	if (board[row][col].top 
		&& board[row][col].bottom
		&& board[row][col].left 
		&& board[row][col].right
		&& board[row][col].owner == Player::none) {
		board[row][col].owner = currentPlayer;
		if (currentPlayer == Player::X) {
			player1Score++;
		}
		else {
			player2Score++;
		}
		return true;
	}
	return false;
}

bool gameStatus() {
	int totalBoxes = size * size;
	return (player1Score + player2Score) < totalBoxes;
}

void gamePlay() {
	int row;
	int col;
	char directionInput;
	bool extraTurn;

	while (gameStatus()) {
		makeBoard();
		std::cout << "Player: " << (currentPlayer == Player::X ? 'X' : 'O') << "'s turn. Enter row, column, and direction (T/B/L/R): ";
		
		Direction direction;
		while (!(std::cin >> row >> col >> directionInput) || 
			(directionInput != 'T' && directionInput != 'B' && directionInput != 'L' && directionInput != 'R' &&
			 directionInput != 't' && directionInput != 'b' && directionInput != 'l' && directionInput != 'r')) {
			std::cout << "Invalid move. Try again: ";
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}

		switch (directionInput) {
		case 'T': case 't':
			direction = Direction::top;
			break;
		case 'B': case 'b':
			direction = Direction::bottom;
			break;
		case 'L': case 'l':
			direction = Direction::left;
			break;
		case 'R': case 'r':
			direction = Direction::right;
			break;
		}

		if (!isMoveValid(row -1, col - 1, direction)) {
			std::cout << "Invalid move. Try again." << std::endl;
			continue;
		}

		makeMove(row - 1, col - 1, direction); //account for 0-based intexing of row/col
		extraTurn = completeBox(row - 1, col - 1);

		if (!extraTurn) {
			currentPlayer = (currentPlayer == Player::X) ? Player::O : Player::X;
		}
	}

	makeBoard();
	std::cout << "Game over!" << std::endl;
	std::cout << "Player X Score: " << player1Score << std::endl;
	std::cout << "Player O Score: " << player2Score << std::endl;
	if (player1Score > player2Score) {
		std::cout << "Player X wins!";
	}
	else if (player2Score > player1Score) {
		std::cout << "Player O wins!";
	}
	else {
		std::cout << "It's a tie!";
	}
}


int main() {
	printRules();
	gamePlay();
	

	while (true) {
		char playAgain;
		std::cout << "Play Again? (Y/N): ";
		std::cin >> playAgain;
		if (playAgain == 'y' || playAgain == 'Y') {
			//rest scores, board, and set first player to X
			player1Score = 0;
			player2Score = 0;
			board = std::vector<std::vector<Cell>>(size, std::vector<Cell>(size));
			currentPlayer = Player::X;
			gamePlay();
		}
		else if (playAgain == 'n' || playAgain== 'N') {
			std::cout << "Thanks for playing!" << std::endl;
			return 0;
		}
		else {
			std::cout << "Invalid input. Please enter Y or N. " << std::endl;
		}
	}
}

/*AI: Used to help me get started with gamePlay(); I had printRules(), makeBoard(), a couple other functions, and the enums created, 
I just needed help with a jumping off point for game play. I was struggling with the user input part of game play.*/
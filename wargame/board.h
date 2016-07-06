/**
 * @file board.h
 * @author Rahul Kumar and Roger Xiao
 */
#ifndef BOARD_H
#define BOARD_H

/**
 * This class is used as a War Game board.
 * This class can output the current scores of Player Blue and Green,
 * output the board's current state, have a Player take a piece in 
 * position (row, column), and finally, detect if the War Game is over.
 */
class Board 
{
	public:
		Board(int filenum);
		Board(const Board * other);
		bool isGameOver();
		int putPiece(int row, int col, char player);
		void updateScore(int & blueScore, int & greenScore);
		void printBoard();
		
	private:
		/**
		 * BoardPiece is a "piece" of the Board class.
		 * A Player that takes a BoardPiece adds that BoardPiece's
		 * value to their score.
		 * 
		 * @see board.cpp for function definitions.
		 */
		class BoardPiece 
		{
			public:
				int value;
				char playerInControl;
				BoardPiece();
		};
		BoardPiece gameBoard[6][6];
		
		void paraDrop(int row, int col, char player);
		bool deathBlitz(int row, int col, char player);
};

#endif

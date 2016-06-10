/**
 * @file board.h
 * @author Rahul Kumar
 */
#ifndef BOARD_H
#define BOARD_H

/**
 * This class is used as a War Game board
 * @see board.cpp for function comments.
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
		 * BoardPiece is used as a piece of the
		 * Board class
		 * @see board.cpp for function comments
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

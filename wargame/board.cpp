/**
 * @file board.cpp
 * @author Rahul Kumar
 */
#include <fstream>
#include <iostream>
using namespace std;

#include "board.h"

/**
 * Constructs a board based on one of the five input files
 * 
 * @param filenum A number representing which file to load into a Board.
 */
Board::Board(int filenum) 
{
	//Easy-to-read file parsing from http://stackoverflow.com/questions/7868936/read-file-line-by-line
	ifstream * infile = NULL;
	
	if (filenum == 1) 
		infile = new ifstream("boards/Keren.txt");
	else if (filenum == 2) 
		infile = new ifstream("boards/Narvik.txt");
	else if (filenum == 3) 
		infile = new ifstream("boards/Sevastopol.txt");
	else if (filenum == 4) 
		infile = new ifstream("boards/Smolensk.txt");
	else 
		infile = new ifstream("boards/Westerplatte.txt");
	
	int col1,col2,col3,col4,col5,col6;
	int row = 0;
	
	while (*infile >> col1 >> col2 >> col3 >> col4 >> col5 >> col6 && row < 6)
	{
		this->gameBoard[row][0].value = col1;
		this->gameBoard[row][1].value = col2;
		this->gameBoard[row][2].value = col3;
		this->gameBoard[row][3].value = col4;
		this->gameBoard[row][4].value = col5;
		this->gameBoard[row][5].value = col6;
		row++;
	}
	
	delete infile; //closes the ifstream to the file as well.
}

/**
 * Copy Constructor
 * 
 * @param other A Board reference to do a deep copy
 */
Board::Board(const Board * other) 
{	
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			this->gameBoard[i][j].playerInControl = other->gameBoard[i][j].playerInControl;
			this->gameBoard[i][j].value = other->gameBoard[i][j].value; 
		}
	}
}

/**
 * Puts a player piece on the board.
 * 
 * @param row X position of piece to put
 * @param col Y position of piece to put
 * @param player Character representing player that put the piece
 * 
 * @return int 0 - move is illegal
 *  		   1 - move is legal and is a para drop
 * 			   2 - move is legal and is a blitz
 */
int Board::putPiece(int row, int col, char player)
{
	if (row < 0 || row > 5 || col < 0 || col > 5)
		return 0;
		
	char test = gameBoard[row][col].playerInControl;
	
	if (test != 'N' )
		return 0;
	
	this->paraDrop(row, col, player);
	if (this->deathBlitz(row, col, player))
		return 2;
	else
		return 1;
}

/**
 * Fills in parameters with the player scores of the current game
 * 
 * @param blueScore Int reference to blue player's score
 * @param greenScore Int reference to green player's score
 */
void Board::updateScore(int & blueScore, int & greenScore)
{
	blueScore = 0;
	greenScore = 0;
	
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			if (this->gameBoard[i][j].playerInControl == 'B')
				blueScore += this->gameBoard[i][j].value;
			else if (this->gameBoard[i][j].playerInControl == 'G')
				greenScore += this->gameBoard[i][j].value;
		}
	}
}

/**
 * Tests if the game is finished by checking all BoardPieces if they
 * have a non-'N' player controller.
 * 
 * @return boolean
 */
bool Board::isGameOver()
{
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			if (this->gameBoard[i][j].playerInControl == 'N')
				return false;
		}
	}
	return true;
}

/**
 * Prints the board to standard out.
 */
void Board::printBoard()
{
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			cout << this->gameBoard[i][j].value << " " << this->gameBoard[i][j].playerInControl << '\t';
		}
		cout << endl;
	}
}

/**
 * Constructs a board piece for the War Game Board that has its
 * members initialized to game start values.
 */
Board::BoardPiece::BoardPiece()
{
	this->value = 0;
	this->playerInControl = 'N';
}

/**
 * Does the paradrop on the board.
 * 
 * @param row X position of piece to put
 * @param col Y position of piece to put
 * @param player Character representing player that put the piece
 */
void Board::paraDrop(int row, int col, char player)
{
	gameBoard[row][col].playerInControl = player;
}

/**
 * If possible, does a death blitz.
 * 
 * @param row X position of piece already put
 * @param col Y position of piece already put
 * @param player Character representing player that already put the piece
 * 
 * @return True if death blitz occured
 */
bool Board::deathBlitz(int row, int col, char player)
{
	//Is there an adjacent same-player piece to the piece already put?
	//NOTE: There are alot of boundary checks in conditionals to act as short-circuiting.
	bool canBlitz = false;
	
	if (row - 1 >= 0 && gameBoard[row - 1][col].playerInControl == player)
	{
		canBlitz = true;
	}
	else if (row + 1 <= 5 && gameBoard[row + 1][col].playerInControl == player)
	{
		canBlitz = true;
	}
	else if (col - 1 >= 0 && gameBoard[row][col - 1].playerInControl == player)
	{
		canBlitz = true;
	}
	else if (col + 1 <= 5 && gameBoard[row][col + 1].playerInControl == player)
	{
		canBlitz = true;
	}
	if (canBlitz == true)
	{
		if (row - 1 >= 0 && gameBoard[row - 1][col].playerInControl != 'N')
		{
			gameBoard[row - 1][col].playerInControl = player;
		}
		if (row + 1 <= 5 && gameBoard[row + 1][col].playerInControl != 'N')
		{
			gameBoard[row + 1][col].playerInControl = player;
		}
		if (col - 1 >= 0 && gameBoard[row][col - 1].playerInControl != 'N')
		{
			gameBoard[row][col - 1].playerInControl = player;
		}
		if (col + 1 <= 5 && gameBoard[row][col + 1].playerInControl != 'N')
		{
			gameBoard[row][col + 1].playerInControl = player;
		}
	}
	return canBlitz;
}



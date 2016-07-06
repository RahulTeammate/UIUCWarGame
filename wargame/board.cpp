/**
 * @file board.cpp
 * @author Rahul Kumar and Roger Xiao
 */
#include "board.h"
 
#include <iostream>
#include <fstream>

using namespace std;

/**
 * Constructs a board based on one of the five input files
 * 
 * @credit http://stackoverflow.com/questions/7868936/read-file-line-by-line
 *         for parsing lines with numbers separated by tabs.
 * @param filenum A number representing which file to load into a Board.
 */
Board::Board(int filenum) 
{
	ifstream * infile = NULL;
	//connect to the board file specified by filenum.
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
	
	//this loop takes the 2D structure of the board file and translates
	//it to the values of gameBoard, a 2D BoardPiece array.
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
	//close and delete the ifstream to the board file.
	delete infile; 
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
 * Player takes the piece at (row,col).
 * 
 * @param row X position of piece to take
 * @param col Y position of piece to take
 * @param player Character representing player that takes the piece
 * 
 * @return int 0 - move is illegal
 *  		   1 - move is legal and is a para drop
 * 			   2 - move is legal and is a blitz
 */
int Board::putPiece(int row, int col, char player)
{
	//do a boundary check on (row,col).
	if (row < 0 || row > 5 || col < 0 || col > 5)
		return 0;
	//check if the piece was already taken by another player.
	char test = gameBoard[row][col].playerInControl;
	if (test != 'N' )
		return 0;
	//this piece is legal to take. First, the player
	//takes the piece with a Para Drop. Then, the player
	//affects the neighboring pieces with a Death Blitz.
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
 * 
 * @return Nothing
 */
void Board::updateScore(int & blueScore, int & greenScore)
{
	//reset the parameter variables to 0.
	blueScore = 0;
	greenScore = 0;
	//traverse through all of gameBoard. 
	//pieces that belong to Blue Player will have their values added to blueScore.
	//pieces that belong to Green Player will have their values added to greenScore.
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
 * Tests if the game is finished by traversing all of 
 * gameBoard to see if all BoardPieces have non-'N' controllers.
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
 * Prints gameBoard to stdout as a 2D Board.
 * 
 * @return Nothing
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
 * Default Constructor 
 * A board piece for the War Game Board.
 */
Board::BoardPiece::BoardPiece()
{
	this->value = 0;
	this->playerInControl = 'N';
}

/**
 * Does the paradrop on the board.
 * 
 * @param row X position of piece to take
 * @param col Y position of piece to take
 * @param player Character representing player that takes the piece
 */
void Board::paraDrop(int row, int col, char player)
{
	gameBoard[row][col].playerInControl = player;
}

/**
 * If possible, does a death blitz on the board.
 * This function can only be called after paraDrop.
 * 
 * @param row X position of piece that player took.
 * @param col Y position of piece that player took.
 * @param player Character representing player that already took the piece
 * 
 * @return True if death blitz occured
 */
bool Board::deathBlitz(int row, int col, char player)
{
	
	bool canBlitz = false;
	
	//does player own a piece adjacent to the (row,col) piece they took?
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
	//if player owns a piece adjacent to the (row,col) piece they took,
	//a death blitz occurs where all of the opponent's pieces 
	//north-west-south-east of the (row,col) piece becomes the player's.
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



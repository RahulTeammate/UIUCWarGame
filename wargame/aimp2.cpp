#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace std;

#include "minimax.h"
#include "board.h"

/**
 * Main game loop
 * 
 * @return 0 upon sucessful execution
 * 		   -1 upon failed execution
 */
int main () 
{
	srand (time(NULL)); //seeds the random number generator used in minimax.cpp's last function
	
	int input;
	Board * game;
	
	//LOADS THE BOARD===============================================================================
	//==============================================================================================
	cout << "Welcome to War Game" << endl;
	cout << endl;
	cout << "Which board do you want to play in?" << endl;
	cout << "Type in 1 for the Keren Board" << endl;
	cout << "Type in 2 for the Narvik Board" << endl;
	cout << "Type in 3 for the Sevastopol Board" << endl;
	cout << "Type in 4 for the Smolensk Board" << endl;
	cout << "Type in 5 for the Westerplatte Board" << endl;
	cout << endl;
	cout << "Input: ";
	cin >> input;
	
	if (input > 0 && input < 6) 
	{
		game = new Board(input);
	}
	else 
	{
		cout << "INPUT ERR: Please reload the program" << endl;
		return -1;
	}
	
	int blueScore = 0;
	int greenScore = 0;
	
	cout << endl;
	cout << "Board Load successful. Outputting initial state of board" << endl;
	game->printBoard();
	
	
	//SETS PLAYER AI================================================================================
	//==============================================================================================
	bool playerBlueUsesAB = false;
	bool playerGreenUsesAB = false;
	bool playerBlueIsHuman = false;
	bool playerGreenIsHuman = false;
	
	cout << endl;
	cout << "Which should the AI of Player Blue, the first player, be?" << endl;
	cout << "Type in 1 for Minimax Search for depth 3" << endl;
	cout << "Type in 2 for Alpha-Beta Search for depth 5" << endl;
	cout << "Type in 3 to play as a Human" << endl;
	cout << endl;
	cout << "Input: ";
	cin >> input;
	
	if (input > 0 && input < 4) 
	{
		playerBlueUsesAB = (input == 2);
		playerBlueIsHuman = (input == 3);
	}
	else 
	{
		cout << "INPUT ERR: Please reload the program" << endl;
		return -1;
	}
	
	cout << endl;
	cout << "Which should the AI of Player Green, the second player, be?" << endl;
	cout << "Type in 1 for Minimax Search for depth 3" << endl;
	cout << "Type in 2 for Alpha-Beta Search for depth 5" << endl;
	cout << "Type in 3 to play as a Human" << endl;
	cout << endl;
	cout << "Input: ";
	cin >> input;
	
	if (input > 0 && input < 4) 
	{
		playerGreenUsesAB = (input == 2);
		playerGreenIsHuman = (input == 3);
	}
	else 
	{
		cout << "INPUT ERR: Please reload the program" << endl;
		return -1;
	}
	
	
	
	//MAIN GAME LOOP================================================================================
	//time difference calculation accredited to http://stackoverflow.com/questions/2808398/easily-measure-elapsed-time
	//==============================================================================================
	int numBlueMoves = 0;
	int numGreenMoves = 0;
	
	double totalTimeBlue = 0;
	double totalTimeGreen = 0;
	int totalNodesBlue = 0;
	int totalNodesGreen = 0;
	
	char currPlayer = 'B';
	bool currPlayerUsesAB = playerBlueUsesAB;
	bool currPlayerIsHuman = playerBlueIsHuman;
	
	while (game->isGameOver() == false)
	{
		cout << endl;
		//TURN START=======================
		if (currPlayerIsHuman == false)
		{
			//AI MOVE==============================
			if (currPlayer == 'B')
				cout << "Choose a move, AI Player Blue" << endl;
			else
				cout << "Choose a move, AI Player Green" << endl;
			
			clock_t begin = clock();
		
			Minimax * tree = new Minimax(currPlayer, game, currPlayerUsesAB);
			int row = 0; int col = 0;
			
			tree->chooseNextMove(row,col);
			
			game->putPiece(row, col, currPlayer);
		
			clock_t end = clock();
			
			cout << "Row: " << row << ", col: " << col << endl;
		
			cout << endl;
			game->printBoard();
			game->updateScore(blueScore, greenScore);
			cout << "Player Blue: " << blueScore << ", Player Green: " << greenScore << endl;
		
			//GET SOME VALUES======================
		
			if (currPlayer == 'B')
			{
				totalTimeBlue += double(end - begin) / CLOCKS_PER_SEC;
				totalNodesBlue += tree->getNumNodes();
				numBlueMoves++;
			}
			else
			{
				totalTimeGreen += double(end - begin) / CLOCKS_PER_SEC;
				totalNodesGreen += tree->getNumNodes();
				numGreenMoves++;
			}
			
			//TURN CLEANUP=====================
			delete tree;
		}
		else
		{	
			//HUMAN MOVE======================================
			if (currPlayer == 'B')
				cout << "Choose a move, Human Player Blue" << endl;
			else
				cout << "Choose a move, Human Player Green" << endl;
			int row = 0;
			int col = 0;
			
			clock_t begin = clock();
			
			while (true)
			{
				cout << "Row to move: ";
				cin >> row;
				cout << "Col to move: ";
				cin >> col;
				
				if (game->putPiece(row, col, currPlayer) != 0)
				{
					break;
				}
				else
				{
					cout << "Illegal Move. Try Again" << endl;
					cout << endl;
				}
			}
			
			clock_t end = clock();
			
			cout << endl;
			game->printBoard();
			game->updateScore(blueScore, greenScore);
			cout << "Player Blue: " << blueScore << ", Player Green: " << greenScore << endl;
			
			if (currPlayer == 'B')
			{
				totalTimeBlue += double(end - begin) / CLOCKS_PER_SEC;
				numBlueMoves++;
			}
			else
			{
				totalTimeGreen += double(end - begin) / CLOCKS_PER_SEC;
				numGreenMoves++;
			}
		}
		//SWITCH TURN===========================
		currPlayerUsesAB = (currPlayer == 'B') ? playerGreenUsesAB : playerBlueUsesAB;
		currPlayerIsHuman = (currPlayer == 'B') ? playerGreenIsHuman : playerBlueIsHuman;
		currPlayer = (currPlayer == 'B') ? 'G' : 'B';
	}
	
	cout << "Game Over" << endl;
	if (blueScore > greenScore) cout << "Player Blue wins by " << blueScore - greenScore << " points." << endl;
	else if (blueScore < greenScore) cout << "Player Green wins by " << greenScore - blueScore << " points." << endl;
	else cout << "Tie Game." << endl;
	
	cout << endl;
	cout << "Total number of nodes expanded by Blue: " << totalNodesBlue << ", total number of nodes expanded by Green: " << totalNodesGreen << endl;
	cout << "Average number of nodes expanded by Blue: " << double(totalNodesBlue)/numBlueMoves
		 << ", average number of nodes expanded by Green: " << double(totalNodesGreen)/numGreenMoves << endl;
	cout << "Average time to make a move by Blue: " << double(totalTimeBlue)/numBlueMoves
		 << ", average time to make a move by Green: " << double(totalTimeGreen)/numGreenMoves << endl;
	
	//CLEAN UP======================================================================================
	//==============================================================================================
	delete game;
	return 0;
}

/**
 * @file board.cpp
 * @author Rahul Kumar and Roger Xiao
 */
 
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "board.h"
#include "minimax.h"

using namespace std;

/**
 * The main function sets up the Main Game Loop 
 * of the War Game, using these steps:
 * 1. Ask the User to choose 1 out of the 5 Boards
 *    that the War Game will be played on.
 * 2. Ask the User to choose the types of Player Blue
 *    and Green: human, Minimax AI, or Minimax AI 
 *    with Alpha-Beta Pruning.
 * 
 * Then, this function runs the Main Game Loop
 * until the War Game ends. Finally, this function
 * displays the Blue and Green Players' scores.
 * 
 * @credit http://stackoverflow.com/questions/2808398/easily-measure-elapsed-time
 *         for time difference calculation.
 * @return 0 upon sucessful execution
 * 		   -1 upon failed execution
 */
int main () 
{
	//seed the RNG used in Minimax's chooseNextMove function.
	srand (time(NULL)); 
	
	int input;
	Board * game;
	
	//LOAD THE BOARD============================================================
	//==========================================================================
	cout << "Welcome to War Game" << endl;
	cout << endl;
	cout << "Which board do you want to play in?" << endl;
	cout << "Type in 1 for the Keren Board" << endl;
	cout << "Type in 2 for the Narvik Board" << endl;
	cout << "Type in 3 for the Sevastopol Board" << endl;
	cout << "Type in 4 for the Smolensk Board" << endl;
	cout << "Type in 5 for the Westerplatte Board" << endl;
	cout << endl;
	//ask the User to choose 1 of 5 Boards.
	cout << "Input: ";
	cin >> input;
	//error handling for bad User input.
	if (input > 0 && input < 6) 
	{
		game = new Board(input);
	}
	else 
	{
		cout << "INPUT ERR: Please reload the program" << endl;
		return -1;
	}
	//set up Player Blue and Green's scores.
	int blueScore = 0;
	int greenScore = 0;
	//output a success message and display the chosen Board.
	cout << endl;
	cout << "Board Load successful. Outputting initial state of board" << endl;
	game->printBoard();
	
	
	//SET PLAYER TYPE===========================================================
	//==========================================================================
	bool playerBlueUsesAB = false;
	bool playerGreenUsesAB = false;
	bool playerBlueIsHuman = false;
	bool playerGreenIsHuman = false;
	
	//display all types that Player Blue can be.
	cout << endl;
	cout << "Which should the AI of Player Blue, the first player, be?" << endl;
	cout << "Type in 1 for Minimax Search for depth 3" << endl;
	cout << "Type in 2 for Alpha-Beta Search for depth 5" << endl;
	cout << "Type in 3 to play as a Human" << endl;
	cout << endl;
	//ask the User to choose the type of Player Blue.
	cout << "Input: ";
	cin >> input;
	//error handling for bad User input.
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
	
	//display all types that Player Green can be.
	cout << endl;
	cout << "Which should the AI of Player Green, the second player, be?" << endl;
	cout << "Type in 1 for Minimax Search for depth 3" << endl;
	cout << "Type in 2 for Alpha-Beta Search for depth 5" << endl;
	cout << "Type in 3 to play as a Human" << endl;
	cout << endl;
	//ask the User to choose the type of Player Green.
	cout << "Input: ";
	cin >> input;
	//error handling for bad User input.
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
	
	//FINAL SETUPS FOR MAIN GAME LOOP===========================================
	//==========================================================================
	//total number of moves Player Blue and Green use in one War Game.
	int numBlueMoves = 0;
	int numGreenMoves = 0;
	//total time Player Blue and Green take to make their moves in one War Game.
	double totalTimeBlue = 0;
	double totalTimeGreen = 0;
	//total number of nodes Player Blue and Green traverse in their Minimax
	//AIs (if applicable) in one War Game.
	int totalNodesBlue = 0;
	int totalNodesGreen = 0;
	//set the first player to Player Blue.
	char currPlayer = 'B';
	//set the first player's AI attributes to Player Blue's attributes.
	bool currPlayerUsesAB = playerBlueUsesAB;
	bool currPlayerIsHuman = playerBlueIsHuman;
	
	//MAIN GAME LOOP============================================================
	//==========================================================================
	while (game->isGameOver() == false)
	{
		//newline done for aesthetic purposes.
		cout << endl;
		
		//if the current player has an AI type.
		if (currPlayerIsHuman == false)
		{
			//output that an AI Player is choosing a move.
			if (currPlayer == 'B')
				cout << "Choose a move, AI Player Blue" << endl;
			else
				cout << "Choose a move, AI Player Green" << endl;
			
			//have the AI Player choose a move on the Board
			//by building and traversing a Minimax Tree, with Alpha-Beta Pruning
			//if it's enabled.
			//also, the time it takes for the AI Player to choose a move is saved.
			clock_t begin = clock();
			Minimax * tree = new Minimax(currPlayer, game, currPlayerUsesAB);
			int row = 0; int col = 0;
			tree->chooseNextMove(row,col);
			game->putPiece(row, col, currPlayer);
			clock_t end = clock();
			
			//output that an AI Player made a move by taking
			//a piece in position (row,column) on the Board.
			cout << "Row: " << row << ", col: " << col << endl;
		
			//output the Board's state after an AI Player made a move.
			cout << endl;
			game->printBoard();
			//also, output Player Blue and Player Green's updated score.
			game->updateScore(blueScore, greenScore);
			cout << "Player Blue: " << blueScore 
			     << ", Player Green: " << greenScore << endl;
		
			//update 3 attributes about the current player
			//since they are an AI Player.
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
			//clean the Minimax Tree used to make a move.
			delete tree;
		}
		else
		{	
			//output that a Human Player is choosing a move.
			if (currPlayer == 'B')
				cout << "Choose a move, Human Player Blue" << endl;
			else
				cout << "Choose a move, Human Player Green" << endl;
			
			//the time it takes for a Human Player to choose a move is saved.
			clock_t begin = clock();
			int row = 0; int col = 0;
			
			//"Valid User Input" Loop where the loop is only broken out of
			//when the User inputs a valid move for the Human Player to make.
			while (true)
			{
				cout << "Row to move: ";
				cin >> row;
				cout << "Col to move: ";
				cin >> col;
				
				//Player makes a valid move by taking
				//a piece in position (row,column) on the Board.
				if (game->putPiece(row, col, currPlayer) != 0)
				{
					break;
				}
				else //Player makes an invalid move.
				{
					cout << "Illegal Move. Try Again" << endl;
					cout << endl;
				}
			}
			clock_t end = clock();
			
			//output the Board's state after a Human Player made a move.
			cout << endl;
			game->printBoard();
			//also, output Player Blue and Player Green's updated score.
			game->updateScore(blueScore, greenScore);
			cout << "Player Blue: " << blueScore 
			     << ", Player Green: " << greenScore << endl;
			
			//update 1 attribute about the current player
			//since they are an AI Player.
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
		
		//change the current player to the other player.
		currPlayerUsesAB = (currPlayer == 'B') ? playerGreenUsesAB : playerBlueUsesAB;
		currPlayerIsHuman = (currPlayer == 'B') ? playerGreenIsHuman : playerBlueIsHuman;
		currPlayer = (currPlayer == 'B') ? 'G' : 'B';
	}
	
	//END OF MAIN GAME==========================================================
	//==========================================================================
	cout << "Game Over" << endl;
	//output the victor based on which player got the highest score.
	if (blueScore > greenScore) 
		cout << "Player Blue wins by " << blueScore - greenScore << " points." << endl;
	else if (blueScore < greenScore) 
		cout << "Player Green wins by " << greenScore - blueScore << " points." << endl;
	else 
		cout << "Tie Game." << endl;
	cout << endl;
	//output attributes about Player Green and Blue's gameplay.
	cout << "Total number of nodes expanded by Blue: " << totalNodesBlue 
	     << ", total number of nodes expanded by Green: " << totalNodesGreen << endl;
	cout << "Average number of nodes expanded by Blue: " << double(totalNodesBlue)/numBlueMoves
		 << ", average number of nodes expanded by Green: " << double(totalNodesGreen)/numGreenMoves << endl;
	cout << "Average time to make a move by Blue: " << double(totalTimeBlue)/numBlueMoves
		 << ", average time to make a move by Green: " << double(totalTimeGreen)/numGreenMoves << endl;
	
	delete game;
	return 0;
}

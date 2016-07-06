/**
 * @file minimax.cpp
 * @author Rahul Kumar and Roger Xiao
 */
#include "minimax.h"
#include <algorithm>
#include <iostream>
#include "board.h"
#include "stdlib.h"

using namespace std;

/**
 * Constructor that builds a minimax tree.
 * 
 * @param currPlayer Char which will be used to judge whether the root will be a min node or a max node.
 * @param currBoard Board to use as the root of the minimax tree
 * @param enableAlphaBeta bool to judge if the player uses alpha-beta search rather than minimax search
 */
Minimax::Minimax(char currPlayer, const Board * currBoard, bool enableAlphaBeta)
{
	this->numNodesExpanded = 0;
	//if alpha-beta pruning is enabled.
	if (enableAlphaBeta)
	{
		//use a helper function to build a minimax tree with alpha-beta pruning.
		alphaBetaBuild(currPlayer, currBoard);
	}
	else //if alpha-beta pruning is disabled.
	{
		//set up the head node of the minimax tree.
		Board * headBoard = new Board(currBoard);
		this->head = new Node(headBoard, 0, 0);
		//account for this node's creation.
		numNodesExpanded++;
		//go to a helper function to build the rest of it.
		minimaxBuild(this->head, currPlayer, 0);
	}
}

/**
 * Destroys all heap memory allocated for a Minimax instance.
 * Calls a recursive helper function.
 */
Minimax::~Minimax()
{
	this->clear(this->head);
}

/**
 * A recursive Minimax tree traversal which frees all
 * memory associated with it.
 * 
 * @param node The Node Pointer the traverser currently points to.
 * @return Nothing
 */
void Minimax::clear(Node * node)
{
	//if this node has children, then recursively
	//free the children first.
	while (node->children.empty() == false)
	{
		this->clear(node->children.back());
		node->children.pop_back();
	}
	//now free this node and its board member.
	delete node->board;
	delete node;
}

/**
 * Constructor of a Node
 * 
 * @param board Board that holds the current game state.
 * @param rowMove Int that holds the row of the move.
 * @param colMove Int that holds the col of the move
 */
Minimax::Node::Node(Board * board, int rowMove, int colMove)
{
	this->board = board;
	this->rowMove = rowMove;
	this->colMove = colMove;
	this->value = 0;
}



/**
 * Recursive helper function for the constructor that builds a 
 * minimax tree to a certain depth.
 * 
 * @param node Node that contains info on Board state.
 * @param currPlayer Current player at a depth within the minimax tree
 * @param depth Int whose value is a base case.
 * 
 * @return int Minimax value of node.
 */
int Minimax::minimaxBuild(Node * node, char player, int depth)
{
	//BASE CASES================================================================
	//Utility(node) if node is terminal=========================================
	//if this node's game state is "Game Over".
	if (node->board->isGameOver() == true)
	{
		int blue = 0; int green = 0;
		node->board->updateScore(blue, green);
		//utility function
		if (blue > green) node->value = 9001;
		else if (blue == green) node->value = 0;
		else node->value = -9001;
		return node->value;
	}
	//if this node is at the maximum depth of the minimax tree.
	if (depth == DEPTH_OF_MINIMAX )
	{
		int blue = 0; int green = 0;
		node->board->updateScore(blue, green);
		//utility function
		node->value = blue - green;
		return node->value;
	}
	//RECURSIVE CASE============================================================
	//max_action Minimax(Succ(node, action)) if player = MAX====================
	//min_action Minimax(Succ(node, action)) if player = MIN====================
	//let player take a piece on all possible spots on the Board. 
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			Board * childBoard = new Board(node->board);
			
			//For all legal moves, make the new Board States into this node's children.
			int moveProp = childBoard->putPiece(i, j, player);
			if (moveProp != 0)
			{
				Node * child = new Node(childBoard,i,j);
				node->children.push_back(child);
				//account for this node's creation.
				this->numNodesExpanded++;
			}
			else
			{
				delete childBoard;
			}
		}
	}
	//set up minimax variables and update the next player.
	int maxVal = -1*INFINITY;
	int minVal = INFINITY;
	char nextPlayer = (player == MAX) ? MIN : MAX;
	
	//iterate through all of this node's children and find the maximum 
	//or minimum value (depending if player is MAX or MIN, respectively)
	for (unsigned int i = 0; i < node->children.size(); i++)
	{
		Node * child = node->children.at(i);
		if (player == MAX)
			maxVal = max(maxVal, minimaxBuild(child, nextPlayer, depth + 1));
		else
			minVal = min(minVal, minimaxBuild(child, nextPlayer, depth + 1));
	}
	if (player == MAX) node->value = maxVal;
	else node->value = minVal;
	return node->value;
}

/**
 * Helper function for constructor which begins the process of building a minimax tree with
 * alpha-beta pruning by calling recursive functions.
 * 
 * @param currPlayer Current player at a depth within the minimax tree
 * @param currBoard Current Game at a depth within the minimax tree
 */
void Minimax::alphaBetaBuild(char currPlayer, const Board * currBoard)
{
	//Function action = Alpha-Beta-Search(node)=================================
	//v = Max-Value(node, −infinity, infinity)==================================
	//AND=======================================================================
	//Function action = Alpha-Beta-Search(node)=================================
	//v = Min-Value(node, −infinity, infinity)==================================
	//set up the head node of the minimax tree (with alpha-beta pruning).
	Board * headBoard = new Board(currBoard);
	//account for this node's creation.
	numNodesExpanded++;
	this->head = new Node(headBoard, 0, 0);
	
	//start building the rest of the minimax tree (with alpha-beta pruning).
	if (currPlayer == MAX)
		abMax(this->head, currPlayer, -1*INFINITY, INFINITY, 0);
	else
		abMin(this->head, currPlayer, -1*INFINITY, INFINITY, 0);
}

/**
 * Helper function for alphaBetaBuild which does half the work of building
 * a minimax tree with alpha-beta pruning recursively.
 * 
 * @param node Node that contains info on the board state
 * @param player Current player at a depth within the minimax tree
 * @param alpha Best alternative available to the Max player 
 * @param beta Best alternative available to the Min player 
 * @param depth Int whose value is a base case
 * 
 * @return int Minimax value of node
 */
int Minimax::abMax(Node * node, char player, int alpha, int beta, int depth)
{
	//BASE CASE=================================================================
	//if Terminal(node) return Utility(node)====================================
	//if this node's game state is "Game Over".
	if (node->board->isGameOver() == true)
	{
		int blue = 0; int green = 0;
		node->board->updateScore(blue, green);
		//utility function.
		if (blue > green) node->value = 9001;
		else if (blue == green) node->value = 0;
		else node->value = -9001;
		return node->value;
	}
	//if this node is at the maximum depth of the minimax tree.
	if (depth == DEPTH_OF_MINIMAX )
	{
		int blue = 0; int green = 0;
		node->board->updateScore(blue, green);
		//utility function.
		node->value = blue - green;
		return node->value;
	}
	
	//RECURSIVE CASE============================================================
	//The comments in the recursive case are based on pseudocode for============
	//Minimax with Alpha-Beta Pruning.==========================================
	//v = −infinity
	int v = -1*INFINITY;
	char nextPlayer = (player == MAX) ? MIN : MAX;
	
	//Set up each action in node
	//AND for each action in node
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			Board * childBoard = new Board(node->board);
			
			int moveProp = childBoard->putPiece(i, j, player);
			if (moveProp != 0)
			{
				this->numNodesExpanded++;
				Node * child = new Node(childBoard,i,j);
				
				// v = Max(v, Min-Value(Succ(node, action), alpha, beta)) 
				v = max(v, abMin(child, nextPlayer, alpha, beta, depth+1));
				
				//Best Move pushed forward, worst move pushed backward
				if (moveProp == 2 && node->children.empty() == false) 
					node->children.insert(node->children.begin(), child);
				else if (moveProp == 2 && node->children.empty() == true) 
					node->children.push_back(child);
				else 
					node->children.push_back(child);
				
				//if v >= beta return v
				if (v >= beta)
				{
					node->value = v;
					return node->value;
				}
				//alpha = Max(alpha, v) 
				alpha = max(alpha, v);
			}
			else
			{
				delete childBoard;
			}
		}
	}
	//end for
	//return v
	node->value = v;
	return node->value;
}

/**
 * Helper function for alphaBetaBuild which does half the work of building
 * a minimax tree with alpha-beta pruning recursively.
 * 
 * @param node Node that contains info on the board state
 * @param currPlayer Current player at a depth within the minimax tree
 * @param alpha Best alternative available to the Max player 
 * @param beta Best alternative available to the Min player 
 * @param depth Int whose value is a base case
 * 
 * @return int Minimax value of node
 */
int Minimax::abMin(Node * node, char player, int alpha, int beta, int depth)
{
	//BASE CASE=================================================================
	//if Terminal(node) return Utility(node)====================================
	//if this node's game state is "Game Over".
	if (node->board->isGameOver() == true)
	{
		int blue = 0; int green = 0;
		node->board->updateScore(blue, green);
		//utility function
		if (blue > green) node->value = 9001;
		else if (blue == green) node->value = 0;
		else node->value = -9001;
		return node->value;
	}
	//if this node is at the maximum depth of the minimax tree.
	if (depth == DEPTH_OF_MINIMAX )
	{
		int blue = 0; int green = 0;
		node->board->updateScore(blue, green);
		//utility function
		node->value = blue - green;
		return node->value;
	}
	
	//RECURSIVE CASE============================================================
	//The comments in the recursive case are based on pseudocode for============
	//Minimax with Alpha-Beta Pruning.==========================================
	//v = infinity
	int v = INFINITY;
	char nextPlayer = (player == MAX) ? MIN : MAX;
	
	//Set up each action in node
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			Board * childBoard = new Board(node->board);
			
			int moveProp = childBoard->putPiece(i, j, player);
			if (moveProp != 0)
			{
				this->numNodesExpanded++;
				Node * child = new Node(childBoard,i,j);
				
				// v = Min(v, Max-Value(Succ(node, action), alpha, beta)) 
				v = min(v, abMax(child, nextPlayer, alpha, beta, depth+1));
				
				//Best Move pushed forward, worst move pushed backward
				if (moveProp == 2 && node->children.empty() == false) 
					node->children.insert(node->children.begin(), child);
				else if (moveProp == 2 && node->children.empty() == true) 
					node->children.push_back(child);
				else 
					node->children.push_back(child);
				
				//if v <= alpha return v
				if (v <= alpha)
				{
					node->value = v;
					return node->value;
				}
				//beta= Min(beta, v) 
				beta = min(beta, v);
			}
			else
			{
				delete childBoard;
			}
		}
	}
	//end for
	//return v
	node->value = v;
	return node->value;
}

/**
 * Function that stores the coordinate to make a move on
 * the board to the params.
 * 
 * @param row Int that will hold the row of the move to make.
 * @param col Int that will hold the col of the move to make.
 */
void Minimax::chooseNextMove(int & row, int & col)
{
	//go through all of the children of the Minimax Tree head.
	vector<unsigned int> listOfBestMoveIndices;
	for (unsigned int i = 0; i < head->children.size(); i++)
	{
		Node * child = head->children.at(i);
		//find the children whose values are equal to the head's value.
		if (head->value == child->value)
		{
			listOfBestMoveIndices.push_back(i);
		}
	}
	
	//if there is more than one 1 child whose values are equal to 
	//the head's value, randomly pick one of these children.
	int randIndex = rand() % ((int)listOfBestMoveIndices.size());
	Node * randChild = head->children.at( listOfBestMoveIndices.at(randIndex) );
	//choose the move the AI must make based on the chosen child node.
	row = randChild->rowMove;
	col = randChild->colMove;
}
/**
 * Getter function
 * 
 * @return numNodesExpanded Member of this class
 */
int Minimax::getNumNodes()
{
	return this->numNodesExpanded;
}

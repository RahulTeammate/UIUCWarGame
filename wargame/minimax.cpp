/**
 * @file minimax.cpp
 * @author Rahul Kumar
 */
#include <algorithm>
#include <iostream>
#include "stdlib.h"
using namespace std;
#include "minimax.h"
#include "board.h"


/**
 * Builds a minimax tree. Uses multiple helper functions
 * 
 * @param currPlayer Char which will be used to judge whether the root will be a min node or a max node.
 * @param currBoard Board to use as the root of the minimax tree
 * @param enableAlphaBeta bool to judge if the player uses alpha-beta search rather than minimax search
 */
Minimax::Minimax(char currPlayer, const Board * currBoard, bool enableAlphaBeta)
{
	this->numNodesExpanded = 0;
	
	if (enableAlphaBeta)
	{
		alphaBetaBuild(currPlayer, currBoard);
	}
	else
	{
		Board * headBoard = new Board(currBoard);
		this->head = new Node(headBoard, 0, 0);
		numNodesExpanded++;
		minimaxBuild(this->head, currPlayer, 0);
	}
}

/**
 * Destroys all heap memory allocated for a Minimax instance
 * Calls a recursive helper function
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
 */
void Minimax::clear(Node * node)
{
	if (node->children.empty())
	{
		delete node->board;
		delete node;
	}
	else
	{
		while (node->children.empty() == false)
		{
			this->clear(node->children.back());
			node->children.pop_back();
		}
		delete node->board;
		delete node;
	}
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
 * Recursive helper function for constructor that builds a minimax tree to a certain depth
 * 
 * @param node Node that contains info on Board state.
 * @param currPlayer Current player at a depth within the minimax tree
 * @param depth Int whose value is a base case.
 * 
 * @return int Minimax value of node.
 */
int Minimax::minimaxBuild(Node * node, char player, int depth)
{
	//BASE CASE============================================================================
	//Utility(node) if node is terminal
	if (node->board->isGameOver() == true)
	{
		int blue = 0; int green = 0;
		node->board->updateScore(blue, green);
		
		if (blue > green) node->value = 9001;
		else if (blue == green) node->value = 0;
		else node->value = -9001;
		return node->value;
	}
	if (depth == DEPTH_OF_MINIMAX )
	{
		int blue = 0; int green = 0;
		node->board->updateScore(blue, green);
		
		node->value = blue - green;
		return node->value;
	}
	//RECURSIVE CASE===============================================================
	//
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			Board * childBoard = new Board(node->board);
			
			int moveProp = childBoard->putPiece(i, j, player);
			if (moveProp != 0)
			{
				Node * child = new Node(childBoard,i,j);
				this->numNodesExpanded++;
				node->children.push_back(child);
			}
			else
			{
				delete childBoard;
			}
		}
	}
	
	int maxVal = -1*INFINITY;
	int minVal = INFINITY;
	char nextPlayer = (player == MAX) ? MIN : MAX;
	
	for (unsigned int i = 0; i < node->children.size(); i++)
	{
		Node * child = node->children.at(i);
		
		//max_action Minimax(Succ(node, action)) if player = MAX 
		//min_action Minimax(Succ(node, action)) if player = MIN 
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
	//Function action = Alpha-Beta-Search(node)
	//v = Max-Value(node, −infinity, infinity) 
	//
	//AND
	//
	//Function action = Alpha-Beta-Search(node)
	//v = Min-Value(node, −infinity, infinity)

	Board * headBoard = new Board(currBoard);
	numNodesExpanded++;
	this->head = new Node(headBoard, 0, 0);
	
	if (currPlayer == MAX)
	{
		abMax(this->head, currPlayer, -1*INFINITY, INFINITY, 0);
	}
	else
	{
		abMin(this->head, currPlayer, -1*INFINITY, INFINITY, 0);
	}
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
	//BASE CASE============================================================================
	//if Terminal(node) return Utility(node)
	if (node->board->isGameOver() == true)
	{
		int blue = 0; int green = 0;
		node->board->updateScore(blue, green);
		
		if (blue > green) node->value = 9001;
		else if (blue == green) node->value = 0;
		else node->value = -9001;
		return node->value;
	}
	if (depth == DEPTH_OF_MINIMAX )
	{
		int blue = 0; int green = 0;
		node->board->updateScore(blue, green);
		
		node->value = blue - green;
		return node->value;
	}
	
	//RECURSIVE CASE=======================================================================
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
				if (moveProp == 2 && node->children.empty() == false) node->children.insert(node->children.begin(), child);
				else if (moveProp == 2 && node->children.empty() == true) node->children.push_back(child);
				else node->children.push_back(child);
				
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
	//BASE CASE============================================================================
	//if Terminal(node) return Utility(node)
	if (node->board->isGameOver() == true)
	{
		int blue = 0; int green = 0;
		node->board->updateScore(blue, green);
		
		if (blue > green) node->value = 9001;
		else if (blue == green) node->value = 0;
		else node->value = -9001;
		return node->value;
	}
	if (depth == DEPTH_OF_MINIMAX )
	{
		int blue = 0; int green = 0;
		node->board->updateScore(blue, green);
		
		node->value = blue - green;
		return node->value;
	}
	
	//RECURSIVE CASE=======================================================================
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
				if (moveProp == 2 && node->children.empty() == false) node->children.insert(node->children.begin(), child);
				else if (moveProp == 2 && node->children.empty() == true) node->children.push_back(child);
				else node->children.push_back(child);
				
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
	vector<unsigned int> listOfBestMoveIndices;
	for (unsigned int i = 0; i < head->children.size(); i++)
	{
		Node * child = head->children.at(i);
		
		if (head->value == child->value)
		{
			listOfBestMoveIndices.push_back(i);
		}
	}
	
	//So listOfBestMoveIndices has all the indices of 
	//child minimax nodes with the same value as their parent.
	//So let's randomly choose a move that is supposedly the same as the rest.
	int randIndex = rand() % ((int)listOfBestMoveIndices.size());
	
	Node * randChild = head->children.at( listOfBestMoveIndices.at(randIndex) );
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

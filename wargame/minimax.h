/**
 * @file minimax.h
 * @author Rahul Kumar
 */
 
#ifndef MINIMAX_H
#define MINIMAX_H

#include <vector>
using namespace std;
#include "board.h"

 /**
 * This class is used for Minimax Strategy.
 * @see minimax.cpp for function comments.
 */
class Minimax
{
	public:
		static const int DEPTH_OF_MINIMAX = 3;
		static const int DEPTH_OF_AB = 5;
		
		static const char MAX = 'B';
		static const char MIN = 'G';
		
		static const int INFINITY = 1000000;
		
		/**
		 * This class is a node of a minimax tree
		 * @see minimax.cpp for function comments
		 */
		class Node
		{
			public:
				Node(Board * board, int rowMove, int colMove);
				
				int value;
				Board * board;
				int rowMove;
				int colMove;
				vector<Node *> children;
		};
	
		Minimax(char currPlayer, const Board * currBoard, bool enableAlphaBeta);
		~Minimax();
		void clear(Node * node);
		
		int minimaxBuild(Node * node, char player, int depth);
		void alphaBetaBuild(char currPlayer, const Board * currBoard);
		
		int abMax(Node * node, char player, int alpha, int beta, int depth);
		int abMin(Node * node, char player, int alpha, int beta, int depth);
		
		void chooseNextMove(int & row, int & col);;
		
		int getNumNodes();
		
	private:
		Node * head;
		int numNodesExpanded;
};

#endif

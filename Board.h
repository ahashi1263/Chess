#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <map>
#include "Piece.h"
#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"
#include "Mystery.h"


namespace Chess
{
  class Board {

		// Throughout, we will be accessing board positions using Position defined in Piece.h.
		// The assumption is that the first value is the column with values in
		// {'A','B','C','D','E','F','G','H'} (all caps)
		// and the second is the row, with values in {'1','2','3','4','5','6','7','8'}

	public:
		// Default constructor
		Board();

		//Default decontructor
		~Board();

		// Returns a const pointer to the piece at a prescribed location if it exists,
		// or nullptr if there is nothing there.
		const Piece* operator() (const Position& position) const;

		// Attempts to add a new piece with the specified designator, at the given position.
		// Throw exception for the following cases:
		// -- the designator is invalid, throw exception with error message "invalid designator"
		// -- the specified position is not on the board, throw exception with error message "invalid position"
		// -- if the specified position is occupied, throw exception with error message "position is occupied"
		void add_piece(const Position& position, const char& piece_designator);

		//If position is occupied, it removes piece from position
		void remove_piece(const Position& position);

		// Displays the board by printing it to stdout
		void display() const;

		// Returns true if the board has the right number of kings on it
		bool has_valid_kings() const;

		//Returns position of specified king
		const Position* getKingPostion(const bool& white) const {return white ? &whiteKingPosition:&blackKingPosition;}

		//sets specified king to its new position
		void setKingPosition(const bool& white, Position newKingPosition ) {white ? whiteKingPosition = newKingPosition : 
		blackKingPosition = newKingPosition;}

		//Iterator class to iterate over entire board
		class Iterator {
			public:

			Iterator(){}
			//Constructor accepting position of board
			Iterator(Position& setPosition):position(setPosition){}
			//Returns Iterator at next position of board
			Iterator operator++();
			//Returns position at that iterator
			Position operator*()const;
			
			//Returns true if two iterators positions are not the same
			bool operator!=(const Iterator& comparedIterator)const;

			private:
			//Position of iterator
			Position position;
		};
		//Returns first board position
		Iterator begin()const;
		//Returns position right after the last board position
		Iterator end()const;

	private:
		// The sparse map storing the pieces, keyed off locations
		std::map<Position, Piece*> occ;

        // Write the board state to an output stream
        friend std::ostream& operator<< (std::ostream& os, const Board& board);

		//Position of the whiteKing
		Position whiteKingPosition = Position('E','1');

		//Position of the blackKing
		Position blackKingPosition = Position('E','8');

		//returns true if position is on board
		bool isOnBoard(const Position& position);

		//Helper function to print the columns of the board
		void displayColumns() const;
	};
}
#endif // BOARD_H

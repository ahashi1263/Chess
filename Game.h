#ifndef GAME_H
#define GAME_H

#include <iostream>
#include "Piece.h"
#include "Board.h"
#include "Exceptions.h"

namespace Chess
{

	class Game {

	public:
		// This default constructor initializes a board with the standard
		// piece positions, and sets the state to white's turn
		Game();

		//Copy constructor for game
		Game(const Game &game);

		//Destructor for game
		~Game() {}

		// Returns true if it is white's turn
		/////////////////////////////////////
		// DO NOT MODIFY THIS FUNCTION!!!! //
		/////////////////////////////////////
		bool turn_white() const { return is_white_turn; }
    
    /////////////////////////////////////
		// DO NOT MODIFY THIS FUNCTION!!!! //
		/////////////////////////////////////
    // Displays the game by printing it to stdout
		void display() const { board.display(); }
    
    /////////////////////////////////////
		// DO NOT MODIFY THIS FUNCTION!!!! //
		/////////////////////////////////////
    // Checks if the game is valid
		bool is_valid_game() const { return board.has_valid_kings(); }

		// Attempts to make a move. If successful, the move is made and
		// the turn is switched white <-> black. Otherwise, an exception is thrown
		void make_move(const Position& start, const Position& end);

		// Returns true if the designated player is in check
		bool in_check(const bool& white) const;

		// Returns true if the designated player is in mate
		bool in_mate(const bool& white) const;

		// Returns true if the designated player is in mate
		bool in_stalemate(const bool& white) const;

        // Return the total material point value of the designated player
        int point_value(const bool& white) const;

		
	

	private:
		// The board
		Board board;

		// Is it white's turn?
		bool is_white_turn;

		//Returns false if position is not on board
		bool isvalidPosition(const Position& position) const;

        // Writes the board out to a stream
        friend std::ostream& operator<< (std::ostream& os, const Game& game);

        // Reads the board in from a stream
        friend std::istream& operator>> (std::istream& is, Game& game);

		//Returns true if path from start to end is clear
		bool pathIsClear(const Position&start, const Position&end, const Piece *piece) const;

		//Returns true if diagonal path is clear
		bool diagonalIsClear(const Position&start, int horizontalChange, int verticalChange) const;

		//Returns true if vertical path is clear
		bool verticalIsClear(const Position&start, const int change) const;

		//Returns true if horizontal path is clear
		bool horizontalIsClear(const Position&start, int change) const;

		//Throws exception if move is invalid
		void checkMoveException(const Position& start, const Position& end, const Piece* currentPiece)const;

		//Throws excpetion if piece capture is invalid
		void checkCaptureException(const Position& start, const Position& end, const Piece* currentPiece)const;

		//promotes a pawn to a queen if pawn is at end of board
		void tryToPromote(const char piece_designator,const Position&end);

		//Returns true if any legal move can be made by given player
		bool can_make_any_move(const bool& white) const;

		//Returns true if piece at start can move legal to position at end
		void try_make_move(const Position& start, const Position& end) const;

	};
}
#endif // GAME_H

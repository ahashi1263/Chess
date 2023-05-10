#include <cassert>
#include <cctype>
#include <cstddef>
#include <iostream>
#include <iterator>
#include "Board.h"
#include "Exceptions.h"
#include "Game.h"
#include "Piece.h"

namespace Chess
{
	/////////////////////////////////////
	// DO NOT MODIFY THIS FUNCTION!!!! //
	/////////////////////////////////////
	Game::Game() : is_white_turn(true) {
		// Add the pawns
		for (int i = 0; i < 8; i++) {
			board.add_piece(Position('A' + i, '1' + 1), 'P');
			board.add_piece(Position('A' + i, '1' + 6), 'p');
		}

		// Add the rooks
		board.add_piece(Position( 'A'+0 , '1'+0 ) , 'R' );
		board.add_piece(Position( 'A'+7 , '1'+0 ) , 'R' );
		board.add_piece(Position( 'A'+0 , '1'+7 ) , 'r' );
		board.add_piece(Position( 'A'+7 , '1'+7 ) , 'r' );

		// Add the knights
		board.add_piece(Position( 'A'+1 , '1'+0 ) , 'N' );
		board.add_piece(Position( 'A'+6 , '1'+0 ) , 'N' );
		board.add_piece(Position( 'A'+1 , '1'+7 ) , 'n' );
		board.add_piece(Position( 'A'+6 , '1'+7 ) , 'n' );

		// Add the bishops
		board.add_piece(Position( 'A'+2 , '1'+0 ) , 'B' );
		board.add_piece(Position( 'A'+5 , '1'+0 ) , 'B' );
		board.add_piece(Position( 'A'+2 , '1'+7 ) , 'b' );
		board.add_piece(Position( 'A'+5 , '1'+7 ) , 'b' );

		// Add the kings and queens
		board.add_piece(Position( 'A'+3 , '1'+0 ) , 'Q' );
		board.add_piece(Position( 'A'+4 , '1'+0 ) , 'K' );
		board.add_piece(Position( 'A'+3 , '1'+7 ) , 'q' );
		board.add_piece(Position( 'A'+4 , '1'+7 ) , 'k' );
	}

	bool Game::isvalidPosition(const Position& position) const {
		//Must be in Range of A - H
		if(position.first - 'A' > 7 || position.first < 'A') {
			return false;
		} else if (position.second - '1' > 7 || position.second < '1') {
			//Must be in range of 1 - 8
			return false;
		}
		return true;
	}

	bool Game::pathIsClear(const Position& start, const Position& end, const Piece *piece) const {
		char pieceType = toupper(piece->to_ascii());
		int horizontalChange = end.first - start.first;
		int verticalChange = end.second - start.second;
		switch(pieceType) {			
			case 'Q':
				if(horizontalChange && verticalChange) {
					return diagonalIsClear(start,horizontalChange,verticalChange);
				} else if (horizontalChange) {
					return horizontalIsClear(start, horizontalChange);
				} else {
					return verticalIsClear(start, verticalChange);
				} 		
			case 'R':
				if(horizontalChange) {
					return horizontalIsClear(start, horizontalChange);
				} else {
					return verticalIsClear(start, verticalChange);
				}
			case 'B':
				return diagonalIsClear(start,horizontalChange,verticalChange); 
			case 'P':
				return verticalChange > 1 ? verticalIsClear(start,verticalChange) : true;
			default:
				return true;	
		}
	}

	bool Game::diagonalIsClear(const Position&start, int horizontalChange, int verticalChange) const {
		//Adds or subtracts 1 depending on the direction of movement of the piece
		int horizontalAugumenter = horizontalChange < 0 ? -1:1;
		int verticalAugumenter = verticalChange < 0 ? -1:1;
		horizontalChange = abs(horizontalChange);
		Position currentPosition = start;
		for(int i = 1; i < horizontalChange; i++) {
			currentPosition.second += verticalAugumenter;
			currentPosition.first += horizontalAugumenter;
			if(board(currentPosition)) {
				return false;
			}
		}
		return true;
	}

	bool Game::verticalIsClear(const Position&start, int change) const {
		//Adds or subtracts 1 depending on the direction of movement of the piece
		int augmenter = change < 0 ? -1 : 1;
		change = abs(change);
		Position currentPosition = start;
		for(int i = 1; i < change; i++) {
			currentPosition.second += augmenter;
			if(board(currentPosition)) {
				return false;
			}
		}
		return true;
	}

	bool Game::horizontalIsClear(const Position&start, int change) const {
		//Adds or subtracts 1 depending on the direction of movement of the piece
		int augmenter = change < 0 ? -1 : 1;
		change = abs(change);
		Position currentPosition = start;
		for(int i = 1; i < change; i++) {
			currentPosition.first += augmenter;
			if(board(currentPosition)) {
				return false;
			}
		}
		return true;
	}

		
	void Game::checkMoveException(const Position& start, const Position& end, const Piece* currentPiece)const {
		if(!isvalidPosition(start)) {
			throw Exception("Could not make move: start position is not on board");
		}  
		if (!isvalidPosition(end)) {
			throw Exception("Could not make move: end position is not on board");
		} 
		if(!currentPiece) {
			throw Exception("Could not make move: no piece at start position");
		}
		if(is_white_turn != currentPiece->is_white()) {
			throw Exception("Could not make move: piece color and turn do not match");
		} 
		if(!currentPiece->legal_move_shape(start,end)) {
			if(tolower(currentPiece->to_ascii()) == 'p' && board(end)) {
				checkCaptureException(start, end, currentPiece);
			} else {
				throw Exception("Could not make move: illegal move shape");
			}
		} 
		if(!pathIsClear(start,end,currentPiece)) {
			throw Exception("Could not make move: path is not clear");
		}
	}

	void Game::checkCaptureException(const Position& start, const Position& end, const Piece* currentPiece)const {
		if(currentPiece->is_white() == board(end)->is_white()) {
			throw Exception("Could not make move: cannot capture own piece");
		} else if (!currentPiece->legal_capture_shape(start,end)) {
			throw Exception("Could not make move: illegal capture shape");
		}
	}

	void Game::tryToPromote(const char piece_designator,const Position&end) {
		if (piece_designator == 'p' && end.second == '1') {
			board.remove_piece(end);
			board.add_piece(end, 'q');
		} else if (piece_designator == 'P' && end.second == '8') {
			board.remove_piece(end);
			board.add_piece(end, 'Q');
		}
	}

	void Game::make_move(const Position& start, const Position& end) {
		try_make_move(start, end);
		const Piece *currentPiece = board(start);
		const char piece_designator = currentPiece->to_ascii();
		if(tolower(currentPiece->to_ascii()) == 'k') {
			board.setKingPosition(is_white_turn, end);			
		}
		board.remove_piece(start);
		if(board(end)) {
			board.remove_piece(end);
		}
		board.add_piece(end,piece_designator); 
		tryToPromote(piece_designator, end);
		is_white_turn = !is_white_turn;	
	}

	Game::Game(const Game &game) {
		for(Board::Iterator it = game.board.begin(); it != game.board.end(); ++it) {
			if(game.board(*it)) {
				board.add_piece(*it, game.board(*it)->to_ascii());
				if(game.board(*it)->to_ascii() == 'k') {
					board.setKingPosition(false, *it);
				} else if(game.board(*it)->to_ascii() == 'K') {
					board.setKingPosition(true, *it);
				}
			}
				
		}
	}

	

	void Game::try_make_move(const Position& start, const Position& end) const{
		const Piece *currentPiece = board(start);		
		checkMoveException(start,end,currentPiece);
		if(board(end)) {
			checkCaptureException(start,end,currentPiece);	

		}
		const char piece_designator = currentPiece->to_ascii();
		Game gameCopy(*this);
		if(tolower(piece_designator) == 'k' ) {
			gameCopy.board.setKingPosition(is_white_turn, end);
		}
		gameCopy.board.remove_piece(start);
		gameCopy.board.remove_piece(end);
		gameCopy.board.add_piece(end, piece_designator);
		if(gameCopy.in_check(is_white_turn)) {
			throw Exception("Could not make move: move exposes check");
		}
	}

	//For every piece on the board, iterate through the entire board and try and make a move
	bool Game::can_make_any_move(const bool& white) const{
	for (Board::Iterator start = board.begin(); start != board.end(); ++start) {
			if(board(*start) && board(*start)->is_white() == white) {
				for (Board::Iterator end = board.begin(); end != board.end(); ++end) {
					try {
						try_make_move(*start, *end);
						return true;
					} catch(Exception const &ex) {
						continue;
					}
				}
			}
	}
return false;
	}
	
	bool Game::in_check(const bool& white) const {
			//Iterate over every piece on board and check if king position
			//is in legal capture state 
			for (Board::Iterator it = board.begin(); it != board.end(); ++it) {	
				if(!board(*it)) {
					//Piece is null
					continue;
				} 
				if(board(*it)->is_white() == white) {
					//Need to make sure pieces checking are from oppoistion team
					continue;
				} else if(board(*it)->legal_capture_shape(*it,*board.getKingPostion(white)) && 
				pathIsClear(*it,*board.getKingPostion(white),board(*it))) { 
					//checks if a piece can capture king
					return true;
				} 
		}
		return false;
	}


	bool Game::in_mate(const bool& white) const {
		// in mate is simply when it is in check and cannot make any move
		return in_check(white) && !can_make_any_move(white);
	}


	bool Game::in_stalemate(const bool& white) const {
		// in stalemate is simply when you are not in check and cannot make any move
		return !in_check(white) && !can_make_any_move(white);
	}

	// keep a running value that adds the already declared point value for each piece on the board
    int Game::point_value(const bool& white) const {
		int value = 0;
		for (Board::Iterator it = board.begin(); it != board.end(); ++it) {
			Position pos = *it;
			const Piece * piece = board(pos);
			if (piece != nullptr && piece->is_white() == white) {
				value += piece->point_value();
			} else {
				continue;
			}
		}
		return value;
	}

      std::istream& operator>> (std::istream& is, Game& game) {		
		//No input in istream
        if(is.peek() == -1) {
            throw Exception("Cannot load the game!");
        }
        char c;
        int i = 0;
        for(Board::Iterator it = game.board.begin(); it != game.board.end(); ++it) {
            game.board.remove_piece(*it);
            c = is.get();
            if(c == '\n') {
                c = is.get();
                i++;
            }
            if(c == '-') {
                continue;
            }
            if(c == 'k') {
                game.board.setKingPosition(false, *it);
            } else if (c == 'K') {
                game.board.setKingPosition(true, *it);
            }
            try {
                game.board.add_piece(*it, c);
            } catch(Exception &ex) {
                throw Exception("Cannot load the game!");
            }           
        }
        is >> c;
        if(c != 'w' && c != 'b') {
            throw Exception("Cannot load the game!");
        } else if (c == 'w') {
            game.is_white_turn = true;
        } else if (c == 'b') {
            game.is_white_turn = false;
        }
        if(!game.is_valid_game() || i != 7) {
            throw Exception("Cannot load the game!");
        }
        return is;
	}

    /////////////////////////////////////
    // DO NOT MODIFY THIS FUNCTION!!!! //
    /////////////////////////////////////
	std::ostream& operator<< (std::ostream& os, const Game& game) {
		// Write the board out and then either the character 'w' or the character 'b',
		// depending on whose turn it is
		return os << game.board << (game.turn_white() ? 'w' : 'b');
	}
}

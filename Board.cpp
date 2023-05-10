#include "Piece.h"
#include <iostream>
#include <utility>
#include <map>
#ifndef _WIN32
#include "Terminal.h"
#endif // !_WIN32
#include "Board.h"
#include "CreatePiece.h"
#include "Exceptions.h"

namespace Chess
{
  /////////////////////////////////////
  // DO NOT MODIFY THIS FUNCTION!!!! //
  /////////////////////////////////////
  Board::Board(){}

  Board::~Board() {
    for(Iterator it = Board::begin(); it != Board::end(); ++it) {
      if((*this)(*it)) {
        delete (*this)(*it);
      }
    }
  }

  Board::Iterator Board::end() const {
    Position position = Position('A','0') ;
    return Board::Iterator(position);
  }

  Board::Iterator Board::begin() const {
    Position position = Position('A','8') ;
    return Board::Iterator(position);
  }

  Position Board::Iterator::operator*() const {
    return position;
  }

  Board::Iterator Board::Iterator::operator++() {
    if(position.first == 'H') {
      position = Position('A',position.second - 1) ;
      return *this;
    } else {
       position = Position(position.first + 1, position.second) ;
      return *this;
    }
  }

  bool Board::Iterator::operator!= (const Iterator& comparedIterator)const {
    const Position comparedPostion = comparedIterator.position;
    if(position.first != comparedPostion.first || position.second != comparedPostion.second) {
      return true;
    }
    return false;
  }

  const Piece* Board::operator()(const Position& position) const {
    try {
      return occ.at(position);
    } catch(const std::out_of_range &e) {
      return nullptr;
    }  
  }
  
  	bool Board::isOnBoard(const Position& position) {
		//Must be in Range of A - H
		if(position.first - 'A' > 7 || position.first < 'A') {
			return false;
		} else if (position.second - '1' > 7 || position.second < '1') {
			//Must be in range of 1 - 8
			return false;
		}
		return true;
	}

  void Board::remove_piece(const Position& position) {
    if((*this)(position)) {
      const Piece* currentPiece = occ[position];
      delete currentPiece; // make sure to delete so no memory leaks
      occ.erase(position);
    }
  }

  void Board::add_piece(const Position& position, const char& piece_designator) {
    if (!isOnBoard(position)) {
      throw Exception ("invalid position");
    } else if ((*this)(position) != nullptr) {
      throw Exception("position is occupied");
    }
    // create piece, perform proper error checks and add to map
    Piece * piece1 = create_piece(piece_designator);
    if (piece1 == nullptr) {
      throw Exception("invalid designator");
    }
    occ[position] = piece1; // add to map
  }


  void Board::displayColumns() const{
    Terminal::color_all(true, Terminal::BLACK, Terminal::WHITE);
    std::cout << "   ";
    for(char cur = 'a'; cur <= 'h'; cur++) {
      std::cout << " " << cur << " ";
    }
    std::cout << "   ";
    Terminal::set_default();
    std::cout << std::endl;
    
  }

  void Board::display() const {
    bool black = true;
    displayColumns();
    for (Iterator it = begin(); it != end(); ++it) {	
      Position currentPosition = *it;
      if(currentPosition.first == 'A') {
        //Pritns row numbers on the left
        Terminal::color_all(true, Terminal::BLACK, Terminal::WHITE);
        std::cout << " " << currentPosition.second << " ";
      }
      Terminal::Color backgroundColor = black ? Terminal::WHITE : Terminal::BLACK;
      Terminal::color_bg(backgroundColor);
			if(!(*this)(currentPosition)) {
        Terminal::color_fg(true, Terminal::BLACK);
        std::cout << "   ";
      } else if((*this)(currentPosition)->is_white()) {
				//Piece is white key
        Terminal::color_fg(true, Terminal::RED);
        std::cout << " " << (*this)(currentPosition)->to_unicode() << " "; 
			} else {
        //Piece is black key
				Terminal::color_fg(true, Terminal::BLUE);
        std::cout << " " << (*this)(currentPosition)->to_unicode() << " "; 
			}
      if(currentPosition.first == 'H') {
        //Prints row numbers on the right
        Terminal::color_all(true, Terminal::BLACK, Terminal::WHITE);
        std::cout << " " << currentPosition.second << " "; 
        Terminal::set_default();
        std::cout << std::endl;
        continue;
      } 
      //Alternates coloring of background
      black = !black;
	}
  Terminal::set_default();
  displayColumns();
  }

  bool Board::has_valid_kings() const {
    int white_king_count = 0;
    int black_king_count = 0;
    for (std::map<std::pair<char, char>, Piece*>::const_iterator it = occ.begin();
	 it != occ.end();
	 it++) {
      if (it->second) {
	switch (it->second->to_ascii()) {
	case 'K':
	  white_king_count++;
	  break;
	case 'k':
	  black_king_count++;
	  break;
	}
      }
    }
    return (white_king_count == 1) && (black_king_count == 1);
  }

  /////////////////////////////////////
  // DO NOT MODIFY THIS FUNCTION!!!! //
  /////////////////////////////////////
  std::ostream& operator<<(std::ostream& os, const Board& board) {
    for(char r = '8'; r >= '1'; r--) {
      for(char c = 'A'; c <= 'H'; c++) {
	const Piece* piece = board(Position(c, r));
	if (piece) {
	  os << piece->to_ascii();
	} else {
	  os << '-';
	}
      }
      os << std::endl;
    }
    return os;
  }
}

#include "Pawn.h"
#include <iostream>


    bool Chess::Pawn::legal_move_shape(const Position& start, const Position& end) const  {
        int directedVerticalChange = end.second - start.second;
        int horizontalChange = abs(start.first - end.first);
        
        //Can't move horizontally
        if(horizontalChange == 0) {
            //white pawn
            if(is_white()) {
                //can move up by 1
                if(directedVerticalChange == 1) {
                    return true;
                } else if(directedVerticalChange == 2 && start.second == '2') {
                    //If on row 2 can move up by 2
                    return true;
                }
            } else {
                //black pawn
                if(directedVerticalChange == -1) {
                    return true;
                } else if(directedVerticalChange == -2 && start.second == '7') {
                    //If on row 2 can move up by 2
                    return true;
                }
            }
            

            
        }
        return false; 
    }

    bool Chess::Pawn::legal_capture_shape(const Position& start, const Position& end) const  {
        
        int directedVerticalChange = end.second - start.second;
        int horizontalChange = abs(start.first - end.first);
        //can move diagonally up by 1
        if(is_white()) {
                if(directedVerticalChange == 1 && horizontalChange == 1) {
                return true;
            }
        } else if(directedVerticalChange == -1 && horizontalChange ==1) {
            return true;
        }
        
        return false;
    }
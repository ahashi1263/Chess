#include "King.h"


    bool Chess::King::legal_move_shape(const Position& start, const Position& end) const  {
        int verticalChange = abs(start.second - end.second);
        int horizontalChange = abs(start.first - end.first);

        //Can move up or down by 1
        if(verticalChange == 1) {
            //Can move diagonally or vertically
             if(horizontalChange == 1 || horizontalChange == 0) {
                return true;
             }
        } else if(horizontalChange == 1 && verticalChange == 0) {
            //If didn't move up or down must move horizontally
            return true;
        }        
        return false;
    }


#include "Queen.h"


    bool Chess::Queen::legal_move_shape(const Position& start, const Position& end) const  {
        int verticalChange = abs(start.second - end.second);
        int horizontalChange = abs(start.first - end.first);

        //Can either move diagonally
        if((verticalChange > 0) && (horizontalChange == verticalChange)) {
            return true;
        } else if ((verticalChange > 0) && (horizontalChange == 0)) {
            //Move up or down 
            return true;
        } else if ((horizontalChange > 0) && (verticalChange == 0)) {
            //Move horizontally
            return true;
        }
        return false; 
    }
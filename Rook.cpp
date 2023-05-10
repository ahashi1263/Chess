#include "Rook.h"


    bool Chess::Rook::legal_move_shape(const Position& start, const Position& end) const  {
        int verticalChange = abs(start.second - end.second);
        int horizontalChange = abs(start.first - end.first);

        //Can Move up or down 
        if ((verticalChange > 0) && (horizontalChange == 0)) {
            return true;
        } else if ((horizontalChange > 0) && (verticalChange == 0)) {
            //Move horizontally
            return true;
        }
        return false; 
    }
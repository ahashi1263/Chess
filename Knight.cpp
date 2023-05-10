#include "Knight.h"


    bool Chess::Knight::legal_move_shape(const Position& start, const Position& end) const  {
        int verticalChange = abs(start.second - end.second);
        int horizontalChange = abs(start.first - end.first);

        //Can either go 2 horizontally then 1 up
        if((horizontalChange == 2 ) && (verticalChange == 1)) {
            return true;
        } else if ((horizontalChange == 1 ) && (verticalChange == 2)) {
            //Or go 2 up then 1 horizontally
            return true;  
        }  
        return false;
    }
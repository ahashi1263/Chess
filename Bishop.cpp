#include "Bishop.h"

namespace Chess
{
  bool Bishop::legal_move_shape(const Position& start, const Position& end) const {
    int verticalChange = abs(start.second - end.second);
    int horizontalChange = abs(start.first - end.first); //I think this should be start.first - end.first

    //Movement horizontally == movement vertically
    if((verticalChange > 0) && (horizontalChange == verticalChange)) {
      return true;
    }
    return false;
  }
}

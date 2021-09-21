#ifndef CHESSMATRIX_H
#define CHESSMATRIX_H

#include "square.h"

class ChessMatrix
{
public:
    ChessMatrix();
    ChessMatrix(ChessMatrix &other);

    Square &operator()(short x, short y) { return _matrix[x][y]; }

private:
    Square _matrix[8][8];
};

#endif // CHESSMATRIX_H

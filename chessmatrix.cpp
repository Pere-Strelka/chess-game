#include "chessmatrix.h"

ChessMatrix::ChessMatrix()
{
}

ChessMatrix::ChessMatrix(ChessMatrix &other)
{
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            _matrix[x][y] = other._matrix[x][y];
        }
    }
}

#include "queen.h"
#include "bishop.h"
#include "rock.h"
#include "../square.h"
#include "../boardcondition.h"

Queen::Queen(Side side, QSvgWidget *parent) : Piece{ side, parent }
{

}

Queen::Queen(Side side, const QString &filepath) : Piece{ side, filepath } {}

QVector<Square *> Queen::getAvailableSquaresFor(BoardCondition *condition, Square *current, ChessMatrix &matrix) const
{
    auto vec = Bishop::static_getAvailableSquaresFor(condition, current, matrix);
    vec += Rock::static_getAvailableSquaresFor(condition, current, matrix);
    return vec;
}

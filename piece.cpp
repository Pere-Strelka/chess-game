#include "piece.h"
#include "square.h"
#include "boardcondition.h"


Piece::Piece(Side side, QSvgWidget *parent) : QSvgWidget(parent), _side{ side }
{
}

Piece::Piece(Side side, const QString &filepath) : QSvgWidget(filepath), _side{ side }
{
}

QVector<Square *> Piece::getAvailableSquaresFor(BoardCondition *condition, Square *current, ChessMatrix &matrix) const
{
    return QVector<Square *>();
}

bool Piece::sharedLogic(QVector<Square *> &availableToMove, bool isCurrentWhite, bool *setAvailable,
                        BoardCondition *condition, Square *current, Square *target)
{
    // this function is shared between bishop and rock because they both process single square the same way
    // it processes a single square and returns true if this is the end square of the path
    // and false if cycle should continue looking for more available squares
    // also false is returned if there may be a bounded piece which means there is opponent's king behind the piece
    // also false is returned if there is king on the path so that the function can mark squares behind the king as unsafe for it's color
    // bool setAvailable is used for both these cases so that the squares are marked as unsafe not as available to move
    if (target->hasPiece() && target->piece()->type() == Piece::Type::King) {
        if (*setAvailable) {
            condition->setKingChecked(isCurrentWhite ? Side::Black : Side::White, current);
            condition->addUnsafeFor(isCurrentWhite ? Side::Black : Side::White, target);
            *setAvailable = false;
        }
        return false;
    } else {
        if (*setAvailable) {
            availableToMove.append(target);
            condition->addUnsafeFor(isCurrentWhite ? Side::Black : Side::White, target);
        }
        if (target->hasPiece()) {
            if (*setAvailable) {
                *setAvailable = false;
                return false;
            } else {
                return true;
            }
        } else {
            return false;
        }
    }
}

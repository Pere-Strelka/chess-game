#include "knight.h"
#include "square.h"
#include "boardcondition.h"

Knight::Knight(Side side, QSvgWidget *parent) : Piece{ side, parent }
{

}

Knight::Knight(Side side, const QString &filepath) : Piece{ side, filepath } {}

QVector<Square *> Knight::getAvailableSquaresFor(BoardCondition *condition, Square *current, ChessMatrix &matrix) const
{
    QVector<Square *> availableToMove;
    Side currentSide = current->piece()->side();
    bool isCurrentWhite = currentSide == Side::White;
    Square *possibleTarget;

    auto process = [&isCurrentWhite, &availableToMove, &currentSide, &condition, &current] (Square *target) {
        if (target->hasPiece() && target->piece()->side() == currentSide)
            condition->addProtected(target);
        else
            if (target->hasPiece() && target->piece()->type() == Piece::Type::King)
                condition->setKingChecked(isCurrentWhite ? Side::Black : Side::White, current);
            else {
                availableToMove.append(target);
                condition->addUnsafeFor(isCurrentWhite ? Side::Black : Side::White, target);
            }
    };

    if (current->x() + 2 < 8 && current->y() + 1 < 8) {
        possibleTarget = &matrix(current->x() + 2, current->y() + 1);
        process(possibleTarget);
    }

    if (current->x() + 2 < 8 && current->y() - 1 >= 0) {
        possibleTarget = &matrix(current->x() + 2, current->y() - 1);
        process(possibleTarget);
    }

    if (current->x() - 2 >= 0 && current->y() - 1 >= 0) {
        possibleTarget = &matrix(current->x() - 2, current->y() - 1);
        process(possibleTarget);
    }

    if (current->x() - 2 >= 0 && current->y() + 1 < 8) {
        possibleTarget = &matrix(current->x() - 2, current->y() + 1);
        process(possibleTarget);
    }

    if (current->x() - 1 >= 0 && current->y() + 2 < 8) {
        possibleTarget = &matrix(current->x() - 1, current->y() + 2);
        process(possibleTarget);
    }

    if (current->x() + 1 < 8 && current->y() - 2 >= 0) {
        possibleTarget = &matrix(current->x() + 1, current->y() - 2);
        process(possibleTarget);
    }

    if (current->x() - 1 >= 0 && current->y() - 2 >= 0) {
        possibleTarget = &matrix(current->x() - 1, current->y() - 2);
        process(possibleTarget);
    }

    if (current->x() + 1 < 8 && current->y() + 2 < 8) {
        possibleTarget = &matrix(current->x() + 1, current->y() + 2);
        process(possibleTarget);
    }
    return availableToMove;
}

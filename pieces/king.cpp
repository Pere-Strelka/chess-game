#include "king.h"
#include "../square.h"
#include "../boardcondition.h"

King::King(Side side, QSvgWidget *parent) : Piece{ side, parent }
{

}

King::King(Side side, const QString &filepath) : Piece{ side, filepath }
{
}

QVector<Square *> King::getAvailableSquaresFor(BoardCondition *condition, Square *current, ChessMatrix &matrix) const
{
    QVector<Square *> availableToMove;
    Side currentSide = current->piece()->side();
    bool isCurrentWhite = currentSide == Side::White;
    int initX = current->x() - 1;
    int initY = current->y() - 1;
    Square *possibleTarget;

    auto process = [&isCurrentWhite, &availableToMove, &currentSide, &condition] (Square *target) {
        if (target->hasPiece() && target->piece()->side() != currentSide && !condition->isSquareProtected(target)) {
            availableToMove.append(target);
            condition->addUnsafeFor(isCurrentWhite ? Side::Black : Side::White, target);

        } else if (target->hasPiece() && target->piece()->side() != currentSide)
            condition->addProtected(target);
        else if (!target->hasPiece() && condition->isSquareSafeFor(currentSide, target))
            availableToMove.append(target);
        condition->addUnsafeFor(isCurrentWhite ? Side::Black : Side::White, target);
    };

    for (int x = 0; x < 3; x++) {
        for (int y = 0; y < 3; y++) {
            if (initX + x < 8 && initX + x >= 0 && initY + y < 8 && initY + y >= 0) {
                possibleTarget = &matrix(initX + x, initY + y);
                process(possibleTarget);
            }
        }
    }
    return availableToMove;
}

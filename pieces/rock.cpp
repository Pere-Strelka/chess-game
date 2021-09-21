#include "rock.h"
#include "square.h"
#include "boardcondition.h"

Rock::Rock(Side side, QSvgWidget *parent) : Piece{ side, parent }
{

}

Rock::Rock(Side side, const QString &filepath) : Piece{ side, filepath } {}

QVector<Square *> Rock::getAvailableSquaresFor(BoardCondition *condition, Square *current, ChessMatrix &matrix) const
{
    return static_getAvailableSquaresFor(condition, current, matrix);
}

QVector<Square *> Rock::static_getAvailableSquaresFor(BoardCondition *condition, Square *current, ChessMatrix &matrix)
{
    QVector<Square *> availableToMove;
    Side currentSide = current->piece()->side();
    bool isCurrentWhite = currentSide == Side::White;
    Square *possibleTarget;

    bool setAvailable = true;
    bool *setAvailablePtr = &setAvailable;

    auto process = [&availableToMove, isCurrentWhite, &setAvailablePtr, &condition, &current] (Square *target) {
        return Piece::sharedLogic(availableToMove, isCurrentWhite, setAvailablePtr, condition, current, target);
    };

    auto processTarget = [&currentSide, &process, &condition] (Square *target) {
        if (target->hasPiece() && target->piece()->side() != currentSide) {
            return process(target);
        } else if (target->hasPiece() && target->piece()->side() == currentSide) {
            condition->addProtected(target);
            return true;
        } else
            return process(target);
    };

    for (int x = current->x() + 1; x < 8; x++) {
        possibleTarget = &matrix(x, current->y());
        if (processTarget(possibleTarget))
            break;
    }
    setAvailable = true;

    for (int x = current->x() - 1; x >= 0; x--) {
        possibleTarget = &matrix(x, current->y());
        if (processTarget(possibleTarget))
            break;
    }
    setAvailable = true;

    for (int y = current->y() + 1; y < 8; y++) {
        possibleTarget = &matrix(current->x(), y);
        if (processTarget(possibleTarget))
            break;
    }
    setAvailable = true;

    for (int y = current->y() - 1; y >= 0; y--) {
        possibleTarget = &matrix(current->x(), y);
        if (processTarget(possibleTarget))
            break;
    }

    return availableToMove;
}

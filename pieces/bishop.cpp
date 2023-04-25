#include "bishop.h"
#include "../square.h"
#include "../boardcondition.h"

Bishop::Bishop(Side side, QSvgWidget *parent) : Piece{ side, parent }
{

}

Bishop::Bishop(Side side, const QString &filepath) : Piece{ side, filepath }
{
}


QVector<Square *> Bishop::getAvailableSquaresFor(BoardCondition *condition, Square *current, ChessMatrix &matrix) const
{
    return static_getAvailableSquaresFor(condition, current, matrix);
}

QVector<Square *> Bishop::static_getAvailableSquaresFor(BoardCondition *condition, Square *current, ChessMatrix &matrix)
{
    QVector<Square *> availableToMove;
    Side currentSide = current->piece()->side();
    bool isCurrentWhite = currentSide == Side::White;
    int i = 1;
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

    for (Coord d = Coord(current->x()+i, current->y()+i); d.x() < 8 && d.y() < 8; d.setCoords(current->x()+i, current->y()+i)) {
        possibleTarget = &matrix(d.x(), d.y());
        if (processTarget(possibleTarget))
            break;
        i++;
    }
    i = 1;
    setAvailable = true;

    for (Coord d = Coord(current->x()-i, current->y()+i); d.x() >= 0 && d.y() < 8; d.setCoords(current->x()-i, current->y()+i)) {
        possibleTarget = &matrix(d.x(), d.y());
        if (processTarget(possibleTarget))
            break;
        i++;
    }
    i = 1;
    setAvailable = true;

    for (Coord d = Coord(current->x()-i, current->y()-i); d.x() >= 0 && d.y() >= 0; d.setCoords(current->x()-i, current->y()-i)) {
        possibleTarget = &matrix(d.x(), d.y());
        if (processTarget(possibleTarget))
            break;
        i++;
    }
    i = 1;
    setAvailable = true;

    for (Coord d = Coord(current->x()+i, current->y()-i); d.x() < 8 && d.y() >= 0; d.setCoords(current->x()+i, current->y()-i)) {
        possibleTarget = &matrix(d.x(), d.y());
        if (processTarget(possibleTarget))
            break;
        i++;
    }
    return availableToMove;
}

#include "pawn.h"
#include "../square.h"
#include "../boardcondition.h"
#include "../board.h"

Pawn::Pawn(Side side, QSvgWidget *parent) : Piece{ side, parent }
{

}

Pawn::Pawn(Side side, const QString &filepath) : Piece{ side, filepath } {}

QVector<Square *> Pawn::getAvailableSquaresFor(BoardCondition *condition, Square *current, ChessMatrix &matrix) const
{
    Square *possibleTarget;
    Move *last = condition->lastMove();
    QVector<Square *> availableToMove;
    Side currentSide = current->piece()->side();
    bool isCurrentWhite = currentSide == Side::White;

    auto processTarget = [&current, &availableToMove, &currentSide, &condition, isCurrentWhite] (Square *target) {
        if (target->hasPiece() && target->piece()->side() != currentSide)
            if (target->piece()->type() == Piece::Type::King)
                condition->setKingChecked(isCurrentWhite ? Side::Black : Side::White, current);
            else
                availableToMove.append(target);
        else if (target->hasPiece() && target->piece()->side() == currentSide)
            condition->addProtected(target);
        else
            condition->addUnsafeFor(isCurrentWhite ? Side::Black : Side::White, target);
    };

    if (current->piece()->side() == Side::White) {

        // if pawn hasn't moved yet, let it go two squares
        if (current->y() == g_wSecondLine && !matrix(current->x(), g_wFourthLine).hasPiece() && !matrix(current->x(), g_wThirdLine).hasPiece())
            availableToMove.append(&matrix(current->x(), g_wFourthLine));

        if (!matrix(current->x(), current->y() - 1).hasPiece())
            availableToMove.append(&matrix(current->x(), current->y() - 1));

        // attacking moves
        if (current->x() - 1 >= 0 && current->y() - 1 >= 0) {
            possibleTarget = &matrix(current->x() - 1, current->y() - 1);
            processTarget(possibleTarget);
        }

       if (current->x() + 1 < 8 && current->y() - 1 >= 0) {
            possibleTarget = &matrix(current->x() + 1, current->y() - 1);
            processTarget(possibleTarget);
        }

        // en pessant
        if (last != nullptr && current->y() == g_bFourthLine) {
            if (current->y() == g_bFourthLine && last->piece()->type() == Piece::Type::Pawn &&
                last->newSquare().y() == g_bFourthLine && last->oldSquare().y() == g_bSecondLine &&
               (last->newSquare().x() == current->x() + 1 || last->newSquare().x() == current->x() - 1)) {
                    availableToMove.append(&matrix(last->newSquare().x(), g_bThirdLine));
                    condition->addEnPessantCase(EnPessant(current, &matrix(last->newSquare().x(), last->newSquare().y()), last->newSquare().x()));
            }
        }
    } else { // if the pawn is black

        // if pawn hasn't moved yet, let it go two squares
        if (current->y() == g_bSecondLine && !matrix(current->x(), g_bFourthLine).hasPiece() && !matrix(current->x(), g_bThirdLine).hasPiece())
            availableToMove.append(&matrix(current->x(), g_bFourthLine));

        if (!matrix(current->x(), current->y() + 1).hasPiece())
            availableToMove.append(&matrix(current->x(), current->y() + 1));

        // attacking moves
        if (current->x() - 1 >= 0 && current->y() + 1 < 8) {
            possibleTarget = &matrix(current->x() - 1, current->y() + 1);
            processTarget(possibleTarget);
        }

        if (current->x() + 1 < 8 && current->y() + 1 < 8) {
            possibleTarget = &matrix(current->x() + 1, current->y() + 1);
            processTarget(possibleTarget);
        }

        // en pessant
        if (last != nullptr && current->y() == g_wFourthLine) {
            if (current->y() == g_wFourthLine && last->piece()->type() == Piece::Type::Pawn &&
                last->newSquare().y() == g_wFourthLine && last->oldSquare().y() == g_wSecondLine &&
               (last->newSquare().x() == current->x() + 1 || last->newSquare().x() == current->x() - 1)) {
                availableToMove.append(&matrix(last->newSquare().x(), g_wThirdLine));
                condition->addEnPessantCase(EnPessant(current, &matrix(last->newSquare().x(), last->newSquare().y()), last->newSquare().x()));
            }
        }
    }
    return availableToMove;
}

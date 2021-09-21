#ifndef PIECE_H
#define PIECE_H

#include <QSvgWidget>
#include <QMouseEvent>
#include <QObject>
#include "side.h"

class Square;
class Move;
class BoardCondition;
class ChessMatrix;
class Coord;


class Piece : public QSvgWidget
{     
public:
    enum class Type {
        King,
        Queen,
        Bishop,
        Knight,
        Rock,
        Pawn,
        None
    };

    Piece(QSvgWidget *parent = nullptr) : QSvgWidget{ parent } {}
    Piece(Side side, QSvgWidget *parent = nullptr);
    Piece(Side side, const QString &filepath);

    virtual Type type() const { return Type::None; }
    virtual QVector<Square *> getAvailableSquaresFor(BoardCondition *condition, Square *current, ChessMatrix &matrix) const;

    Side side() const { return _side; }

private:
    Side _side;

protected:
    static bool sharedLogic(QVector<Square *> &availableToMove, bool isCurrentWhite, bool *setAvailable,
                            BoardCondition *condition, Square *current, Square *target);
};

#endif // PIECE_H

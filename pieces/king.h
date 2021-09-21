#ifndef KING_H
#define KING_H

#include "piece.h"

class King : public Piece
{
public:
    King(QSvgWidget *parent = nullptr) : Piece{ parent } {}
    King(Side side, QSvgWidget *parent = nullptr);
    King(Side side, const QString &filepath);

    virtual Type type() const override { return Type::King; }
    virtual QVector<Square *> getAvailableSquaresFor(BoardCondition *condition, Square *current, ChessMatrix &matrix) const override;
};

#endif // KING_H

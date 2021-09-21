#ifndef KNIGHT_H
#define KNIGHT_H

#include "piece.h"

class Knight : public Piece
{
public:
    Knight(QSvgWidget *parent = nullptr) : Piece{ parent } {}
    Knight(Side side, QSvgWidget *parent = nullptr);
    Knight(Side side, const QString &filepath);

    virtual Type type() const override { return Type::Knight; }
    virtual QVector<Square *> getAvailableSquaresFor(BoardCondition *condition, Square *current, ChessMatrix &matrix) const override;
};

#endif // KNIGHT_H

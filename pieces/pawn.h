#ifndef PAWN_H
#define PAWN_H

#include "../piece.h"

class Pawn : public Piece
{
public:
    Pawn(QSvgWidget *parent = nullptr) : Piece{ parent } {}
    Pawn(Side side, QSvgWidget *parent = nullptr);
    Pawn(Side side, const QString &filepath);

    virtual Type type() const override { return Type::Pawn; }
    virtual QVector<Square *> getAvailableSquaresFor(BoardCondition *condition, Square *current, ChessMatrix &matrix) const override;
};

#endif // PAWN_H

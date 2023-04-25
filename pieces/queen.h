#ifndef QUEEN_H
#define QUEEN_H

#include "../piece.h"

class Queen : public Piece
{
public:
    Queen(QSvgWidget *parent = nullptr) : Piece{ parent } {}
    Queen(Side side, QSvgWidget *parent = nullptr);
    Queen(Side side, const QString &filepath);

    virtual Type type() const override { return Type::Queen; }
    virtual QVector<Square *> getAvailableSquaresFor(BoardCondition *condition, Square *current, ChessMatrix &matrix) const override;
};

#endif // QUEEN_H

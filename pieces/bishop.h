#ifndef BISHOP_H
#define BISHOP_H

#include "piece.h"

class Bishop : public Piece
{
public:
    Bishop(QSvgWidget *parent = nullptr) : Piece{ parent } {}
    Bishop(Side side, QSvgWidget *parent = nullptr);
    Bishop(Side side, const QString &filepath);

    virtual Type type() const override { return Type::Bishop; }
    virtual QVector<Square *> getAvailableSquaresFor(BoardCondition *condition, Square *current, ChessMatrix &matrix) const override;
    static QVector<Square *> static_getAvailableSquaresFor(BoardCondition *condition, Square *current, ChessMatrix &matrix);
};

#endif // BISHOP_H

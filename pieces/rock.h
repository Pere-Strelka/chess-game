#ifndef ROCK_H
#define ROCK_H

#include "piece.h"

class Rock : public Piece
{
public:
    Rock(QSvgWidget *parent = nullptr) : Piece{ parent } {}
    Rock(Side side, QSvgWidget *parent = nullptr);
    Rock(Side side, const QString &filepath);

    virtual Type type() const override { return Type::Rock; }
    virtual QVector<Square *> getAvailableSquaresFor(BoardCondition *condition, Square *current, ChessMatrix &matrix) const override;
    static QVector<Square *> static_getAvailableSquaresFor(BoardCondition *condition, Square *current, ChessMatrix &matrix);
};

#endif // ROCK_H

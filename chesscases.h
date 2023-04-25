#ifndef CHESSCASES_H
#define CHESSCASES_H

#include "square.h"

class EnPessant
{
public:
    EnPessant() : _null{ true } {}
    EnPessant(Square *pawn, Square *target, short x)
        :  _null{ false }, _x{ x }, _pawn{ pawn }, _target{ target } {}

    void setNull() { _null = true; }

    bool isNull() const { return _null; }
    Square *pawn() const { return _pawn; }
    Square *target() const { return _target; }
    short x() const { return _x;}

private:
    bool _null;
    short _x;
    Square *_pawn, *_target;
};

class BoundedPiece
{
public:
    BoundedPiece() {}
    BoundedPiece(Square *piece, Square *by) : _piece{ piece }, _by{ by } {}

    Square *piece() const  { return _piece; }
    Square *by() const { return _by; }

private:
    Square *_piece;
    Square *_by;
};

#endif // CHESSCASES_H

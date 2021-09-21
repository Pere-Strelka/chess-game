#ifndef MOVE_H
#define MOVE_H

#include <QVector>

#include "side.h"
#include "piece.h"
#include "coord.h"
#include "getid.h"

class Piece;

class Move
{
public:
    Move(Coord oldSqr, Coord newSqr, Piece *piece, unsigned int pieceID, bool attacking = false,
         unsigned int lostPieceID = ID::NoID, bool shortCastles = false, bool longCastles = false,
         bool check = false, bool checkmate = false);

    void setPawnTurnedTo(Piece::Type piece);
    void setCheckmate() { _checkmate = true; }
    void setCheck() { _check = true; }
    void setLongCastles() { _isLongCastles = true; }
    void setShortCastles() { _isShortCastles = true; }
    void setEnPessant() { _enPessant = true; }

    Coord oldSquare() const             { return _oldSquare; }
    Coord newSquare() const             { return _newSquare; }
    Piece *piece() const                { return _piece;     }
    unsigned int lostPieceID() const    { return _lostPieceID; }
    unsigned int pieceID() const        { return _pieceID; }
    bool isAttacking() const            { return _attacking; }
    bool isCheck() const                { return _check;     }
    bool isCheckmate() const            { return _checkmate; }
    Side side() const                   { return _side; }
    bool isLongCastles() const          { return _isLongCastles; }
    bool isShortCastles() const         { return _isShortCastles; }
    bool isEnPessant() const            { return _enPessant; }
    bool isEnPessantPossibleFor(Side side) const;

private:
    Coord _oldSquare, _newSquare;
    Piece *_piece;
    unsigned int _lostPieceID, _pieceID;
    bool _attacking, _isLongCastles, _isShortCastles, _check, _checkmate, _enPessant;
    Side _enPessantPossibleFor, _side;
    Piece::Type _pawnTurnedTo;
};

#endif // MOVE_H

#ifndef SQUARE_H
#define SQUARE_H

#include <QColor>
#include <QRect>
#include <QObject>

#include "coord.h"
#include "side.h"
#include "piece.h"
#include "getid.h"

class Square
{

public:
    Square();
    Square(Square &other);
    Square(Coord coords);
    Square(short x, short y);
    Square(Coord coords, QColor color, QRect rect = QRect());
    Square(short x, short y, QColor color, QRect rect = QRect());

    // setters
    void setCoords(short x, short y)                { _coords = Coord(x, y); }
    void setColor(QColor color)                     { _color = color; }
    void setColor(int r, int g, int b, int a = 255) { _color = QColor(r, g, b, a); }
    void setRect(QRect rect)                        { _rect = rect; }
    void setPiece(Piece *piece);

    // getters
    bool hasPiece() const                           { return _hasPiece; }
    int x() const                                   { return _coords.x(); }
    int y() const                                   { return _coords.y(); }
    Coord coords() const                            { return _coords; }
    unsigned int id() const                         { return _id; }
    QColor color() const                            { return _color; }
    QRect rect() const                              { return _rect; }
    QPoint center() const                           { return _rect.center(); }

    // will return nullptr if there is no piece
    Piece *piece() const                            { return _piece; }
    Piece::Type pieceType() const                   { return _hasPiece ? _piece->type() : Piece::Type::None; }

    // this function returns id of this square's previous piece if there was one and ID::NoID if there wasn't
    unsigned int getPieceFrom(Square *other);
    void removePiece();
    void changePieceTo(Piece *piece) { if (_hasPiece) _piece = piece; else setPiece(piece); }

    bool operator!=(const Square *other) const;
    void operator=(Square other);

private:
    unsigned int _id;
    Coord _coords;
    bool _hasPiece;
    QColor _color;
    QRect _rect;
    Piece *_piece;
};

#endif // SQUARE_H

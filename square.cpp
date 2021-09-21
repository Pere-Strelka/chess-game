#include "square.h"

Square::Square() :  _id{ ID::NoID }, _hasPiece{ false }, _piece{ nullptr }
{
}

Square::Square(Square &other)
{
    _id = other._id;
    _coords = other._coords;
    _hasPiece = other._hasPiece;
    _color = other._color;
    _rect = other._rect;
    _piece = other._piece;
}

Square::Square(Coord coords) : _id{ ID::NoID }, _hasPiece{ false }, _coords{ coords },  _piece{ nullptr }
{
}

Square::Square(short x, short y) : _coords{ x, y }, _id{ ID::NoID }, _hasPiece{ false }, _piece{ nullptr }
{
}

Square::Square(Coord coords, QColor color, QRect rect) : _coords{ coords }, _color{ color }, _rect{ rect },
    _id{ ID::NoID }, _hasPiece{ false }, _piece{ nullptr }
{
}

Square::Square(short x, short y, QColor color, QRect rect) : _coords{ x, y }, _color{ color }, _rect{ rect },
    _id{ ID::NoID }, _hasPiece{ false }, _piece{ nullptr }
{
}

unsigned int Square::getPieceFrom(Square *other)
{ 
    unsigned int temp = _id;
    _piece = other->_piece;
    _id = other->_id;
    other->removePiece();
    _hasPiece = true;
    return temp;
}

void Square::removePiece()
{
    _id = ID::NoID;
    _hasPiece = false;
    _piece = nullptr;
}

void Square::setPiece(Piece *piece)
{
    if (piece == nullptr)
        throw std::exception(std::invalid_argument("piece is null pointer"));
    _piece = piece;
    _hasPiece = true;
    _id = ID::getNew();
}

bool Square::operator!=(const Square *other) const
{
    return _coords != other->_coords;
}

void Square::operator=(Square other)
{
    _id = other._id;
    _coords = other._coords;
    _hasPiece = other._hasPiece;
    _color = other._color;
    _rect = other._rect;
    _piece = other._piece;
}

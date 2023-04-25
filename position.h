#ifndef POSITION_H
#define POSITION_H

#include "chessmatrix.h"
#include <map>

class PositionCode;

using Position = PositionCode;

// This is the realisation for the "Transposition tables"

/*
Codes for squares are:

empty: 0
pawn: 100
rock: 101
knight: 1100
bishop: 1101
queen: 1110
king: 1111

Also there is a prefix for side bebore the code: 1 for white and 0 for black, except 
for empty squares.

*/

class PositionCode
{
public:
    PositionCode(unsigned long long x1 = 0, unsigned long long x2 = 0, 
                 unsigned long long x3 = 0, unsigned long long x4 = 0);

    static PositionCode encode(ChessMatrix& matrix);

    bool operator==(const PositionCode &other) const 
        { return (_x1 == other._x1 && _x2 == other._x2 && _x3 == other._x3 && _x4 == other._x4); }

    bool operator!=(const PositionCode &other) const
        { return !(other == *this); }
    
    QString show() { return QString::number(_x1, 2) + QString::number(_x2, 2) +
                            QString::number(_x3, 2) + QString::number(_x4, 2); }

private:
    unsigned long long _x1, _x2, _x3, _x4;
    static const std::map<Piece::Type, short> _code_table;
};


std::map<Piece::Type, short int> create_table()
{
    std::map<Piece::Type, short> m =
    { { Piece::Type::None,      0b0    },
      { Piece::Type::Pawn,      0b100  },
      { Piece::Type::Rock,      0b101  },
      { Piece::Type::Knight,    0b1100 },
      { Piece::Type::Bishop,    0b1101 },
      { Piece::Type::Queen,     0b1110 },
      { Piece::Type::King,      0b1111 } };
    return m;

}
const std::map<Piece::Type, short> PositionCode::_code_table = create_table();


#endif // POSITION_H

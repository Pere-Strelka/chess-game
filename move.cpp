#include "move.h"
#include "board.h"
#include <cmath>

Move::Move(Coord oldSqr, Coord newSqr, Piece *piece, unsigned int pieceID, bool attacking,
           unsigned int lostPieceID, bool shortCastles, bool longCastles, bool check, bool checkmate)
    : _lostPieceID{ lostPieceID }, _pieceID{ pieceID }, _attacking{ attacking }, _isLongCastles{ longCastles }, _isShortCastles{ shortCastles },
      _check{ check }, _checkmate{ checkmate }, _side{ piece->side() }
{
    _oldSquare = oldSqr;
    _newSquare = newSqr;
    _piece = piece;

    if (_isLongCastles || _isShortCastles)
        return;
    if (_piece->type() == Piece::Type::Pawn) {
        if (abs(_oldSquare.y() - _newSquare.y()) == 2) {
            if (_oldSquare.y() == g_wSecondLine)
                _enPessantPossibleFor = Side::Black;
            else if (_oldSquare.y() == g_bSecondLine)
                _enPessantPossibleFor = Side::White;

        }
    } else
        _enPessantPossibleFor = Side::None;
}

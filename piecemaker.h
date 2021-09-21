#ifndef PIECEMAKER_H
#define PIECEMAKER_H

#include "iconpack.h"
#include "piece.h"
#include "pieces/bishop.h"
#include "pieces/king.h"
#include "pieces/rock.h"
#include "pieces/queen.h"
#include "pieces/pawn.h"
#include "pieces/knight.h"
#include "side.h"

class PieceMaker
{
public:
    PieceMaker(IconPack &pack);

    std::unique_ptr<Piece> make(Piece::Type type, Side side);

private:
    IconPack _icons;

};

#endif // PIECEMAKER_H

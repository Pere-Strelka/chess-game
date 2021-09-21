#include "piecemaker.h"

PieceMaker::PieceMaker(IconPack &pack) : _icons{ pack } {}

std::unique_ptr<Piece> PieceMaker::make(Piece::Type type, Side side)
{
    switch (type)
    {
    case Piece::Type::Bishop:
        return std::make_unique<Bishop>(side, _icons.getBishop(side));
    case Piece::Type::King:
        return std::make_unique<King>(side, _icons.getKing(side));
    case Piece::Type::Queen:
        return std::make_unique<Queen>(side, _icons.getQueen(side));
    case Piece::Type::Rock:
        return std::make_unique<Rock>(side, _icons.getRock(side));
    case Piece::Type::Knight:
        return std::make_unique<Knight>(side, _icons.getKnight(side));
    case Piece::Type::Pawn:
        return std::make_unique<Pawn>(side, _icons.getPawn(side));
    default:
        return nullptr;
    }
}

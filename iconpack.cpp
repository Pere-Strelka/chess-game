#include "iconpack.h"

IconPack::IconPack() {}

void IconPack::setKings(const QString &wFile, const QString &bFile)
{
    _wKingFile = wFile;
    _bKingFile = bFile;
}

void IconPack::setQueens(const QString &wFile, const QString &bFile)
{
    _wQueenFile = wFile;
    _bQueenFile = bFile;
}

void IconPack::setBishops(const QString &wFile, const QString &bFile)
{
    _wBishopFile = wFile;
    _bBishopFile = bFile;
}

void IconPack::setKnights(const QString &wFile, const QString &bFile)
{
    _wKnightFile = wFile;
    _bKnightFile = bFile;
}

void IconPack::setRocks(const QString &wFile, const QString &bFile)
{
    _wRockFile = wFile;
    _bRockFile = bFile;
}

void IconPack::setPawns(const QString &wFile, const QString &bFile)
{
    _wPawnFile = wFile;
    _bPawnFile = bFile;
}

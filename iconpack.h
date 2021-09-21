#ifndef ICONPACK_H
#define ICONPACK_H

#include <QString>

#include "side.h"

class IconPack
{
public:
    IconPack();

    inline QString getKing(Side side)   const { return side == Side::White ? _wKingFile : _bKingFile; }
    inline QString getQueen(Side side)  const { return side == Side::White ? _wQueenFile : _bQueenFile; }
    inline QString getBishop(Side side) const { return side == Side::White ? _wBishopFile : _bBishopFile; }
    inline QString getKnight(Side side) const { return side == Side::White ? _wKnightFile : _bKnightFile; }
    inline QString getRock(Side side)   const { return side == Side::White ? _wRockFile : _bRockFile; }
    inline QString getPawn(Side side)   const { return side == Side::White ? _wPawnFile : _bPawnFile; }

    void setKings(const QString &wFile, const QString &bFile);
    void setQueens(const QString &wFile, const QString &bFile);
    void setBishops(const QString &wFile, const QString &bFile);
    void setKnights(const QString &wFile, const QString &bFile);
    void setRocks(const QString &wFile, const QString &bFile);
    void setPawns(const QString &wFile, const QString &bFile);

private:
    QString _wKingFile, _bKingFile, _wQueenFile, _bQueenFile, _wBishopFile, _bBishopFile,
            _wKnightFile, _bKnightFile, _wRockFile, _bRockFile, _wPawnFile, _bPawnFile;
};

#endif // ICONPACK_H

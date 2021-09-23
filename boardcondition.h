#ifndef BOARDCONDITION_H
#define BOARDCONDITION_H

#include <QObject>

#include <map>

#include "square.h"
#include "chessmatrix.h"
#include "side.h"
#include "move.h"
#include "chesscases.h"
#include "constants.h"

class BoardCondition : public QObject
{
    Q_OBJECT

    friend class Piece;
    friend class Bishop;
    friend class King;
    friend class Queen;
    friend class Knight;
    friend class Rock;
    friend class Pawn;

    friend void squarePressedEquivalent(Square *available, BoardCondition *condition);

public:
    BoardCondition();

    void initCondition(ChessMatrix &matrix);

    // setters
    void setSelected(Square *sqr);
    void removeSelected() { _selected = nullptr; }

    // getters
    bool isLongCastlesPossibleFor(Side side) const;
    bool isShortCastlesPossibleFor(Side side) const;
    bool isSquareSafeFor(Side side, Square *sqr) const;
    bool isSquareProtected(Square *sqr) const;
    Side whichKingIsUnderCheck() const;

    Move *lastMove()            const { return _lastMove; }
    bool isSquareSelected()     const { return _selected == nullptr ? false : true; }
    bool isCheckmate()          const { return _bCheckmate || _wCheckmate; }
    bool isWhitesTurn()         const { return _turn == Side::White; }
    Side turn()                 const { return _turn; }
    const QVector<Square *> &availableSquaresFor(Square *sqr) const;

    Square *king(Side side);
    Square *longRock(Side side);
    Square *shortRock(Side side);
    Square *selected()              { return _selected; }
    Square *square(unsigned int id) { return _pieces[id]; }
    EnPessant &enPessant()          { return _enPessantCase; }

signals:
    void gameOver(Side won);

public slots:
    void processBoard(ChessMatrix &matrix, Move *last = nullptr);

private:
    void setKingChecked(Side side, Square *by);
    void addUnsafeFor(Side side, Square *sqr);
    void addProtected(Square *sqr);
    void addEnPessantCase(EnPessant epCase) { _enPessantCase = epCase; }

    static bool isKingUnderCheck(ChessMatrix &matrix, Side side, Square *piece, Square *available);

    QVector<Square *> _protectedSquares;
    QVector<Square *> _unsafeForWhiteSquares;
    QVector<Square *> _unsafeForBlackSquares;
    QVector<Square *> _kingCheckers;
    std::map<unsigned int, Square *> _pieces;
    std::map<unsigned int, QVector<Square *>> _availableSquares;
    Square *_selected;
    Move *_lastMove;
    bool _wKingUnderCheck, _bKingUnderCheck, _wCheckmate, _bCheckmate,
         _wLongCastlesPossible, _wShortCastlesPossible, _bLongCastlesPossible, _bShortCastlesPossible,
         _wKingMoved, _bKingMoved, _wLongRockMoved, _wShortRockMoved, _bLongRockMoved, _bShortRockMoved;
    Side _turn;
    unsigned int _wKingID, _bKingID, _wLongRockID, _wShortRockID, _bLongRockID, _bShortRockID;

    // en pessant variable
    EnPessant _enPessantCase;
};

#endif // BOARDCONDITION_H

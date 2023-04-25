#include "boardcondition.h"

BoardCondition::BoardCondition()
    : _selected{ nullptr }, _lastMove{ nullptr },
      _wKingUnderCheck{ false }, _bKingUnderCheck{ false }, _wCheckmate{ false }, _bCheckmate{ false },
      _wLongCastlesPossible{ false }, _wShortCastlesPossible{ false },
      _bLongCastlesPossible{ false }, _bShortCastlesPossible{ false },
      _wKingMoved{ false }, _bKingMoved{ false },
      _wLongRockMoved{ false }, _wShortRockMoved{ false },
      _bLongRockMoved{ false }, _bShortRockMoved{ false },
      _wKingID{ ID::NoID }, _bKingID{ ID::NoID },
      _wLongRockID{ ID::NoID }, _wShortRockID{ ID::NoID },
      _bLongRockID{ ID::NoID }, _bShortRockID{ ID::NoID },
      _turn{ Side::None }
{}

void BoardCondition::nullify()
{
    _protectedSquares.clear();
    _unsafeForBlackSquares.clear();
    _unsafeForWhiteSquares.clear();
    _kingCheckers.clear();
    _pieces.clear();
    _availableSquares.clear();
    _selected = nullptr;
    _lastMove = nullptr;
    _enPessantCase.setNull();
    _wKingUnderCheck = false; _bKingUnderCheck = false;
    _wCheckmate = false; _bCheckmate = false;
    _wLongCastlesPossible = false; _wShortCastlesPossible = false;
    _bLongCastlesPossible = false; _bShortCastlesPossible = false;
    _wKingMoved = false; _bKingMoved = false;
    _wLongRockMoved = false; _wShortRockMoved = false;
    _bLongRockMoved = false; _bShortRockMoved = false;
    _wKingID = ID::NoID; _bKingID = ID::NoID;
    _wLongRockID = ID::NoID; _wShortRockID = ID::NoID;
    _bLongRockID = ID::NoID; _bShortRockID = ID::NoID;
    _turn = Side::None;
}

void BoardCondition::setSelected(Square *sqr)
{
    if (!sqr->hasPiece() && _selected == nullptr)
        return;
    else if (!sqr->hasPiece() && _selected != nullptr)
        _selected = nullptr;
    else
        _selected = sqr;
}

// this function inits condition class of the board by copying all the important pieces id to variables and
// all the pieces id and their squares to hash map _pieces
void BoardCondition::initCondition(ChessMatrix &matrix)
{
    _lastMove = nullptr;
    _selected = nullptr;
    for (short x = 0; x < 8; x++) {
        for (short y = 0; y < 8; y++) {
            Square *current = &matrix(x, y);
            if (current->id() != ID::NoID) {
                if (current->piece()->type() == Piece::Type::King) {
                    if (current->piece()->side() == Side::White)
                        _wKingID = current->id();
                    else
                        _bKingID = current->id();

                } else if (current->piece()->type() == Piece::Type::Rock) {
                    if (current->piece()->side() == Side::White) {
                        if (current->x() == 0)
                            _wLongRockID = current->id();
                        else if (current->x() == 7)
                            _wShortRockID = current->id();
                    } else {
                        if (current->x() == 0)
                            _bLongRockID = current->id();
                        else if (current->x() == 7)
                            _bShortRockID = current->id();
                    }
                }
                _pieces.insert(std::pair(current->id(), current));
            }
        }
    }
    processBoard(matrix, _lastMove);
}

bool BoardCondition::isLongCastlesPossibleFor(Side side) const
{
    switch (side)
    {
    case Side::Black:
        return _bLongCastlesPossible;
    case Side::White:
        return _wLongCastlesPossible;
    default:
        return false;
    }
}

bool BoardCondition::isShortCastlesPossibleFor(Side side) const
{
    switch (side)
    {
    case Side::Black:
        return _bShortCastlesPossible;
    case Side::White:
        return _wShortCastlesPossible;
    default:
        return false;
    }
}

bool BoardCondition::isSquareSafeFor(Side side, Square *sqr) const
{
    switch (side)
    {
    case Side::Black:
        return !_unsafeForBlackSquares.contains(sqr);
    case Side::White:
        return !_unsafeForWhiteSquares.contains(sqr);
    default:
        return false;
    }
}

bool BoardCondition::isSquareProtected(Square *sqr) const
{
    return _protectedSquares.contains(sqr);
}

Side BoardCondition::whichKingIsUnderCheck() const
{
    if (_wKingUnderCheck)
        return Side::White;
    else if (_bKingUnderCheck)
        return Side::Black;
    else
        return Side::None;
}

const QVector<Square *> &BoardCondition::availableSquaresFor(Square *sqr) const
{
    return _availableSquares.at(sqr->id());
}

Square *BoardCondition::king(Side side)
{
    switch (side)
    {
    case Side::Black:
        return _pieces[_bKingID];
    case Side::White:
        return _pieces[_wKingID];
    default:
        return nullptr;
    }
}

Square *BoardCondition::longRock(Side side)
{
    switch (side)
    {
    case Side::Black:
        return _pieces[_bLongRockID];
    case Side::White:
        return _pieces[_wLongRockID];
    default:
        return nullptr;
    }
}

Square *BoardCondition::shortRock(Side side)
{
    switch (side)
    {
    case Side::Black:
        return _pieces[_bShortRockID];
    case Side::White:
        return _pieces[_wShortRockID];
    default:
        return nullptr;
    }
}

void BoardCondition::processBoard(ChessMatrix &matrix, Move *last)
{
    _lastMove = last;

    auto checkCastles = [this, &matrix] () {
        auto areSquaresEmtpyAndSafeFor = [&matrix, this] (Side side, short fromX, short toX, short y) {
            for (short x = fromX; x <= toX; x++) {
                if (matrix(x, y).hasPiece())
                    return false;
                switch (side)
                {
                case Side::White:
                    if (_unsafeForWhiteSquares.contains(&matrix(x, y)))
                        return false;
                    else
                        continue;
                case Side::Black:
                    if (_unsafeForBlackSquares.contains(&matrix(x, y)))
                        return false;
                default: ;
                }
            }
            return true;
        };

        if (_lastMove->pieceID() == _wKingID) {
            _wKingMoved = true;
            _wShortCastlesPossible = false;
            _wLongCastlesPossible = false;
        } else if (_lastMove->pieceID() == _bKingID) {
            _bKingMoved = true;
            _bShortCastlesPossible = false;
            _bLongCastlesPossible = false;
        }
        if (_lastMove->pieceID() == _wLongRockID || _wLongRockID == ID::NoID) {
            _wLongRockMoved = true;
            _wLongCastlesPossible = false;
        }
        if (_lastMove->pieceID() == _wShortRockID || _wShortRockID == ID::NoID) {
            _wShortRockMoved = true;
            _wShortCastlesPossible = false;
        }
        if (_lastMove->pieceID() == _bLongRockID || _bLongRockID == ID::NoID) {
            _bLongRockMoved = true;
            _bLongCastlesPossible = false;
        }
        if (_lastMove->pieceID() == _bShortRockID || _bShortRockID == ID::NoID) {
            _bShortRockMoved = true;
            _bShortCastlesPossible = false;
        }

        if (!_wLongRockMoved && !_wKingMoved && !_wKingUnderCheck &&
            areSquaresEmtpyAndSafeFor(Side::White, /* from x */ 1, /* to x */ 3, /* y = */ g_wFirstLine))
            _wLongCastlesPossible = true;
        else
            _wLongCastlesPossible = false;

        if (!_wShortRockMoved && !_wKingMoved && !_wKingUnderCheck &&
            areSquaresEmtpyAndSafeFor(Side::White, /* from x */ 5, /* to x */ 6, /* y = */ g_wFirstLine))
            _wShortCastlesPossible = true;
        else
            _wShortCastlesPossible = false;

        if (!_bLongRockMoved && !_bKingMoved && !_bKingUnderCheck &&
            areSquaresEmtpyAndSafeFor(Side::Black, /* from x */ 1, /* to x */ 3, /* y = */ g_bFirstLine))
            _bLongCastlesPossible = true;
        else
            _bLongCastlesPossible = false;

        if (!_bShortRockMoved && !_bKingMoved && !_bKingUnderCheck &&
            areSquaresEmtpyAndSafeFor(Side::Black, /* from x */ 5, /* to x */ 6, /* y = */ g_bFirstLine))
            _bShortCastlesPossible = true;
        else
            _bShortCastlesPossible = false;
    };

    // this block changes _pieces according to the last move
    if (_lastMove != nullptr) {
        if (_lastMove->isAttacking()) {
            _pieces.erase(_lastMove->lostPieceID());
            if (_lastMove->lostPieceID() == _wShortRockID)
                _wShortRockID = ID::NoID;
            if (_lastMove->lostPieceID() == _wLongRockID)
                _wLongRockID = ID::NoID;
            if (_lastMove->lostPieceID() == _bShortRockID)
                _bShortRockID = ID::NoID;
            if (_lastMove->lostPieceID() == _bLongRockID)
                _bLongRockID = ID::NoID;
        }
        if (_lastMove->isLongCastles() || _lastMove->isShortCastles()) {
            bool sideIsWhite = _lastMove->side() == Side::White ? true : false;
            short y = sideIsWhite ? g_wFirstLine : g_bFirstLine;
            unsigned int rock, king;
            short xRock, xKing;
            if (_lastMove->isLongCastles()) {
                rock = sideIsWhite ? _wLongRockID : _bLongRockID;
                king = sideIsWhite ? _wKingID : _bKingID;
                xRock = 3;
                xKing = 2;
            } else {
                rock = sideIsWhite ? _wShortRockID : _bShortRockID;
                king = sideIsWhite ? _wKingID : _bKingID;
                xRock = 5;
                xKing = 6;
            }

            _pieces.insert_or_assign(rock, &matrix(xRock, y));
            _pieces.insert_or_assign(king, &matrix(xKing, y));
        } else {
            Coord coords = _lastMove->newSquare();
            _pieces.insert_or_assign(_lastMove->pieceID(), &matrix(coords.x(), coords.y()));
        }
    }

    _protectedSquares.clear();
    _unsafeForWhiteSquares.clear();
    _unsafeForBlackSquares.clear();
    _kingCheckers.clear();

    _wKingUnderCheck = false;
    _bKingUnderCheck = false;

    _enPessantCase.setNull();

    // this block processes board to find possible moves
    for (auto &each : _pieces) {
        Square *current = each.second;
        if (current->id() == _wKingID)
            continue;
        else if (current->id() == _bKingID)
            continue;
        else
            _availableSquares.insert_or_assign(current->id(), current->piece()->getAvailableSquaresFor(this, current, matrix));
    }

    QVector<Square *> wKingAvailables = _pieces[_wKingID]->piece()->getAvailableSquaresFor(this, _pieces[_wKingID], matrix);
    QVector<Square *> bKingAvailables = _pieces[_bKingID]->piece()->getAvailableSquaresFor(this, _pieces[_bKingID], matrix);

    _availableSquares.insert_or_assign(_wKingID, wKingAvailables);
    _availableSquares.insert_or_assign(_bKingID, bKingAvailables);

    // this block calls checkCastles and also removes invalid moves
    if (last != nullptr) {
        checkCastles();
        for (auto &each : _pieces) {
            if (each.second->piece()->side() == _turn)
                continue;
            for (short i = 0; i < _availableSquares[each.first].length(); i++) {
                ChessMatrix matrixCopy = matrix;
                Square availableSquareCopy = matrixCopy(_availableSquares[each.first][i]->x(), _availableSquares[each.first][i]->y());
                Square pieceCopy = *each.second;
                if (isKingUnderCheck(matrixCopy, each.second->piece()->side(), &pieceCopy, &availableSquareCopy)) {
                    _availableSquares[each.first].remove(i);
                    i--;
                }
            }
        }
    }

    if (_wShortCastlesPossible)
       _availableSquares[_wKingID].append(_pieces[_wShortRockID]);
    if (_wLongCastlesPossible)
        _availableSquares[_wKingID].append(_pieces[_wLongRockID]);
    if (_bShortCastlesPossible)
       _availableSquares[_bKingID].append(_pieces[_bShortRockID]);
    if (_bLongCastlesPossible)
        _availableSquares[_bKingID].append(_pieces[_bLongRockID]);

    // this block detects checkmate and draw
    if (last != nullptr) {
        auto noAvailableMoves = [this] (Side kingSide) {
            for (auto each : _pieces) {
                if (each.second->piece()->side() != kingSide)
                    continue;
                if (!_availableSquares[each.first].isEmpty())
                    return false;
            }
            return true;
        };

        if (_wKingUnderCheck || _bKingUnderCheck) {
            Side kingSide = _wKingUnderCheck ? Side::White : Side::Black;
            last->setCheck();

            unsigned int checkedKingID = _wKingUnderCheck ? _wKingID : _bKingID;
            if (_availableSquares[checkedKingID].isEmpty()) {
                if (noAvailableMoves(kingSide)) {
                    last->setCheckmate();
                    if (_wKingUnderCheck)
                        _bCheckmate = true;
                    else
                        _wCheckmate = true;
                    emit gameOver(_wKingUnderCheck ? Side::Black : Side::White);
                }
            }
        } else if (noAvailableMoves(Side::White) || noAvailableMoves(Side::Black)) {
            _bCheckmate = true;
            _wCheckmate = true;
            emit gameOver(Side::None);
        }
    }
    _turn = _turn == Side::White ? Side::Black : Side::White;
}

void BoardCondition::setKingChecked(Side side, Square *by)
{
    _kingCheckers.append(by);
    switch (side)
    {
    case Side::White:
        _wKingUnderCheck = true;
        return;
    case Side::Black:
        _bKingUnderCheck = true;
        return;
    default:
        return;
    }
}

void BoardCondition::addUnsafeFor(Side side, Square *sqr)
{
    switch (side)
    {
    case Side::White:
        _unsafeForWhiteSquares.append(sqr);
        return;
    case Side::Black:
        _unsafeForBlackSquares.append(sqr);
        return;
    default:
        return;
    }
}

void BoardCondition::addProtected(Square *sqr)
{
    _protectedSquares.append(sqr);
}

// this function simulate Board::squarePressed() functional (only these isKingUnderCheck() needs)
void squarePressedEquivalent(Square *available, BoardCondition *condition) {
    bool isAttacking = false;
    unsigned int lostPieceID = ID::NoID;
    auto processMove = [&available, &condition, &isAttacking, &lostPieceID] () {
        if (available->hasPiece()) {
            isAttacking = true;
            lostPieceID = available->id();
        }
        condition->_pieces.erase(condition->selected()->id());
        available->getPieceFrom(condition->selected());
    };

    auto processEnPessant = [&available, &condition, &processMove, &isAttacking, &lostPieceID] () {
        EnPessant epCase = condition->enPessant();
        if (!epCase.isNull() && available->x() == epCase.x() && condition->selected() == epCase.pawn()) {
            isAttacking = true;
            lostPieceID = epCase.target()->id();
            condition->_pieces.erase(condition->selected()->id());
            available->getPieceFrom(condition->selected());
            epCase.target()->removePiece();
        } else
            processMove();
    };

    if (condition->selected()->piece()->type() == Piece::Type::Pawn)
        processEnPessant();
    else
        processMove();

    if (isAttacking) {
        condition->_pieces.erase(lostPieceID);
    }
    condition->_pieces.insert_or_assign(available->id(), available);
}

// this function checks if move of piece to available is possible and doesn't lead piece's king to be under check
bool BoardCondition::isKingUnderCheck(ChessMatrix &matrix, Side side, Square *piece, Square *available)
{
    BoardCondition *cond = new BoardCondition();
    for (short x = 0; x < 8; x++) {
        for (short y = 0; y < 8; y++) {
            Square *current = &matrix(x, y);
            if (current->id() != ID::NoID) {
                if (current->piece()->type() == Piece::Type::King) {
                    if (current->piece()->side() == Side::White)
                        cond->_wKingID = current->id();
                    else
                        cond->_bKingID = current->id();

                }
                cond->_pieces.insert_or_assign(current->id(), current);
            }
        }
    }

    cond->setSelected(&matrix(piece->x(), piece->y()));

    squarePressedEquivalent(&matrix(available->x(), available->y()), cond);

    for (auto &each : cond->_pieces) {
        Square *current = each.second;
        if (current->id() == ID::NoID)
            continue;
        if (current->id() == cond->_wKingID)
            continue;
        else if (current->id() == cond->_bKingID)
            continue;
        else if (current->piece()->side() == side)
            continue;
        else
            current->piece()->getAvailableSquaresFor(cond, current, matrix);
    }

    if (side == Side::White ? cond->_wKingUnderCheck : cond->_bKingUnderCheck) {
        delete cond;
        return true;
    } else {
        delete cond;
        return false;
    }
}

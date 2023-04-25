#include "board.h"
#include "piecemaker.h"
#include <QSvgRenderer>

Board::Board(QWidget *parent) : QWidget(parent), _painter{ new QPainter() }
{
    _lightTextColor = QColor(220, 221, 225);
    _background = QBrush(QColor(47, 54, 64));
    _whiteColor = QColor(210, 218, 226);
    _blackColor = QColor(72, 84, 96);
    _grayMarkColor = QColor(149, 165, 166, 100);
    _blueForSelectedColor = QColor(9, 132, 227);
    _redForCheckColor = QColor(214, 48, 49);
    _outlineWidth = 2.5f;

    setFixedSize(1000, 1000);

    loadIcons();

    reset();

    QObject::connect(this, &Board::newMove, &_condition, &BoardCondition::processBoard);
    QObject::connect(&_condition, &BoardCondition::gameOver, this, &Board::processCheckmate);
}

Board::~Board() { delete _painter; }

void Board::reset()
{
    _condition.nullify();
    _pieceHolder.clear();
    _history.clear();

    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            _matrix(x, y) = Square(x, y, ((y % 2 == 0 && x % 2 == 0) || (y % 2 != 0 && x % 2 != 0)) ? _whiteColor : _blackColor);
        }
    }

    initPieces();
}

void Board::squarePressed(Square *sqr, QMouseEvent *event)
{
    // this block aborts function if sqr isn't smth to process (also removes selected in that case)
    if (!(_condition.isSquareSelected() && _condition.availableSquaresFor( _condition.selected() ).contains(sqr)) &&
            sqr->hasPiece() && sqr->piece()->side() != _condition.turn()) {
        _condition.removeSelected();
        return;
    }

    auto processMove = [&sqr, this] () {
        unsigned int targetID = _matrix(sqr->x(), sqr->y()).getPieceFrom(_condition.selected());
        _history.push_back(Move(_condition.selected()->coords(), _matrix(sqr->x(), sqr->y()).coords(),
                _matrix(sqr->x(), sqr->y()).piece(), _matrix(sqr->x(), sqr->y()).id(),
                /* is move attacking */ (targetID == ID::NoID ? false : true), targetID));
    };

    auto processEnPessant = [&sqr, this] () {
        EnPessant *epCase = &_condition.enPessant();
        unsigned int id = epCase->target()->id();
        epCase->target()->removePiece();
        _history.push_back(Move(_condition.selected()->coords(), _matrix(sqr->x(), sqr->y()).coords(),
                    _matrix(sqr->x(), sqr->y()).piece(), _matrix(sqr->x(), sqr->y()).id(),
                    /* is move attacking */ true, id));
        _history.last().setEnPessant();
    };

    auto processCastles = [&sqr, this] () {
        Side currentSide = _condition.selected()->piece()->side();
        if (sqr == _condition.longRock(currentSide)) {
            Piece *piece = _condition.selected()->piece();
            _matrix(3, sqr->y()).getPieceFrom(_condition.longRock(currentSide));
            _matrix(2, sqr->y()).getPieceFrom(_condition.selected());
            _history.push_back(Move(_condition.selected()->coords(), Coord(2, sqr->y()),
                    piece, ID::NoID, /* is attacking */ false, ID::NoID, false /* isShortCastles */, true /* isLongCastles */));
        } else if (sqr == _condition.shortRock(currentSide)) {
            Piece *piece = _condition.selected()->piece();
            _matrix(5, sqr->y()).getPieceFrom(_condition.shortRock(currentSide));
            _matrix(6, sqr->y()).getPieceFrom(_condition.selected());
            _history.push_back(Move(_condition.selected()->coords(), Coord(6, sqr->y()),
                    piece, ID::NoID, /* is attacking */ false, ID::NoID, true /* isShortCastles */, false /* isLongCastles */));
        }
    };

    // this block processes move and calls BoardCondition::processBoard() or sets the selected square if there's no yet

    // on move (when there's already a square selected and second selected is correct square)
    if (_condition.isSquareSelected() && _condition.availableSquaresFor(_condition.selected()).contains(sqr)) {
        if (_condition.selected()->piece()->type() == Piece::Type::Pawn) {

            // if the moving piece is a pawn and _condition says there can be an en pessant,
            // then we check for the vertical coord
            if (!_condition.enPessant().isNull() && sqr->x() == _condition.enPessant().x() &&
                _condition.selected() == _condition.enPessant().pawn())
                processEnPessant();
            else {
                processMove();
                if (sqr->y() == (sqr->piece()->side() == Side::White ? g_bFirstLine : g_wFirstLine)) {
                    PieceMaker maker(_pack);
                    _pieceHolder.push_back(maker.make(Piece::Type::Queen, sqr->piece()->side()));
                    auto it = _pieceHolder.end();
                    std::advance(it, -1);
                    sqr->changePieceTo((*it).get());
                }
            }
        }
        else if (_condition.selected()->piece()->type() == Piece::Type::King && (sqr->x() == 0 || sqr->x() == 7) &&
                 (_condition.isLongCastlesPossibleFor(_condition.selected()->piece()->side()) ||
                 _condition.isShortCastlesPossibleFor(_condition.selected()->piece()->side())))
            processCastles();
        else
            processMove();
        _condition.removeSelected();
        update();
        emit newMove(_matrix, &_history[_history.size() - 1]);
    } else
        _condition.setSelected(sqr);
}

void Board::processCheckmate(Side won)
{
    _gameOverText = _gameOverTexts[won];
}

void Board::mousePressEvent(QMouseEvent *event)
{
    if (_condition.isCheckmate())
        return;
    int xSqr = qFloor( (event->position().x() - _automaticMarginX) / _squareWidth );
    int ySqr = qFloor( (event->position().y() - _automaticMarginY) / _squareWidth);
    squarePressed(&_matrix(xSqr, ySqr), event);
}

void Board::paintEvent(QPaintEvent *event)
{
    _painter->begin(this);
    paint(_painter, event);
    _painter->end();
}

void Board::paint(QPainter *painter, QPaintEvent *event)
{
    QPen pen(Qt::SolidLine);

    // this lamda draws border within given square (is used for selected square and for highlighting checked king)
    auto paintBorders = [&painter, this] (Square *square) {
        int xOffset = _automaticMarginX + (_condition.isCheckmate() ? _squareWidth : 0);
        int yOffset = _automaticMarginY;

        QPoint topLeft = QPoint(xOffset + square->x() * _squareWidth,
                                yOffset + square->y() * _squareWidth);

        QPoint topRight = QPoint(xOffset + square->x() * _squareWidth + _squareWidth,
                                 yOffset + square->y() * _squareWidth);

        QPoint bottomLeft = QPoint(xOffset + square->x() * _squareWidth,
                                   yOffset + square->y() * _squareWidth + _squareWidth);

        QPoint bottomRight = QPoint(xOffset + square->x() * _squareWidth + _squareWidth,
                                    yOffset + square->y() * _squareWidth + _squareWidth);

        QVector<QLine> lines;
        lines.push_back(QLine(topLeft, topRight));
        lines.push_back(QLine(bottomLeft, bottomRight));
        lines.push_back(QLine(topLeft, bottomLeft));
        lines.push_back(QLine(topRight, bottomRight));
        painter->drawLines(lines);
    };

    QRect rectangle = event->rect();
    painter->fillRect(rectangle, _background);

    // when game's over, label "*side* wins!" appears under the board
    // so we need to draw board differently in this case

    const short c_normalSquareWidthDivider = 8;
    const short c_gameOverSquareWidthDivider = 10;

    short usedDivider = _condition.isCheckmate() ?
                            c_gameOverSquareWidthDivider : c_normalSquareWidthDivider;

    _squareWidth = rectangle.width() > rectangle.height() ?
                       rectangle.height() / usedDivider : rectangle.width() / usedDivider;

    _automaticMarginX = (rectangle.width() - _squareWidth * usedDivider) / 2;
    _automaticMarginY = (rectangle.height() - _squareWidth * usedDivider) / 2;

    if (_condition.isCheckmate()) {
        rectangle.setX(_squareWidth);

        // drawing the label
        pen.setColor(_lightTextColor);
        QFont font;

        // params: x, y, width, height
        QRectF textRect = QRectF(_automaticMarginX + _squareWidth * 1,_automaticMarginY + _squareWidth * 8,
                                 _squareWidth * 8, _squareWidth * 2);
        font.setPointSize(textRect.width() * 0.1);
        painter->setFont(font);
        painter->setPen(pen);
        painter->drawText(textRect, Qt::AlignCenter, _gameOverText);
    }


    // drawing the board
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            _matrix(x, y).setRect(QRect(_automaticMarginX + rectangle.x() + x * _squareWidth,
                                        _automaticMarginY + rectangle.y() + y * _squareWidth,
                                        _squareWidth, _squareWidth));
            painter->fillRect(_matrix(x, y).rect(), _matrix(x, y).color());
        }
    }

    // calling paint function for each piece
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            if (_matrix(x, y).hasPiece()) {
                double widthMultiplier = 1;
                if (_matrix(x, y).piece()->type() == Piece::Type::Pawn)
                    widthMultiplier = g_widthMultiplierForPawns;
                else if (_matrix(x, y).piece()->type() == Piece::Type::Rock)
                    widthMultiplier = g_widthMultiplierForRocks;

                int xCoord = _automaticMarginX + rectangle.x() + x * _squareWidth + ((1 - g_pieceSizeMulpitplier * widthMultiplier) / 2 * _squareWidth);
                int yCoord = _automaticMarginY + rectangle.y() + y * _squareWidth + ((1 - g_pieceSizeMulpitplier) / 2 * _squareWidth);
                _matrix(x, y).setRect(QRect(xCoord, yCoord,
                                            _squareWidth * g_pieceSizeMulpitplier * widthMultiplier,
                                            _squareWidth * g_pieceSizeMulpitplier));
                _matrix(x, y).piece()->renderer()->render(painter, _matrix(x, y).rect());
            }
        }
    }


    // calling paintBorders if a king is checked OR if there's a selected square
    Square *sqrToShowCheck = _condition.king(_condition.whichKingIsUnderCheck());

    if (sqrToShowCheck != nullptr || _condition.selected() != nullptr) {
        QPen outline(Qt::SolidLine);
        outline.setWidth(_outlineWidth);

        if (_condition.selected() != nullptr) {
            outline.setColor(_blueForSelectedColor);
            painter->setPen(outline);
            paintBorders(_condition.selected());
        }
        if (sqrToShowCheck != nullptr) {
            outline.setColor(_redForCheckColor);
            painter->setPen(outline);
            paintBorders(sqrToShowCheck);
        }
    }

    // marking available to move squares with a circle
    pen.setColor(_grayMarkColor);
    painter->setPen(pen);
    QBrush brush(_grayMarkColor);
    painter->setBrush(brush);

    if (_condition.selected() != nullptr) {
        QVector<Square *> availables = _condition.availableSquaresFor(_condition.selected());
        int radius = static_cast<int>(_squareWidth * g_radiusMultiplier);
        for (auto each : availables) {
            painter->drawEllipse(each->center(), radius, radius);
        }
    }
}

void Board::initPieces()
{
    PieceMaker pieceMaker(_pack);

    for (int i = 0; i < 2; i++) {
        short yForPieces =  i ? g_bFirstLine : g_wFirstLine;
        short yForPawns =   i ? g_bSecondLine : g_wSecondLine;
        Side side =         i ? Side::Black : Side::White;

        for (auto pieceType : _piecesOrder)
            _pieceHolder.push_back(pieceMaker.make(pieceType, side));

        auto it = _pieceHolder.end();
        std::advance(it, -8);
        for (int i = 0; i < 8; i++) {
            _matrix(i, yForPieces).setPiece((*it).get());
            it = std::next(it);
        }

        for (int x = 0; x < 8; x++) {
            _pieceHolder.push_back(pieceMaker.make(Piece::Type::Pawn, side));
            auto it = _pieceHolder.end();
            std::advance(it, -1);
            _matrix(x, yForPawns).setPiece((*it).get());
        }
    }
    _condition.initCondition(_matrix);
}

void Board::loadIcons()
{
    _pack.setKings("icons/w-king.svg", "icons/b-king.svg");
    _pack.setQueens("icons/w-queen.svg", "icons/b-queen.svg");
    _pack.setBishops("icons/w-bishop.svg", "icons/b-bishop.svg");
    _pack.setKnights("icons/w-knight.svg", "icons/b-knight.svg");
    _pack.setRocks("icons/w-rock.svg", "icons/b-rock.svg");
    _pack.setPawns("icons/w-pawn.svg", "icons/b-pawn.svg");
}

#ifndef BOARD_H
#define BOARD_H

#include <QWidget>
#include <QObject>
#include <QPaintEvent>
#include <QPainter>
#include <QtMath>
#include <QMap>

#include "iconpack.h"
#include "boardcondition.h"
#include "move.h"
#include "chessmatrix.h"
#include "constants.h"

class Board : public QWidget
{
    Q_OBJECT

public:
    explicit Board(QWidget *parent = nullptr);
    ~Board();

    void reset();

public slots:
    void squarePressed(Square *sqr, QMouseEvent *event);
    void processCheckmate(Side won);
    void updateBoard() { update(); }

signals:
    void gameOver(Side side);
    void newMove(ChessMatrix &matrix, Move *last);

protected:
    void paint(QPainter *painter, QPaintEvent *event);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);

private:
    void initPieces();
    void loadIcons();

    // Automatic margin values are used to center the board in the window
    int _automaticMarginX = 0, _automaticMarginY = 0;
    int _squareWidth;
    ChessMatrix _matrix;
    std::list< std::unique_ptr<Piece> > _pieceHolder;
    QBrush _background;
    // Colors for painting
    QColor _whiteColor, _blackColor, _lightTextColor, _grayMarkColor, _blueForSelectedColor, _redForCheckColor;
    // Width of highlight outline for painting
    float _outlineWidth;
    QString _gameOverText;
    BoardCondition _condition;
    IconPack _pack;
    QVector<Move> _history;
    QPainter *_painter;



    const QMap<Side, QString> _gameOverTexts =
        {
            { Side::Black, "Black wins!" },
            { Side:: White, "White wins!" },
            { Side::None, "Stalemate!" },
        };

    const Piece::Type _piecesOrder[8] =
        {
        Piece::Type::Rock,
        Piece::Type::Knight,
        Piece::Type::Bishop,
        Piece::Type::Queen,
        Piece::Type::King,
        Piece::Type::Bishop,
        Piece::Type::Knight,
        Piece::Type::Rock
        };
};

#endif // BOARD_H

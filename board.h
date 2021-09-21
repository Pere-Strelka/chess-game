#ifndef BOARD_H
#define BOARD_H

#include <QWidget>
#include <QObject>
#include <QPaintEvent>
#include <QPainter>
#include <QtMath>

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

    int _squareWidth;
    ChessMatrix _matrix;
    std::list< std::unique_ptr<Piece> > _pieceHolder;
    QBrush _background;
    QColor _whiteColor, _blackColor, _lightTextColor, _grayMarkColor, _blueForSelectedColor, _redForCheckColor;
    QString _gameOverText;
    BoardCondition _condition;
    IconPack _pack;
    QVector<Move> _history;
    QPainter *_painter;
};

#endif // BOARD_H

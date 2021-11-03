#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "board.h"
#include <QGridLayout>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Chess");

    _board = new Board(this);

    auto newGame = [this]() {
        _board->reset();
    };

    ui->menubar->addMenu("Menu")->addAction("New game", newGame);

    setMinimumSize(400, 400);
    setMaximumSize(1000, 1000);

    this->setCentralWidget(_board);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, _board, &Board::updateBoard);
    timer->start(20);
}

MainWindow::~MainWindow()
{
    delete ui;
}


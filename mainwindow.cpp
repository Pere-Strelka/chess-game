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

    Board *board = new Board(this);

    setMinimumSize(400, 400);
    setMaximumSize(1000, 1000);

    this->setCentralWidget(board);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, board, &Board::updateBoard);
    timer->start(20);
}

MainWindow::~MainWindow()
{
    delete ui;
}


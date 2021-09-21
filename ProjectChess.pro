QT       += core gui svg svgwidgets


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17 release

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    board.cpp \
    boardcondition.cpp \
    chessmatrix.cpp \
    constants.cpp \
    iconpack.cpp \
    main.cpp \
    mainwindow.cpp \
    move.cpp \
    piece.cpp \
    piecemaker.cpp \
    pieces/bishop.cpp \
    pieces/king.cpp \
    pieces/knight.cpp \
    pieces/pawn.cpp \
    pieces/queen.cpp \
    pieces/rock.cpp \
    square.cpp

HEADERS += \
    board.h \
    boardcondition.h \
    chesscases.h \
    chessmatrix.h \
    constants.h \
    coord.h \
    getid.h \
    iconpack.h \
    mainwindow.h \
    move.h \
    piece.h \
    piecemaker.h \
    pieces/bishop.h \
    pieces/king.h \
    pieces/knight.h \
    pieces/pawn.h \
    pieces/queen.h \
    pieces/rock.h \
    side.h \
    square.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

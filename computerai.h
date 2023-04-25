#ifndef COMPUTERAI_H
#define COMPUTERAI_H

/*

This is the ComputerAI class. It represents minimax algorithm and evaluation
function combined together to bothevaluate current position on the board and
make moves as the opposite player (a.k.a computer).

This class holds pointer to an object of the BoardCondition class (so ther is no
need to manually update position for ComputerAI). Then after each move of the
player the getMoves() method should be called. This will return QVector of
std::pair's of double (for evaluation of each move) and object of the Move class,
so QVector< std::pair<double, Move> >

The getMoves() (takes the ChessMatrix object as it isn't stored in the 
BoardCondition instance) method checks every possible move within the _depth 
(which can be changed by the player) based on array of legal moves; it 
makes evaluation for the resulting position after each move; the evaluation 
for position = the evalution for position after the best move in starting 
position, the evaluation for the last move (so for the _depth'th move) is 
just its evaluation.

The getMoves() function's execution is divided into five parts:
- first part is checking if the given position was already evaluated (if yes, 
    function's execution will be simplified so that it will make only the 
        necessary serach to complete the evaluation)
- the primary search if performed if the first part's check returned false:
    primary search involves usage of the alpha-beta pruning and move ordering 
        (transposition tables are used by the while class)
    during the primary search function evaluates position after every possible 
        move within the _depth moves (_depth is supposed to be approx. 3-4)
- the back-reevaluating happens after the primary search; during that stage
    the function makes evaluation of every position within the _depth-1 moves
        the evaluation of the position after its best move
    (so, if there is a position which has a chechmate in one move, this position 
        is going to get evaluation as if the checkmate has already happened)
- the secondary search involves usage of the quiescence search as well as 
    previous three techniques; firstly it orders branches by [?] how much 
        they change the starting position (so by how much the evaluation 
        changes) and then makes ordinary search through the top x% (x is 
        around 50 or 40); then starts the second part and its goal is to 
        determine which nodes of the move tree are unstable (positions 
        right after the captures, for example) and search through them 
        to find the stable outcomes
    therefore the secondary search will only go through the chains of captures 
        shouldn't take a lot of time
- the second back-reevaluation if the last step of the function, it does the 
    same thing, but goes through all the resulted tree (except for the nodes 
        that weren't updated after third stage)

The evaluation function (double evaluation()) also takes the board matrix as 
an argument and returns single double - the position's evaluation. It should also
add the position and its evaluation to the _evaluatedPositions array so that if 
the function goes into the same position it has evaluated before, the position 
won't be evaluated again.

The evaluation function should take into account:
- pieces on the board for every player
- in descending order: (obviously) turn, checkmate, captures, bounds, 
    (somehow) pieces's positioning, checks
- (somehow) possible cases after one move from the previous line

Algorithms to use:
- Alpha-Beta pruning
- Transposition tables
- Move ordering
- Quiescence search

*/

#include <QObject>
#include <QVector>
#include "boardcondition.h"
#include "chessmatrix.h"
#include "move.h"
#include "position.h"


class ComputerAI : public QObject
{
    Q_OBJECT // for possible in the future addition of the threads

public:
    explicit ComputerAI(BoardCondition *condition, QObject *parent = nullptr);

    double evaluation(ChessMatrix &matrix);
    QVector< std::pair<double, Move> > getMoves(ChessMatrix &matrix);

signals:

private:
    BoardCondition *_condition;
    QVector< std::pair<double, Position> > _evaluatedPositions;
};

#endif // COMPUTERAI_H

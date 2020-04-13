#include "FigurepPawn.h"
#include "../ChessBoard.h"
#include <assert.h>
#include <algorithm>

FigurePawn::FigurePawn(ChessTeam team, uint16_t row, uint16_t column) :
    FigureBase(team, row, column)
{
    mFigureName += "P";
}

bool FigurePawn::MoveTo(uint16_t row, uint16_t column)
{
    if (!mChessBoard)
    {
        assert(("No Chessboard!", false));
        return false;
    }

    MovesVector moves;
    moves.reserve(4);
    
    int16_t moveDirection = -1;
    if (mTeam == ChessTeam::Black)
    {
        moveDirection = 1;
    }

    const uint16_t boardSize = mChessBoard->GetBoardSize();
    uint16_t checkCellRow = mCurrentRow + moveDirection;
    uint16_t checkCellCol = mCurrentColumn;
    
    if (checkCellRow < boardSize)
    {
        if (!mChessBoard->GetCellFigure(checkCellRow, checkCellCol))
        {
            moves.emplace_back(checkCellRow, checkCellCol);
        }
    }

    if (mFirstMove)
    {
        checkCellRow = mCurrentRow + (2 * moveDirection);

        if (!mChessBoard->GetCellFigure(checkCellRow, checkCellCol))
        {
            moves.emplace_back(checkCellRow, checkCellCol);
        }
    }

    checkCellRow = mCurrentRow + moveDirection;
    checkCellCol = mCurrentColumn + moveDirection;
    if (checkCellRow < boardSize && checkCellCol < boardSize)
    {
        FigureBase* checkFigure = mChessBoard->GetCellFigure(checkCellRow, checkCellCol);
        if (checkFigure && checkFigure->GetTeam() != mTeam)
        {
            moves.emplace_back(checkCellRow, checkCellCol);
        }
    }

    checkCellCol = mCurrentColumn - moveDirection;
    if (checkCellRow < boardSize && checkCellCol < boardSize)
    {
        FigureBase* checkFigure = mChessBoard->GetCellFigure(checkCellRow, checkCellCol);
        if (checkFigure && checkFigure->GetTeam() != mTeam)
        {
            moves.emplace_back(checkCellRow, checkCellCol);
        }
    }
    
    bool isMoveValid = any_of(moves.begin(), moves.end(), [row, column](const pair<uint16_t, uint16_t>& move) {
        return move.first == row && move.second == column;
        });

    if (!isMoveValid)
    {
        return false;
    }

    MakeMove(row, column);

    if (mFirstMove)
    {
        mFirstMove = false;
    }
    

    return true;
}
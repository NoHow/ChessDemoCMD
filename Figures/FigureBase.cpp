#include "FigureBase.h"
#include <assert.h>
#include <algorithm>
#include "../ChessBoard.h"

ChessBoard* FigureBase::mChessBoard = nullptr;

FigureBase::FigureBase(ChessTeam team, uint16_t row, uint16_t column) :
    mCurrentRow(row),
    mCurrentColumn(column),
    mTeam(team)
{
    if (mTeam == ChessTeam::White)
    {
        mFigureName += "W";
    }
    else if (mTeam == ChessTeam::Black)
    {
        mFigureName += "B";
    }
    else
    {
        assert(("Not valid team", false));
    }
}

const string& FigureBase::GetFigureName() const
{
    return mFigureName;
}

void FigureBase::SetChessBoard(ChessBoard* board)
{
    mChessBoard = board;
}

ChessTeam FigureBase::GetTeam() const
{
    return mTeam;
}

bool FigureBase::MoveTo(uint16_t row, uint16_t column)
{
    return BaseMoveTo(row, column);
}

bool FigureBase::BaseMoveTo(uint16_t row, uint16_t column, uint16_t checkLimit)
{
    MovesVector moves;
    GetPossibleMovements(moves, mCheckDirections, checkLimit);

    bool isMoveValid = any_of(moves.begin(), moves.end(), [row, column](const pair<uint16_t, uint16_t>& move) {
        return move.first == row && move.second == column;
        });

    if (!isMoveValid)
    {
        return false;
    }

    MakeMove(row, column);

    if (mChessBoard->IsInCheck(mTeam))
    {
        mChessBoard->UndoLastMove();
        return false;
    }

    return true;
}

bool FigureBase::CanMoveTo(uint16_t row, uint16_t column)
{
    MovesVector moves;
    GetPossibleMovements(moves, mCheckDirections, mCheckLimit);

    bool isMoveValid = any_of(moves.begin(), moves.end(), [row, column](const pair<uint16_t, uint16_t>& move) {
        return move.first == row && move.second == column;
        });

    if (!isMoveValid)
    {
        return false;
    }

    return true;
}

bool FigureBase::CanMove()
{
    MovesVector moves;
    GetPossibleMovements(moves, mCheckDirections, mCheckLimit);

    for (const auto& move : moves)
    {
        MakeMove(move.first, move.second);

        if (!mChessBoard->IsInCheck(mTeam))
        {
            mChessBoard->UndoLastMove();
            return true;
        }
        mChessBoard->UndoLastMove();
    }

    return false;
}

void FigureBase::MakeMove(uint16_t row, uint16_t column)
{
    mChessBoard->MoveFigures(mCurrentRow, mCurrentColumn, row, column);

    mCurrentRow = row;
    mCurrentColumn = column;
}

void FigureBase::GetPossibleMovements(MovesVector& moves, DirectionsVector& checkDirections, uint16_t checkLimit)
{
    moves.reserve(32);
    const uint16_t boardSize = mChessBoard->GetBoardSize();

    for (const auto& direction : checkDirections)
    {
        uint16_t checkRow = static_cast<int16_t>(mCurrentRow) + direction.first;
        uint16_t checkColumn = static_cast<int16_t>(mCurrentColumn) + direction.second;

        uint16_t checkCounter = 0;
        while (checkRow < boardSize && checkColumn < boardSize && checkCounter++ < checkLimit)
        {
            FigureBase* cellFigure = mChessBoard->GetCellFigure(checkRow, checkColumn);
            if (cellFigure)
            {
                if (cellFigure->GetTeam() != mTeam)
                {
                    moves.push_back(make_pair(checkRow, checkColumn));
                }
                
                break;
            }

            moves.push_back(make_pair(checkRow, checkColumn));
            checkRow += direction.first;
            checkColumn += direction.second;
        }
    }
}

pair<uint16_t, uint16_t> FigureBase::GetLastPosition() const
{
    return make_pair(mCurrentRow, mCurrentColumn);
}

FigureType FigureBase::GetFigureType() const
{
    return FigureType::Invalid;
}

void FigureBase::SetLastPosition(uint16_t row, uint16_t column)
{
    mCurrentRow = row;
    mCurrentColumn = column;
}
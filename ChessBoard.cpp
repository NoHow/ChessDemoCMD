#include "ChessBoard.h"
#include <iostream>
#include <assert.h>
#include "Figures/FigureBishop.h"
#include "Figures/FigureKing.h"
#include "Figures/FigureKnight.h"
#include "Figures/FigurepPawn.h"
#include "Figures/FigureQueen.h"
#include "Figures/FigureRook.h"

using namespace std;

ChessBoard::ChessBoard() 
{
    mDrawBuffer.reserve(1024);

    mCells.reserve(mBoardSize);
    for (size_t index = 0; index < mBoardSize; ++index)
    {
        mCells.emplace_back(8);
    }

    FigureBase::SetChessBoard(this);

    FillBoardWithFigure(FigureType::Rook, 0, 0);
    FillBoardWithFigure(FigureType::Knight, 0, 1);
    FillBoardWithFigure(FigureType::Bishop, 0, 2);
    FillBoardWithFigure(FigureType::Queen, 0, 3, false);
    FillBoardWithFigure(FigureType::King, 0, 4, false);

    FillBoardWithFigure(FigureType::Pawn, 1, 0);
    FillBoardWithFigure(FigureType::Pawn, 1, 1);
    FillBoardWithFigure(FigureType::Pawn, 1, 2);
    FillBoardWithFigure(FigureType::Pawn, 1, 3);
}

void ChessBoard::DrawBoard()
{
    AddLine("****");
    mDrawBuffer += "\n";
    AddLine("****");
    mDrawBuffer += "\n";

    mDrawBuffer += " ";
    for (size_t row = 0; row < mBoardSize; ++row)
    {
        mDrawBuffer += "  ";
        mDrawBuffer += static_cast<char>(65) + static_cast<char>(row);
    }

    mDrawBuffer += "\n  ";
    for (size_t row = 0; row < mBoardSize; ++row)
    {
        AddLine(" --");
        mDrawBuffer += "\n" + to_string(row + 1u) + " ";

        for (size_t col = 0; col < mBoardSize; ++col)
        {
            FigureBase* figure = mCells[row][col].get();
            mDrawBuffer += "|";
            if (figure)
            {
                mDrawBuffer += figure->GetFigureName();
            }
            else
            {
                mDrawBuffer += "  ";
            }
        }
        mDrawBuffer += "|\n  ";
    }

    AddLine(" --");

    cout << mDrawBuffer;
    mDrawBuffer.clear();
}

uint16_t ChessBoard::GetBoardSize() const
{
    return mBoardSize;
}

void ChessBoard::MoveFigures(uint16_t currentRow, uint16_t currentColumn, uint16_t newRow, uint16_t newColumn)
{
    if (currentRow >= mBoardSize || currentColumn >= mBoardSize || newRow >= mBoardSize || newColumn >= mBoardSize)
    {
        assert(("Input in not valid", false));
        return;
    }

    mLastMovedFigure = nullptr;
    mLastKilledFigure.reset(nullptr);

    if (GetCellFigure(newRow, newColumn))
    {
        KillFigure(newRow, newColumn);
    }

    //Remember last move
    mLastMovedFigure = mCells[currentRow][currentColumn].get();
    mLastFigPos.first = currentRow;
    mLastFigPos.second = currentColumn;

    mCells[newRow][newColumn] = std::move(mCells[currentRow][currentColumn]);
}

void ChessBoard::UndoLastMove()
{
    if (!mLastMovedFigure)
    {
        return;
    }

    const auto& currentPosition = mLastKilledFigure->GetLastPosition();
    const uint16_t currentRow = currentPosition.first;
    const uint16_t currentColumn = currentPosition.second;

    mLastKilledFigure->SetLastPosition(mLastFigPos.first, mLastFigPos.second);

    mCells[mLastFigPos.first][mLastFigPos.second] = std::move(mCells[currentRow][currentColumn]);
    if (mLastKilledFigure)
    {
        pair<uint16_t, uint16_t> lastPosition = mLastKilledFigure->GetLastPosition();
        mCells[lastPosition.first][lastPosition.second] = std::move(mLastKilledFigure);
    }
}

void ChessBoard::KillFigure(uint16_t currentRow, uint16_t currentColumn)
{
    if (currentRow >= mBoardSize || currentColumn >= mBoardSize)
    {
        assert(("Input in not valid", false));
        return;
    }

    //Remember last kill
    mLastKilledFigure = std::move(mCells[currentRow][currentColumn]);
}

FigureBase* ChessBoard::GetCellFigure(uint16_t row, uint16_t column) const
{
    if (row >= mBoardSize || column >= mBoardSize)
    {
        assert(("Input in not valid", false));
        return nullptr;
    }

    return mCells[row][column].get();
}

void ChessBoard::AddLine(const string& pattern)
{
    for (size_t col = 0; col < mBoardSize; ++col)
    {
        mDrawBuffer += pattern;
    }
}

unique_ptr<FigureBase> ChessBoard::CreateFigure(FigureType type, ChessTeam team, uint16_t row, uint16_t column) const
{
    unique_ptr<FigureBase> freshFigure;

    switch (type)
    {
    case FigureType::Bishop:
        freshFigure = make_unique<FigureBishop>(team, row, column);
        break;
    case FigureType::King:
        freshFigure = make_unique<FigureKing>(team, row, column);
        break;
    case FigureType::Knight:
        freshFigure = make_unique<FigureKnight>(team, row, column);
        break;
    case FigureType::Pawn:
        freshFigure = make_unique<FigurePawn>(team, row, column);
        break;
    case FigureType::Queen:
        freshFigure = make_unique<FigureQueen>(team, row, column);
        break;
    case FigureType::Rook:
        freshFigure = make_unique<FigureRook>(team, row, column);
        break;
    default:
        break;
    }

    return freshFigure;
}

void ChessBoard::FillBoardWithFigure(FigureType type, uint16_t startingRow, uint16_t startingCol, bool doubleCopy)
{
    assert(("Not valid input", startingCol < mBoardSize && startingRow < mBoardSize));
    vector<pair<uint16_t, uint16_t>> spawnLocations;
    spawnLocations.reserve(4);

    uint16_t oppositeRow = mBoardSize - startingRow - 1u;
    uint16_t oppositeCol = mBoardSize - startingCol - 1u;
    spawnLocations.push_back(make_pair(startingRow, startingCol));
    spawnLocations.push_back(make_pair(oppositeRow, startingCol));

    if (doubleCopy)
    {
        spawnLocations.push_back(make_pair(startingRow, oppositeCol));
        spawnLocations.push_back(make_pair(oppositeRow, oppositeCol));
    }

    for (const auto& spawnLoc : spawnLocations)
    {
        if (spawnLoc.first <= 3u)
        {
            mCells[spawnLoc.first][spawnLoc.second] = CreateFigure(type, ChessTeam::Black, spawnLoc.first, spawnLoc.second);
        }
        else
        {
            mCells[spawnLoc.first][spawnLoc.second] = CreateFigure(type, ChessTeam::White, spawnLoc.first, spawnLoc.second);
        }
    }
}
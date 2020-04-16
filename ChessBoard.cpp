#include "ChessBoard.h"
#include <iostream>
#include <assert.h>
#include <fstream>
#include "Figures/FigureBase.h"
#include "Figures/FigureBishop.h"
#include "Figures/FigureKing.h"
#include "Figures/FigureKnight.h"
#include "Figures/FigurepPawn.h"
#include "Figures/FigureQueen.h"
#include "Figures/FigureRook.h"

namespace
{
    //Draw elements
    const string topBorder = "\xD1";
    const string botBorder = "\xCF";
    const string leftBorder = "\xC7";
    const string rightBorder = "\xB6";
    const string middleWall = "\xC5";
    const string horizWall = "\xC4";
    const string horizWall2 = "\xCD";
    const string vertWall = "\xB3";
    const string vertWall2 = "\xBA";
    const string topLCorner = "\xC9";
    const string topRCorner = "\xBB";
    const string botRCorner = "\xBC";
    const string botLCorner = "\xC8";
}//namespace

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

    InitFigures();
}

void ChessBoard::DrawBoard()
{
    DrawLine("*****");
    mDrawBuffer += "\n";
    DrawLine("*****");
    mDrawBuffer += "\n";

    //Draw Letters line
    mDrawBuffer += " ";
    for (size_t row = 0; row < mBoardSize; ++row)
    {
        mDrawBuffer += "  ";
        mDrawBuffer += static_cast<char>(65) + static_cast<char>(row);
    }

    mDrawBuffer += "\n  ";

    //Draw top border 
    DrawLine(topLCorner + horizWall2 + topBorder + horizWall2 + topRCorner);
    for (size_t row = 0; row < mBoardSize; ++row)
    {
        //Draw horizontal lines between cells;
        if (row != 0u && row != mBoardSize)
        {
            DrawLine(leftBorder + horizWall + middleWall + horizWall + rightBorder);
        }

        //Draw numeration from left
        mDrawBuffer += "\n" + to_string(row + 1u) + " " + vertWall2;

        //Draw cell lines
        for (size_t col = 0; col < mBoardSize; ++col)
        {
            FigureBase* figure = mCells[row][col].get();
            if (figure)
            {
                mDrawBuffer += figure->GetFigureName();
            }
            else
            {
                mDrawBuffer += "  ";
            }
            mDrawBuffer += vertWall;
        }
        mDrawBuffer.replace(mDrawBuffer.end() - 1u, mDrawBuffer.end(), vertWall2);

        //Draw numeration from right
        mDrawBuffer += to_string(row + 1u) + "\n  ";
    }
    //Draw bottom border
    DrawLine(botLCorner + horizWall2 + botBorder + horizWall2 + botRCorner);

    //Draw Letters line
    mDrawBuffer += "\n ";
    for (size_t row = 0; row < mBoardSize; ++row)
    {
        mDrawBuffer += "  ";
        mDrawBuffer += static_cast<char>(65) + static_cast<char>(row);
    }

    cout << mDrawBuffer;
    mDrawBuffer.clear();
}

bool ChessBoard::SaveBoard(const ChessTeam& currentPlayer)
{
    const size_t bufferSize = 256;
    char* buffer = new char[bufferSize];

    size_t currentBufPos = 0;

    buffer[currentBufPos++] = static_cast<char>(currentPlayer);
    for (const auto& row : mCells)
    {
        for (const auto& figure : row)
        {
            FigureType type = FigureType::NoFigure;
            ChessTeam team = ChessTeam::Invalid;
            if (figure)
            {
                type = figure->GetFigureType();
                team = figure->GetTeam();
            }
            
            buffer[currentBufPos++] = static_cast<char>(type);
            buffer[currentBufPos++] = static_cast<char>(team);
        }
    }

    ofstream file("save.bin", ios::binary);

    bool success = false;
    if (file.is_open())
    {
        file.write(buffer, currentBufPos);
        
        file.close();
        success = true;
    }

    delete[] buffer;
    return success;
}

bool ChessBoard::LoadBoard(ChessTeam& currentPlayer)
{
    const size_t bufferSize = 256;
    char* buffer = new char[bufferSize];

    ifstream file("save.bin", ios::binary|ios::ate);

    bool success = false;
    if (file.is_open())
    {
        auto fileSize = file.tellg();
        if (fileSize > 0)
        {
            file.seekg(0, ios::beg);

            file.read(buffer, fileSize);
            file.close();
            success = true;
        }
    }

    if (success)
    {
        size_t bufferPos = 0;
        currentPlayer = static_cast<ChessTeam>(buffer[bufferPos++]);

        for (uint16_t row = 0; row < mBoardSize; ++row)
        {
            for (uint16_t col = 0; col < mBoardSize; ++col)
            {
                FigureType type = static_cast<FigureType>(buffer[bufferPos++]);

                if (type != FigureType::NoFigure)
                {
                    ChessTeam team = static_cast<ChessTeam>(buffer[bufferPos]);
                    mCells[row][col] = CreateFigure(type, team, row, col);
                }
                else
                {
                    mCells[row][col].reset(nullptr);
                }
                ++buffer;
            }
        }
    }

    return success;
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

    const auto currentPosition = mLastMovedFigure->GetLastPosition();
    const uint16_t currentRow = currentPosition.first;
    const uint16_t currentColumn = currentPosition.second;

    mLastMovedFigure->SetLastPosition(mLastFigPos.first, mLastFigPos.second);

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

void ChessBoard::PromotePawn(uint16_t row, uint16_t column)
{
    FigureType type = FigureType::Invalid;

    string userInput;
    while (type == FigureType::Invalid)
    {
        cout << "Please select new figure for pawn promotion(R,B,N or Q): ";
        cin >> userInput;

        if (userInput == "Q")
        {
            type = FigureType::Queen;
        }
        else if (userInput == "B")
        {
            type = FigureType::Bishop;
        }
        else if (userInput == "B")
        {
            type = FigureType::Knight;
        }
        else if (userInput == "R")
        {
            type = FigureType::Rook;
        }
        else
        {
            cout << "Couldn't undestand what you mean...";
        }

        userInput.clear();
    }

    FigureBase* promotionPawn = GetCellFigure(row, column);
    unique_ptr<FigureBase> newFigure = CreateFigure(type, promotionPawn->GetTeam(), row, column);

    mCells[row][column] = std::move(newFigure);
}

void ChessBoard::DrawLine(const string& pattern)
{
    if (pattern.size() != 5u)
    {
        assert(("Input is not valid!", false));
        return;
    }

    const string& firstPart = pattern.substr(0, 2);
    const string& middlePart = pattern.substr(1, 3);
    const string& endingPart = pattern.substr(3, 2);
    
    mDrawBuffer += firstPart;
    
    for (size_t col = 0; col < (mBoardSize - 1u); ++col)
    {
        mDrawBuffer += middlePart;
    }

    mDrawBuffer += endingPart;
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

bool ChessBoard::IsInCheck(ChessTeam team)
{
    vector<FigureBase*> figures;
    figures.reserve(16);

    uint16_t kingRow = 0u;
    uint16_t kingCol = 0u;
    for (const auto& row : mCells)
    {
        for (const auto& figure : row)
        {
            if (!figure)
            {
                continue;
            }

            if (figure->GetTeam() == team)
            {
                if (figure->GetFigureType() == FigureType::King)
                {
                    const auto location = figure->GetLastPosition();
                    kingRow = location.first;
                    kingCol = location.second;
                }
            }
            else
            {
                figures.push_back(figure.get());
            }
        }
    }

    for (auto* figure : figures)
    {
        if (figure->CanMoveTo(kingRow, kingCol))
        {
            return true;
        }
    }

    return false;
}

GameStatus ChessBoard::GetGameStatus(ChessTeam team)
{
    bool isInCheck = IsInCheck(team);

    vector<FigureBase*> figures;
    figures.reserve(16);

    GetTeamFigures(team, figures);

    for (auto* figure : figures)
    {
        if (figure->CanMove())
        {
            return GameStatus::InGame;
        }
    }

    GameStatus result = GameStatus::CheckMate;
    if (!isInCheck)
    {
        result = GameStatus::StaleMate;
    }

    return result;
}

void ChessBoard::GetTeamFigures(ChessTeam team, vector<FigureBase*>& figures) const
{
    for (const auto& row : mCells)
    {
        for (const auto& figure : row)
        {
            if (!figure)
            {
                continue;
            }

            if (figure->GetTeam() == team)
            {
                figures.push_back(figure.get());
            }
        }
    }
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

void ChessBoard::ResetBoard()
{
    for (auto& row : mCells)
    {
        for (auto& figure : row)
        {
            if (figure)
            {
                figure.reset(nullptr);
            }
        }
    }

    InitFigures();
}

void ChessBoard::InitFigures()
{
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
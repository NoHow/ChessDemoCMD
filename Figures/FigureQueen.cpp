#include "FigureQueen.h"

FigureQueen::FigureQueen(ChessTeam team, uint16_t row, uint16_t column) :
    FigureBase(team, row, column)
{
    mFigureName += "Q";

    mCheckDirections.reserve(8);
    mCheckDirections.emplace_back(1, 0);
    mCheckDirections.emplace_back(0, 1);
    mCheckDirections.emplace_back(-1, 0);
    mCheckDirections.emplace_back(0, -1);
    mCheckDirections.emplace_back(1, 1);
    mCheckDirections.emplace_back(-1, -1);
    mCheckDirections.emplace_back(-1, 1);
    mCheckDirections.emplace_back(1, -1);
}

FigureType FigureQueen::GetFigureType() const
{
    return FigureType::Queen;
}
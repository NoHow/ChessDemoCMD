#include "FigureKing.h"

FigureKing::FigureKing(ChessTeam team, uint16_t row, uint16_t column) :
    FigureBase(team, row, column)
{
    mFigureName += "K";

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

bool FigureKing::MoveTo(uint16_t row, uint16_t column)
{
    return BaseMoveTo(row, column, 1);
}
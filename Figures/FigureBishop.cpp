#include "FigureBishop.h"

FigureBishop::FigureBishop(ChessTeam team, uint16_t row, uint16_t column) :
    FigureBase(team, row, column)
{
    mFigureName += "B";

    mCheckDirections.reserve(4);
    mCheckDirections.emplace_back(1, 1);
    mCheckDirections.emplace_back(-1, -1);
    mCheckDirections.emplace_back(-1, 1);
    mCheckDirections.emplace_back(1, -1);
}
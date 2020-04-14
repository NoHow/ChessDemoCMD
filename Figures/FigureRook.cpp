#include "FigureRook.h"

FigureRook::FigureRook(ChessTeam team, uint16_t row, uint16_t column) :
    FigureBase(team, row, column)
{
    mFigureName += "R";

    mCheckDirections.reserve(4);
    mCheckDirections.emplace_back(1, 0);
    mCheckDirections.emplace_back(0, 1);
    mCheckDirections.emplace_back(-1, 0);
    mCheckDirections.emplace_back(0, -1);
}

FigureType FigureRook::GetFigureType() const
{
    return FigureType::Rook;
}
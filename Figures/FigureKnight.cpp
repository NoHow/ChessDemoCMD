#include "FigureKnight.h"

FigureKnight::FigureKnight(ChessTeam team, uint16_t row, uint16_t column) :
    FigureBase(team, row, column)
{
    mFigureName += "N";

    mCheckDirections.reserve(8);
	mCheckDirections.emplace_back(2, 1);
	mCheckDirections.emplace_back(1, 2);
	mCheckDirections.emplace_back(-2, 1);
	mCheckDirections.emplace_back(-1, 2);
	mCheckDirections.emplace_back(2, -1);
	mCheckDirections.emplace_back(1, -2);
	mCheckDirections.emplace_back(-2, -1);
	mCheckDirections.emplace_back(-1, -2);

	mCheckLimit = 1u;
}

bool FigureKnight::MoveTo(uint16_t row, uint16_t column)
{
    return BaseMoveTo(row, column, mCheckLimit);
}

FigureType FigureKnight::GetFigureType() const
{
	return FigureType::Knight;
}
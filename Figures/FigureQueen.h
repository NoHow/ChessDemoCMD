#pragma once
#include "FigureBase.h"

using namespace std;

class FigureQueen : public FigureBase
{
public:
    FigureQueen(ChessTeam team, uint16_t row, uint16_t column);
    FigureType GetFigureType() const override;
};

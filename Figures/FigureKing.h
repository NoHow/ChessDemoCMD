#pragma once
#include "FigureBase.h"

using namespace std;

class FigureKing : public FigureBase
{
public:
    FigureKing(ChessTeam team, uint16_t row, uint16_t column);
    bool MoveTo(uint16_t row, uint16_t column) override;
    FigureType GetFigureType() const override;
};

#pragma once
#include "FigureBase.h"

using namespace std;

class FigureKnight : public FigureBase
{
public:
    FigureKnight(ChessTeam team, uint16_t row, uint16_t column);
    bool MoveTo(uint16_t row, uint16_t column) override;
};

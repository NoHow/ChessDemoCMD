#pragma once
#include "FigureBase.h"

using namespace std;

class FigureBishop : public FigureBase
{
public:
    FigureBishop(ChessTeam team, uint16_t row, uint16_t column);
};

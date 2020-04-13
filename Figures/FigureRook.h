#pragma once
#include "FigureBase.h"

using namespace std;

class FigureRook : public FigureBase
{
public:
    FigureRook(ChessTeam team, uint16_t row, uint16_t column);
};

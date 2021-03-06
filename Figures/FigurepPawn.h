#pragma once
#include "FigureBase.h"

using namespace std;

class FigurePawn : public FigureBase
{
public:
    FigurePawn(ChessTeam team, uint16_t row, uint16_t column);
    bool MoveTo(uint16_t row, uint16_t column) override;

    FigureType GetFigureType() const override;
    void GetPossibleMovements(MovesVector& moves, DirectionsVector& checkDirections, uint16_t checkLimit = UINT16_MAX) override;
private:
    bool mFirstMove = true;
};

#pragma once
#include <vector>
#include <utility>
#include <string>

using namespace std;
using MovesVector = vector<pair<uint16_t, uint16_t>>;
using DirectionsVector = vector<pair<int16_t, int16_t>>;

enum class ChessTeam
{
    Invalid,
    Black,
    White
};

enum class FigureType
{
    Invalid,
    NoFigure,
    Bishop,
    King,
    Knight,
    Pawn,
    Queen,
    Rook
};

class ChessBoard;

class FigureBase
{
public:
    virtual bool MoveTo(uint16_t row, uint16_t column);
    virtual FigureType GetFigureType() const;

    bool CanMoveTo(uint16_t row, uint16_t column);
    bool CanMove();
    const string& GetFigureName() const;
    ChessTeam GetTeam() const;

    pair<uint16_t, uint16_t> GetLastPosition() const;
    void SetLastPosition(uint16_t row, uint16_t column);

    static void SetChessBoard(ChessBoard* board);
protected:
    FigureBase(ChessTeam team, uint16_t row, uint16_t column);

    bool BaseMoveTo(uint16_t row, uint16_t column, uint16_t checkLimit = UINT16_MAX);
    void MakeMove(uint16_t row, uint16_t column);
    virtual void GetPossibleMovements(MovesVector& moves, DirectionsVector& checkDirections, uint16_t checkLimit = UINT16_MAX);

protected:
    static ChessBoard* mChessBoard;

    ChessTeam mTeam = ChessTeam::Invalid;
    string mFigureName{};

    //Directions of movement to check - where figure can move
    DirectionsVector mCheckDirections;
    uint16_t mCheckLimit = UINT16_MAX;

    //Current position of figure
    uint16_t mCurrentRow = 0;
    uint16_t mCurrentColumn = 0;
};


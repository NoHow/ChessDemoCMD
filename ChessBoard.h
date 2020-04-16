#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Figures/FigureBase.h"


using namespace std;
using UniquePtrFigureVector = vector<unique_ptr<FigureBase>>;

enum class GameStatus
{
    InGame,
    CheckMate,
    StaleMate
};

class FigureBase;

class ChessBoard
{
public:
    ChessBoard();

    void DrawBoard();
    bool SaveBoard(const ChessTeam& currentPlayer);
    bool LoadBoard(ChessTeam& currentPlayer);
    void ResetBoard();

    uint16_t GetBoardSize() const;
    FigureBase* GetCellFigure(uint16_t row, uint16_t column) const;
    void MoveFigures(uint16_t currentRow, uint16_t currentColumn, uint16_t newRow, uint16_t newColumn);
    void UndoLastMove();
    void KillFigure(uint16_t currentRow, uint16_t currentColumn);

    void PromotePawn(uint16_t row, uint16_t column); 

    GameStatus GetGameStatus(ChessTeam team);
    bool IsInCheck(ChessTeam team);

private:
    void DrawLine(const string& pattern);
    void FillBoardWithFigure(FigureType type, uint16_t startingRow, uint16_t startingCol, bool doubleCope = true);
    void GetTeamFigures(ChessTeam team, vector<FigureBase*>& figures) const;
    void InitFigures();
    
    unique_ptr<FigureBase> CreateFigure(FigureType type, ChessTeam team, uint16_t row, uint16_t column) const;
private: 
    vector<UniquePtrFigureVector> mCells;

    uint16_t mBoardSize = 8;
    string mDrawBuffer;
    string mFigureName;

    FigureBase* mLastMovedFigure;
    unique_ptr<FigureBase> mLastKilledFigure;
    pair<uint16_t, uint16_t> mLastFigPos;
};
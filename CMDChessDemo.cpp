#include <iostream>
#include <string>
#include <array>
#include "ChessBoard.h"
#include "Figures/FigureBase.h"

using namespace std;

namespace
{
    const string SELECT_FIGURE_TEXT = "\nPlease select figure.. \n";
    const string SELECT_ROW_TEXT = "Row: ";
    const string SELECT_COLUMN_TEXT = "Column: ";
    const string SELECT_MOVE_TEXT = "Please select where you want to move selected figure!\n";

    const string NOT_VALID_MOVE_TEXT = "Can't move this figure there! \n";

    const array<string, 8> VALID_COLUMNS = { "A", "B", "C", "D", "E", "F", "G", "H" };
}//namespace

pair<uint16_t, uint16_t> SelectCell(string& userInput);

int main()
{
    ChessBoard chessBoard;

    ChessTeam mCurrentPlayer = ChessTeam::White;

    string userInput;
    userInput.reserve(32);
    while (true)
    {
        chessBoard.DrawBoard();
        
        cout << SELECT_FIGURE_TEXT;
        pair<uint16_t, uint16_t> figurePos = SelectCell(userInput);
        FigureBase* figure = chessBoard.GetCellFigure(figurePos.first, figurePos.second);
        if (figure && figure->GetTeam() == mCurrentPlayer)
        {
            bool isMoveValid = false;
            while (!isMoveValid)
            {
                cout << SELECT_MOVE_TEXT;
                pair<uint16_t, uint16_t> movePos = SelectCell(userInput);

                isMoveValid = figure->MoveTo(movePos.first, movePos.second);
                if (!isMoveValid)
                {
                    cout << NOT_VALID_MOVE_TEXT;
                }
            }

            if (mCurrentPlayer == ChessTeam::White)
            {
                mCurrentPlayer = ChessTeam::Black;
            }
            else if (mCurrentPlayer == ChessTeam::Black)
            {
                mCurrentPlayer = ChessTeam::White;
            }
        }
    }
}

pair<uint16_t, uint16_t> SelectCell(string& userInput)
{
    pair<uint16_t, uint16_t> result{};

    bool isValidInput = false;
    while (!isValidInput)
    {
        cout << SELECT_ROW_TEXT;
        cin >> userInput;

        if (!isdigit(userInput[0]))
        {
            userInput.clear();
            continue;
        }

        const int16_t row = stoi(userInput);

        const int16_t boardSize = 8;
        if (row > 0 && row <= boardSize)
        {
            result.first = row - 1;
            isValidInput = true;
        }
        userInput.clear();
    }

    isValidInput = false;
    while (!isValidInput)
    {
        cout << SELECT_COLUMN_TEXT;
        cin >> userInput;

        isValidInput = any_of(VALID_COLUMNS.begin(), VALID_COLUMNS.end(), [userInput](const string& letter)
            {
                return userInput == letter;
            });

        if (isValidInput)
        {
            const uint16_t ASCI_OFFSET = 65;
            result.second = static_cast<uint16_t>(*userInput.c_str()) - ASCI_OFFSET;
        }
        userInput.clear();
    }

    return result;
}
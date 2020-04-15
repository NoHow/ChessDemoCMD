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
    const string SELECT_USER_STATE_TEXT = "\nWhat are we going to do next? (Move, Restart, Save, Load, Exit): ";

    const string SUCCESS_GAME_SAVE_TEXT = "Game is saved!\n";
    const string FAIL_GAME_SAVE_TEXT = "Error: couldnt't save game for some reason :(";

    const string NOT_VALID_MOVE_TEXT = "Can't move this figure there! \n";

    const array<string, 8> VALID_COLUMNS = { "A", "B", "C", "D", "E", "F", "G", "H" };
}//namespace

enum class UserState
{
    Invalid,
    Restart,
    SaveGame,
    LoadGame,
    Move,
    Exit
};

pair<uint16_t, uint16_t> SelectCell(string& buffer);
UserState GetUserState(string& buffer);

int main()
{
    ChessBoard chessBoard;

    UserState userState = UserState::Invalid;
    ChessTeam currentPlayer = ChessTeam::White;
    ChessTeam winner = ChessTeam::Invalid;
    GameStatus gameStatus = GameStatus::InGame;

    string userInputBuffer;
    userInputBuffer.reserve(32);

    while (userState != UserState::Exit)
    {
        chessBoard.DrawBoard();
        userState = GetUserState(userInputBuffer);

        switch (userState)
        {
        case UserState::Restart:
        {
            chessBoard.ResetBoard();
            currentPlayer = ChessTeam::White;
            break;
        }
        case UserState::SaveGame:
        {
            if (chessBoard.SaveBoard(currentPlayer))
            {
                cout << SUCCESS_GAME_SAVE_TEXT;
            }
            else
            {
                cout << FAIL_GAME_SAVE_TEXT;
            }
            break;
        }
        case UserState::LoadGame:
        {
            chessBoard.LoadBoard(currentPlayer);
            break;
        }
        case UserState::Move:
        {
            FigureBase* figure = nullptr;
            while (!figure)
            {
                cout << SELECT_FIGURE_TEXT;
                pair<uint16_t, uint16_t> figurePos = SelectCell(userInputBuffer);
                figure = chessBoard.GetCellFigure(figurePos.first, figurePos.second);

                if (figure)
                {
                    if (figure->GetTeam() == currentPlayer && figure->CanMove())
                    {
                        continue;
                    }
                    else
                    {
                        figure = nullptr;
                    }
                }
            }

            bool isMoveValid = false;
            while (!isMoveValid)
            {
                cout << SELECT_MOVE_TEXT;
                pair<uint16_t, uint16_t> movePos = SelectCell(userInputBuffer);

                isMoveValid = figure->MoveTo(movePos.first, movePos.second);
                if (!isMoveValid)
                {
                    cout << NOT_VALID_MOVE_TEXT;
                }
            }

            if (currentPlayer == ChessTeam::White)
            {
                currentPlayer = ChessTeam::Black;
            }
            else if (currentPlayer == ChessTeam::Black)
            {
                currentPlayer = ChessTeam::White;
            }

            gameStatus = chessBoard.GetGameStatus(currentPlayer);
            if (gameStatus == GameStatus::CheckMate)
            {
                if (currentPlayer == ChessTeam::White)
                {
                    winner = ChessTeam::Black;
                }
                else if (currentPlayer == ChessTeam::Black)
                {
                    winner = ChessTeam::White;
                }

                userState = UserState::Exit;
            }
            else if (gameStatus == GameStatus::StaleMate)
            {
                winner = ChessTeam::Invalid;
                userState = UserState::Exit;
            }

            break;
        }
        default:
            break;
        }
    }
    
    cout << "Game is over!\n ";

    if (gameStatus != GameStatus::InGame)
    {
        if (winner == ChessTeam::Black)
        {
            cout << "Winner is: Black player!";
        }
        else if (winner == ChessTeam::White)
        {
            cout << "Winner is: White player!";
        }
        else
        {
            cout << "Result is Draw!";
        }
    }

    cout << "\n\n\n";
}

UserState GetUserState(string& buffer)
{
    UserState result = UserState::Invalid;

    while (result == UserState::Invalid)
    {
        cout << SELECT_USER_STATE_TEXT;
        cin >> buffer;

        if (buffer == "Move")
        {
            result = UserState::Move;
        }
        else if (buffer == "Restart")
        {
            result = UserState::Restart;
        }
        else if (buffer == "Save")
        {
            result = UserState::SaveGame;
        }
        else if (buffer == "Load")
        {
            result = UserState::LoadGame;
        }
        else if (buffer == "Exit")
        {
            result = UserState::Exit;
        }

        buffer.clear();
    }

    return result;
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


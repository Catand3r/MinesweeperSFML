#pragma once
#include "GameState.h"
#include "RandomEngine.h"
#include "UserInput.h"
#include <array>
#include <iostream>
#include <utility>
#include <vector>

enum class Result
{
    empty = 0,
    mine
};

class Minesweeper
{
    friend RunningFirstCheck;
    friend Running;
    friend Lost;
    friend Won;

  public:
    Minesweeper(IUserInput *userInput, IRandomEngine *randomEngine, int x, int y);
    bool Run();

  private:
    GameStateManager gStateManager_;

    IUserInput *userInput_;
    IRandomEngine *randomEngine_;

    Cells cells_;

    void CreateEmptyBoard(int x, int y);
    void PlaceMines(std::vector<std::pair<int, int>>);

    [[nodiscard]] bool ExecuteCheckCell(int, int);
    bool ExecuteMarkCell(int, int);

    int boardSizeX_ = 0;
    int boardSizeY_ = 0;
    float mineAmountIndex_ = 0.1f;
    int mineAmount_;
    int flagAmount_;

    bool IsPositionInRange(int, int);
    int CheckAroundCell(int, int);
    void UncoverCellsAroundCell(int, int);
    bool IsGameWon();
    void UncoverAllMines();
};

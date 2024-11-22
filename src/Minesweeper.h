#pragma once
#include "RandomEngine.h"
#include "UserInput.h"
#include <array>
#include <iostream>
#include <utility>
#include <vector>

enum class GameState
{
    gameRunningFirstCheck = 0,
    gameRunning,
    gameLost,
    gameWon
};

enum class Result
{
    empty = 0,
    mine
};

class Minesweeper
{
  public:
    Minesweeper(IUserInput *userInput, IRandomEngine *randomEngine, int x, int y);
    bool Run();

  private:
    GameState gState_ = GameState::gameRunningFirstCheck;

    IUserInput *userInput_;
    IRandomEngine *randomEngine_;

    Cells cells_;

    void RunGameState();

    void CreateEmptyBoard(int x, int y);
    void PlaceMines();

    [[nodiscard]] bool ExecuteCheckCell(int, int);
    [[nodiscard]] bool ExecuteMarkCell(int, int);

    int boardSizeX_ = 0;
    int boardSizeY_ = 0;
    float mineAmountIndex_ = 0.1f;
    int mineAmount_;
    int flagAmount_;
    bool firstCheck_ = true;

    bool IsPositionInRange(int, int);
    int CheckAroundCell(int, int);
    void UncoverCellsAroundCell(int, int);
    void CreateEmptyMinePositions();
    bool IsGameWon();
    void UncoverAllMines();

    MinePositions minePostitions_;
};

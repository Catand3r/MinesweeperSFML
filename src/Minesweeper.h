#pragma once
#include "UserInput.h"
#include "RandomEngine.h"
#include <iostream>
#include <utility>
#include <vector>
#include <array>
#include <vector>


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
    IUserInput *userInput_;
    IRandomEngine *randomEngine_;

    Cells cells_;
    
    void CreateEmptyBoard(int x, int y);
    void PlaceMines();
    
    [[nodiscard]] bool ExecuteCheckCell(int, int);
    [[nodiscard]] bool ExecuteMarkCell(int, int);
    
    float mineAmountIndex_ = 0.1f;
    int mineAmount_;
    int flagAmount_;
    bool firstCheck_ = true;

    bool IsPositionInRange(int, int);
    int CheckAroundCell(int, int);
    void UncoverCellsAroundCell(int, int);
    void CreateEmptyMinePositions();
    bool IsGameWon();
    
    MinePositions minePostitions_;
};

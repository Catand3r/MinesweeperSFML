#pragma once
#include "UserInput.h"
#include "RandomEngine.h"
#include <iostream>
#include <utility>
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
    Minesweeper(IUserInput *userInput, IRandomEngine *randomEngine);
    bool Run();

private:
    IUserInput *userInput_;
    IRandomEngine *randomEngine_;

    Cells cells_;
    
    void CreateEmptyBoard();
    void PlaceMines();
    
    [[nodiscard]] bool ExecuteCheckCell(int, int);
    [[nodiscard]] bool ExecuteMarkCell(int, int);
    
    int flagAmount_;
    int mineAmount;
    bool firstCheck_ = true;

    int CheckAroundCell(int, int);
    void CheckCellsAroundCell(int, int);
    void CreateEmptyMinePositions();
    
    MinePositions minePostitions_;
};

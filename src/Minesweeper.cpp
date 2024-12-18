#include "Minesweeper.h"
#include <array>
#include <exception>
#include <iostream>
#include <utility>
#include <vector>

namespace
{
std::array<Position, 8> GetAroundCellPosition(int x, int y)
{
    std::array<Position, 8> positions = {
        Position{x, y + 1},     Position{x, y - 1},     Position{x + 1, y},     Position{x - 1, y},
        Position{x + 1, y + 1}, Position{x + 1, y - 1}, Position{x - 1, y + 1}, Position{x - 1, y - 1},
    };
    return positions;
}

} // namespace

Minesweeper::Minesweeper(IUserInput *userInput, IRandomEngine *randomEngine, int x, int y)
    : userInput_(userInput), randomEngine_(randomEngine), flagAmount_(mineAmount_), boardSizeX_(x), boardSizeY_(y),
      mineAmount_(x * y * mineAmountIndex_)
{
    CreateEmptyBoard(boardSizeX_, boardSizeY_);
}

void Minesweeper::CreateEmptyBoard(int x, int y)
{
    cells_.resize(x);
    for (int i = 0; i < cells_.size(); i++)
    {
        cells_[i].resize(y);
        for (int j = 0; j < cells_[i].size(); j++)
        {
            cells_[i][j].state = CellState::empty;
            cells_[i][j].SetCovered();
        }
    }
}

bool Minesweeper::Run()
{
    bool result = true;
    Action action = userInput_->PollEvent();
    gStateManager_.RunCurrentState(*this, action);
    if (action.actionType_ == ActionType::Close)
    {
        return false;
    }
    else if (action.actionType_ == ActionType::None)
    {
        userInput_->Delay(100);
        return true;
    }
    return true;
}

bool Minesweeper::IsGameWon()
{
    for (int i = 0; i < cells_.size(); i++)
    {
        for (int j = 0; j < cells_[i].size(); j++)
        {
            if ((cells_[i][j].state == CellState::mine && !cells_[i][j].marked) ||
                (cells_[i][j].state == CellState::empty && !cells_[i][j].uncovered))
            {
                return false;
            }
        }
    }
    return true;
}

void Minesweeper::UncoverAllMines()
{
    for (int i = 0; i < cells_.size(); i++)
    {
        for (int j = 0; j < cells_[i].size(); j++)
        {
            if (cells_[i][j].state == CellState::mine && !cells_[i][j].uncovered)
                cells_[i][j].SetUncovered();
        }
    }
}

bool Minesweeper::ExecuteCheckCell(int x, int y)
{
    if (cells_[x][y].state == CellState::mine)
    {
        userInput_->OnResultMine(x, y);
        cells_[x][y].SetUncovered();
        return false;
    }
    else if (cells_[x][y].state != CellState::mine)
    {
        if (cells_[x][y].marked)
            return ExecuteMarkCell(x, y);
        cells_[x][y].SetUncovered();
        int checkAroundCellAmount = CheckAroundCell(x, y);
        userInput_->OnResultEmpty(x, y, checkAroundCellAmount);
        if (checkAroundCellAmount == 0)
        {
            UncoverCellsAroundCell(x, y);
        }
        return true;
    }
    return true;
}

bool Minesweeper::ExecuteMarkCell(int x, int y)
{
    if (!cells_[x][y].marked && !cells_[x][y].uncovered && flagAmount_ > 0)
    {
        flagAmount_--;
        cells_[x][y].SetMarked();
        userInput_->OnMarkCell(x, y, flagAmount_);
    }
    else if (cells_[x][y].marked)
    {
        flagAmount_++;
        cells_[x][y].SetCovered();
        userInput_->OnMarkCell(x, y, flagAmount_);
    }
    return true;
}

void Minesweeper::PlaceMines(std::vector<std::pair<int, int>> minePositions)
{
    for (const auto &[x, y] : minePositions)
    {
        cells_[x][y].state = CellState::mine;
        cells_[x][y].SetCovered();
    }
}

int Minesweeper::CheckAroundCell(int x, int y)
{
    int mineAroundAmount = 0;
    std::array<Position, 8> positions = GetAroundCellPosition(x, y);

    for (const auto &[newX, newY] : positions)
    {
        if (IsPositionInRange(newX, newY) && cells_[newX][newY].state == CellState::mine)
        {
            mineAroundAmount++;
        }
    }
    return mineAroundAmount;
}

void Minesweeper::UncoverCellsAroundCell(int x, int y)
{
    std::array<Position, 8> positions = GetAroundCellPosition(x, y);
    for (const auto &[newX, newY] : positions)
    {
        if (IsPositionInRange(newX, newY) && cells_[newX][newY].state != CellState::mine &&
            !cells_[newX][newY].uncovered && !cells_[newX][newY].marked)
        {
            cells_[newX][newY].SetUncovered();
            int cellAroundAmount = CheckAroundCell(newX, newY);
            userInput_->OnResultEmpty(newX, newY, cellAroundAmount);
            if (cellAroundAmount == 0)
                UncoverCellsAroundCell(newX, newY);
        }
    }
}

bool Minesweeper::IsPositionInRange(int x, int y)
{
    return x >= 0 && x < cells_.size() && y >= 0 && y < cells_[0].size();
}

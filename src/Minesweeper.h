#pragma once
#include <iostream>
#include <utility>
#include <array>
#include <vector>

enum class CellState
{
    empty = 0,
    mine
};

enum class Result
{
    empty = 0,
    mine
};

using Cells = std::array<std::array<CellState, 8>, 10>;

using Position = std::pair<int, int>;
using MinePositions = std::vector<Position>;
class IUserInput
{
public:
    virtual ~IUserInput() = default;

    virtual Position GetPos() = 0;

    virtual void OnResultEmpty(int, int, int) = 0;
    virtual void OnResultMine(int, int) = 0;
};

class KeyboardUserInput : public IUserInput
{
public:
    ~KeyboardUserInput() override = default;

    Position GetPos() override;

    void OnResultEmpty(int, int, int) override;
    void OnResultMine(int, int) override;
};

class IRandomEngine
{
public:
    virtual ~IRandomEngine() = default;

    virtual MinePositions RandomizeMinePlacement(Cells, int) = 0;
};

class RandomEngine : public IRandomEngine
{
    ~RandomEngine() override = default;

    MinePositions RandomizeMinePlacement(Cells, int) override;
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
    void Draw();
    void CreateEmptyBoard();
    void PlaceMines();
    int flagAmount;
    int mineAmount;
    int CheckAroundCell(int, int);
    MinePositions minePostitions_;
};

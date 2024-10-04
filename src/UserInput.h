#pragma once
#include <utility>
#include <array>
#include <vector>



enum class ActionType
{
    None = 0,
    CheckCell,
    MarkCell,
    Close
};

enum class CellState
{
    empty = 0,
    mine,
    uncovered,
    marked
};

using Position = std::pair<int, int>;
using Cells = std::array<std::array<CellState, 8>, 10>;
using MinePositions = std::vector<Position>;

struct Action
{
public:
    ActionType actionType_;
    Position playerPos_;
};


class IUserInput
{
public:

    virtual ~IUserInput() = default;

    virtual Action MakeAction() = 0;

    virtual bool Init(const Cells &, const MinePositions &) = 0;

    virtual void OnResultEmpty(int, int, int) = 0;
    virtual void OnResultMine(int, int) = 0;
    virtual void OnMarkCell(int, int) = 0;

    virtual bool PollEvent() { return true; }

    virtual void Delay(int) const { };

};

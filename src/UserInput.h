#pragma once
#include <utility>
#include <array>
#include <vector>

enum class CellState
{
    empty = 0,
    mine
};

using Position = std::pair<int, int>;
using Cells = std::array<std::array<CellState, 8>, 10>;
using MinePositions = std::vector<Position>;

class IUserInput
{
public:
    virtual ~IUserInput() = default;

    virtual Position GetPos() = 0;

    virtual bool Init(const Cells &, const MinePositions &) = 0;

    virtual void OnResultEmpty(int, int, int) = 0;
    virtual void OnResultMine(int, int) = 0;
};

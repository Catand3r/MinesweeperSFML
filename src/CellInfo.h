#pragma once
#include "array"
#include "utility"
#include "vector"

enum class CellState
{
    empty = 0,
    mine = 1
};

struct CellInfo
{
    CellState state = CellState::empty;

    bool uncovered = false;
    bool marked = false;

    void SetUncovered()
    {
        uncovered = true;
        marked = false;
    }

    void SetMarked()
    {
        if (!uncovered)
        {
            uncovered = false;
            marked = true;
        }
    }

    void SetCovered()
    {
        uncovered = false;
        marked = false;
    }
};

using Position = std::pair<int, int>;
using Cells = std::vector<std::vector<CellInfo>>;
using MinePositions = std::vector<Position>;

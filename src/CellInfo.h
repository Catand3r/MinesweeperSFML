#pragma once
#include "array"
#include "vector"
#include "utility"


enum class CellState
{
	empty = 0,
	mine = 1
};

struct CellInfo
{
	CellState state;

	bool uncovered;
	bool marked;

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
using Cells = std::array<std::array<CellInfo, 8>, 10>;
using MinePositions = std::vector<Position>;
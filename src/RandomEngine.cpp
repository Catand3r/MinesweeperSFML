#include "RandomEngine.h"
#include <cstdlib>
#include <ctime>

MinePositions RandomEngine::RandomizeMinePlacement(const Cells& cells_, int mineAmount, int playerX, int playerY)
{
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	MinePositions result;
	int randomizationValue = static_cast<int>(cells_.size()) * static_cast<int>(cells_[0].size()) / mineAmount;
	while (mineAmount > 0)
	{
		for (int i = 0; i < cells_.size(); i++)
		{
			for (int j = 0; j < cells_[i].size(); j++)
			{
				if (mineAmount > 0 && i != playerX && j != playerY && cells_[i][j].state == CellState::empty && std::rand() % randomizationValue == 1)
				{
					mineAmount--;
					result.push_back({ i, j });
				}
			}
		}
	}
	return result;
}
#include "Minesweeper.h"
#include <iostream>
#include <exception>
#include <array>
#include <utility>

Minesweeper::Minesweeper(IUserInput* userInput, IRandomEngine* randomEngine) : userInput_(userInput),
randomEngine_(randomEngine),
flagAmount_(10),
mineAmount(10)
{
	CreateEmptyBoard();
	CreateEmptyMinePositions();

	userInput->Init(cells_, minePostitions_, flagAmount_);
}


void Minesweeper::CreateEmptyBoard()
{
	for (int i = 0; i < cells_.size(); i++)
	{
		for (int j = 0; j < cells_[i].size(); j++)
		{
			cells_[i][j].state = CellState::empty;
		}
	}
}

void Minesweeper::CreateEmptyMinePositions()
{
	minePostitions_.clear();
}

bool Minesweeper::Run()
{
	bool result = true;
	Action action = userInput_->PollEvent();
	if (action.actionType_ == ActionType::Close)
	{
		result = false;
	}
	else if (action.actionType_ == ActionType::None)
	{
		userInput_->Delay(100);
		result =  true;
	}
	else if (action.actionType_ == ActionType::CheckCell || action.actionType_ == ActionType::MarkCell)
	{
		auto [x, y] = action.playerPos_;
		if (x < 0 || x > cells_.size() || y < 0 || y > cells_[0].size())
		{
			throw std::out_of_range("User input is out of range\n");
		}
		if (action.actionType_ == ActionType::CheckCell && firstCheck_)
		{
			firstCheck_ = false;
			minePostitions_ = randomEngine_->RandomizeMinePlacement(cells_, mineAmount, x, y);
			PlaceMines();
			result = ExecuteCheckCell(x, y);
		}
		else if (action.actionType_ == ActionType::CheckCell && !firstCheck_)
		{
			result = ExecuteCheckCell(x, y);
		}
		else if (action.actionType_ == ActionType::MarkCell)
		{
			result = ExecuteMarkCell(x, y);
		}
	}

	userInput_->Draw();
	return result;
}

bool Minesweeper::ExecuteCheckCell(int x, int y)
{
	if (cells_[x][y].state == CellState::mine)
	{
		userInput_->OnResultMine(x, y);
		userInput_->Delay(1000);
		return false;
	}
	else if (cells_[x][y].state != CellState::mine)
	{
		cells_[x][y].SetUncovered();
		userInput_->OnResultEmpty(x, y, CheckAroundCell(x, y));
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

void Minesweeper::PlaceMines()
{
	for (const auto& [x,y] : minePostitions_)
	{
		cells_[x][y].state = CellState::mine;
	}
}

int Minesweeper::CheckAroundCell(int x, int y)
{
	int mineAroundAmount = 0;
	std::array<Position, 8> positions =
	{
		Position{x, y + 1},
		Position{x, y - 1},
		Position{x + 1, y},
		Position{x - 1, y},
		Position{x + 1, y + 1},
		Position{x + 1, y - 1},
		Position{x - 1, y + 1},
		Position{x - 1, y - 1},
	};

	for (const auto& [newX, newY] : positions)
	{
		if (newX >= 0 && newX < cells_.size() && newY >= 0 && newY < cells_[0].size() && cells_[newX][newY].state == CellState::mine)
		{
			mineAroundAmount++;
		}
	}
	return mineAroundAmount;
}

void Minesweeper::CheckCellsAroundCell(int x, int y)
{
	
}
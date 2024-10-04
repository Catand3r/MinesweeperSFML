#include "Minesweeper.h"
#include <iostream>
#include <exception>
#include <array>
#include <utility>
#include <cstdlib>
#include <ctime>

Minesweeper::Minesweeper(IUserInput* userInput, IRandomEngine* randomEngine) : userInput_(userInput),
randomEngine_(randomEngine),
flagAmount_(10),
mineAmount(10)
{
	CreateEmptyBoard();
	minePostitions_ = randomEngine_->RandomizeMinePlacement(cells_, mineAmount);
	PlaceMines();

	userInput->Init(cells_, minePostitions_);
}

bool Minesweeper::Run()
{
	Action action = userInput_->PollEvent();
	if (action.actionType_ == ActionType::Close)
	{
		return false;
	}
	else if (action.actionType_ == ActionType::None)
	{
		userInput_->Delay(100);
		return true;
	}
	else
	{
		auto [x, y] = action.playerPos_;
		if (x < 0 || x > cells_.size() || y < 0 || y > cells_[0].size())
		{
			throw std::out_of_range("User input is out of range\n");
		}
		if (action.actionType_ == ActionType::CheckCell)
		{
			return ExecuteCheckCell(x, y);
		}
		else if (action.actionType_ == ActionType::MarkCell)
		{
			return ExecuteMarkCell(x, y);
		}
	}
	return true;
}

bool Minesweeper::ExecuteCheckCell(int x, int y)
{
	if (cells_[x][y] == CellState::mine)
	{
		userInput_->OnResultMine(x, y);
		userInput_->Delay(1000);
		return false;
	}
	else
	{
		cells_[x][y] = CellState::uncovered;
		userInput_->OnResultEmpty(x, y, CheckAroundCell(x, y));
		return true;
	}
	return true;
}

bool Minesweeper::ExecuteMarkCell(int x, int y)
{
	if (cells_[x][y] != CellState::marked && cells_[x][y] != CellState::uncovered && flagAmount_ > 0)
	{
		cells_[x][y] = CellState::marked;
		userInput_->OnMarkCell(x, y);
		flagAmount_--;
	}
	return true;
}

void Minesweeper::CreateEmptyBoard()
{
	for (int i = 0; i < cells_.size(); i++)
	{
		for (int j = 0; j < cells_[i].size(); j++)
		{
			cells_[i][j] = CellState::empty;
		}
	}
}

void Minesweeper::PlaceMines()
{
	for (int i = 0; i < minePostitions_.size(); i++)
	{
		cells_[minePostitions_[i].first][minePostitions_[i].second] = CellState::mine;
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
		if (newX >= 0 && newX < cells_.size() && newY >= 0 && newY < cells_[0].size() && cells_[newX][newY] == CellState::mine)
		{
			mineAroundAmount++;
		}
	}
	return mineAroundAmount;
}

MinePositions RandomEngine::RandomizeMinePlacement(Cells cells_, int mineAmount)
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
				if (std::rand() % randomizationValue == 1 && mineAmount > 0 && cells_[i][j] == CellState::empty)
				{
					mineAmount--;
					result.push_back({ i, j });
				}
			}
		}
	}
	return result;
}

Action KeyboardUserInput::PollEvent()
{
	Action action;
	std::cout << "Wybierz wspolrzedna x: ";
	int x;
	std::cin >> x;
	std::cout << "Wybierz wspolrzedna y: ";
	int y;
	std::cin >> y;
	action.actionType_ = ActionType::CheckCell;
	action.playerPos_ = { x, y };
	return action;
}

void KeyboardUserInput::OnResultEmpty(int x, int y, int minesAroundCell)
{
	std::cout << "Na wspolrzednych: " << x << " i " << y << " jest pusto, a wokol sa " << minesAroundCell << " miny. Grasz dalej\n";
}

void KeyboardUserInput::OnResultMine(int x, int y)
{
	std::cout << "Na wspolrzednych: " << x << " i " << y << " jest mina. Przegrywasz";
}

void KeyboardUserInput::OnMarkCell(int x, int y)
{
	
}

bool KeyboardUserInput::Init(const Cells&, const MinePositions&)
{
	return true;
}
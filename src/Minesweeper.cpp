#include "Minesweeper.h"
#include <iostream>
#include <array>
#include <cstdlib>
#include <ctime>

Minesweeper::Minesweeper() : flagAmount(10),
                             mineAmount(10)
{
    std::srand(std::time(nullptr));
    CreateEmptyBoard();
}

void Minesweeper::Run()
{
    while (true)
    {
        RandomizeMinePlacement();
        std::cout << "Wybierz wspolrzedna x: ";
        int x;
        std::cin >> x;
        std::cout << "Wybierz wspolrzedna y: ";
        int y;
        std::cin >> y;
        if (cells_[x][y] == CellState::mine)
        {
            std::cout << "Na wspolrzednych: " << x << " i " << y << " jest mina. Przegrywasz";
            return;
        }
        else
        {
            std::cout << "Na wspolrzednych: " << x << " i " << y << " jest pusto, a wokol sa " << CheckAroundCell(x, y) << " miny. Grasz dalej\n";
        }
    }
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

void Minesweeper::RandomizeMinePlacement()
{
    for (int i = 0; i < cells_.size(); i++)
    {
        for (int j = 0; j < cells_[i].size(); j++)
        {
            if (std::rand() % 2 == 1 && mineAmount > 0)
            {
                cells_[i][j] = CellState::mine;
                mineAmount--;
                std::cout << "Postawiono minę na x: " << i << " y: " << j << ", pozostało " << mineAmount << " min\n";
            }
        }
    }
}

int Minesweeper::CheckAroundCell(int x, int y)
{
    int mineAroundAmount = 0;
    if (cells_[x][y + 1] == CellState::mine && x >= 0 && y + 1 >= 0)
    {
        std::cout << "Na " << x << " " << y + 1 << " jest mina.\n";
        mineAroundAmount++;
    }
    if (cells_[x][y - 1] == CellState::mine && x >= 0 && y - 1 >= 0)
    {
        std::cout << "Na " << x << " " << y - 1 << " jest mina.\n";
        mineAroundAmount++;
    }
    if (cells_[x + 1][y] == CellState::mine && x + 1 >= 0 && y >= 0)
    {
        std::cout << "Na " << x + 1 << " " << y << " jest mina.\n";
        mineAroundAmount++;
    }
    if (cells_[x - 1][y] == CellState::mine && x - 1 >= 0 && y >= 0)
    {
        std::cout << "Na " << x - 1 << " " << y << " jest mina.\n";
        mineAroundAmount++;
    }
    if (cells_[x + 1][y + 1] == CellState::mine && x + 1 >= 0 && y + 1 >= 0)
    {
        std::cout << "Na " << x + 1 << " " << y + 1 << " jest mina.\n";
        mineAroundAmount++;
    }
    if (cells_[x + 1][y - 1] == CellState::mine && x + 1 >= 0 && y - 1 >= 0)
    {
        std::cout << "Na " << x + 1 << " " << y - 1 << " jest mina.\n";
        mineAroundAmount++;
    }
    if (cells_[x - 1][y + 1] == CellState::mine && x - 1 >= 0 && y + 1 >= 0)
    {
        std::cout << "Na " << x - 1 << " " << y + 1 << " jest mina.\n";
        mineAroundAmount++;
    }
    if (cells_[x - 1][y - 1] == CellState::mine && x - 1 >= 0 && y - 1 >= 0)
    {
        std::cout << "Na " << x - 1 << " " << y - 1 << " jest mina.\n";
        mineAroundAmount++;
    }
    return mineAroundAmount;
}
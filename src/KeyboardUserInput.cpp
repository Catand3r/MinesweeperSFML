#include "KeyboardUserInput.h"
#include <iostream>

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
    action.playerPos_ = {x, y};
    return action;
}

bool KeyboardUserInput::Init(const Cells &, const MinePositions &, const int &)
{
    return true;
}

void KeyboardUserInput::OnResultEmpty(int x, int y, int minesAroundCell)
{
    std::cout << "Na wspolrzednych: " << x << " i " << y << " jest pusto, a wokol sa " << minesAroundCell
              << " miny. Grasz dalej\n";
}

void KeyboardUserInput::OnResultMine(int x, int y)
{
    std::cout << "Na wspolrzednych: " << x << " i " << y << " jest mina. Przegrywasz";
}

void KeyboardUserInput::OnMarkCell(int x, int y, int)
{
}

void KeyboardUserInput::OnGameWon()
{
}

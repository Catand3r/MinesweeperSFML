#pragma once
#include "CellInfo.h"
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

struct Action
{
public:
    static Action CreateNoneAction()
    {
        Action noneAction;
        noneAction.actionType_ = ActionType::None;

        return noneAction;
    }

    ActionType actionType_;
    Position playerPos_;
};


class IUserInput
{
public:

    virtual ~IUserInput() = default;

    virtual bool Init(const Cells &, const MinePositions &, const int &) = 0;

    virtual void OnResultEmpty(int, int, int) = 0;
    virtual void OnResultMine(int, int) = 0;
    virtual void OnMarkCell(int, int, int) = 0;

    virtual void OnGameWon() = 0;

    virtual Action PollEvent() = 0;

    virtual void Draw() { };

    virtual void Delay(int) const { };

};

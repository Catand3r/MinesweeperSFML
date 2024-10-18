#pragma once
#include "UserInput.h"

class KeyboardUserInput : public IUserInput
{
public:
    ~KeyboardUserInput() override = default;

    Action PollEvent() override;

    bool Init(const Cells&, const MinePositions&, const int&) override;

    void OnResultEmpty(int, int, int) override;
    void OnResultMine(int, int) override;
    void OnMarkCell(int, int, int) override;
};
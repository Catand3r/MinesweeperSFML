#pragma once
#include "CellInfo.h"

class IRandomEngine
{
public:
    virtual ~IRandomEngine() = default;

    virtual MinePositions RandomizeMinePlacement(Cells, int, int, int) = 0;
};

class RandomEngine : public IRandomEngine
{
    ~RandomEngine() override = default;

    MinePositions RandomizeMinePlacement(Cells, int, int, int) override;
};
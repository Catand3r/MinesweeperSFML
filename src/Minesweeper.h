#include <iostream>
#include <array>

enum class CellState
{
    empty = 0,
    mine
};

using Cells = std::array<std::array<CellState, 8>, 10>;

class Minesweeper
{
public:
    Minesweeper();
    void Run();

private:
    Cells cells_;
    int flagAmount;
    int mineAmount;
    void CreateEmptyBoard();
    void RandomizeMinePlacement();
};
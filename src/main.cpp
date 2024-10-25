#include <iostream>
#include <array>
#include "Minesweeper.h"
#include "SFMLUserInput.h"
#include "KeyboardUserInput.h"

int main()
{
    int x = 15;
    int y = 12;
    IRandomEngine *randomEngine = new RandomEngine();
    IUserInput *userInput = new SFMLUserInput(x, y);
    Minesweeper ms(userInput, randomEngine, x, y);
    bool keepRunning = true;
    while (keepRunning)
    {
        keepRunning = ms.Run();
    }
    delete userInput;
    delete randomEngine;
}
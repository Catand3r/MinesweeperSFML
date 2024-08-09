#include <iostream>
#include <array>
#include "Minesweeper.h"

int main()
{
    IRandomEngine *randomEngine = new RandomEngine();
    IUserInput *userInput = new KeyboardUserInput();
    Minesweeper ms(userInput, randomEngine);
    bool keepRunning = true;
    while (keepRunning)
    {
        keepRunning = ms.Run();
    }
    delete userInput;
    delete randomEngine;
}
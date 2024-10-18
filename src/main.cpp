#include <iostream>
#include <array>
#include "Minesweeper.h"
#include "SFMLUserInput.h"
#include "KeyboardUserInput.h"

int main()
{
    IRandomEngine *randomEngine = new RandomEngine();
    IUserInput *userInput = new SFMLUserInput();
    Minesweeper ms(userInput, randomEngine);
    bool keepRunning = true;
    while (keepRunning)
    {
        keepRunning = ms.Run();
    }
    delete userInput;
    delete randomEngine;
}
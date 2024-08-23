#include "SFMLUserInput.h"
#include "SFML/Graphics.hpp"

Position SFMLUserInput::GetPos()
{
    return {0, 0};
}

void SFMLUserInput::OnResultEmpty(int, int, int)
{
}

void SFMLUserInput::OnResultMine(int, int)
{
}
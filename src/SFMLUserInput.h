#include "UserInput.h"
#include "SFML/Graphics.hpp"

class SFMLUserInput : public IUserInput
{
public:
    ~SFMLUserInput() override = default;

    virtual Position GetPos() override;

    virtual void OnResultEmpty(int, int, int) override;
    virtual void OnResultMine(int, int) override;

private:
    sf::RenderWindow window_;
};
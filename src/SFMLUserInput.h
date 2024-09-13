#include "UserInput.h"
#include "SFML/Graphics.hpp"

using GCells = std::array<std::array<sf::RectangleShape, 8>, 10>;

class SFMLUserInput : public IUserInput
{
public:

    SFMLUserInput();

    ~SFMLUserInput() override = default;

    virtual Position GetPos() override;

    virtual void OnResultEmpty(int, int, int) override;
    virtual void OnResultMine(int, int) override;

    virtual bool Init(const Cells&, const MinePositions&);

private:
    sf::RenderWindow window_;
    GCells gcells_;
    sf::RectangleShape makeRectangle(float, float, float, float, sf::Color = sf::Color::White);
    void Draw();
    void GCellsUpdate();
    float cellSizeX;
    float cellSizeY;
    Cells cells;
};
#include "UserInput.h"
#include "SFML/Graphics.hpp"
#include <optional>
#include <string>

struct GraphicCell
{
public:
    sf::RectangleShape cellShape_;
    sf::Text mineAmountText_;
};

using GraphicCells = std::array<std::array<GraphicCell, 8>, 10>;

class SFMLUserInput : public IUserInput
{
public:

    SFMLUserInput();

    ~SFMLUserInput() override = default;

    virtual void OnResultEmpty(int, int, int) override;
    virtual void OnResultMine(int, int) override;
    virtual void OnMarkCell(int, int) override;

    virtual bool Init(const Cells&, const MinePositions&) override;
    
    virtual Action PollEvent() override;

    virtual void Delay(int) const override;

private:
    
    sf::Font font_;
    sf::RenderWindow window_;
    GraphicCells graphicCells_;
    const Cells* cells_ = nullptr;
    float cellSize_ = 40.0f;
    ActionType ActionType_;
    sf::RectangleShape makeRectangle(float, float, float, float, sf::Color = sf::Color::White);
    sf::Text makeText(int, std::string, float, float, sf::Color = sf::Color::White);
    Position CalculateMousePosition();
    bool ShouldReadEvent(sf::Event&, Action, sf::Clock);
    void Draw();
    void GraphicCellsUpdate();
    void GraphicCellsMake();
};
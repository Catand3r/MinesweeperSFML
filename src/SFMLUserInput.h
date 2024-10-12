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
    virtual void OnMarkCell(int, int, int) override;

    virtual bool Init(const Cells&, const MinePositions&, const int&) override;

    virtual Action PollEvent() override;

    virtual void Draw() override;

    virtual void Delay(int) const override;

private:

    sf::Clock clock_;
    sf::Text clockText_;

    sf::Text flagAmountLeftText_;

    sf::Font font_;

    sf::RenderWindow window_;

    GraphicCells graphicCells_;
    ActionType ActionType_;

    const Cells* cells_ = nullptr;

    const int* flagAmount_;

    float cellSize_ = 20.0f;
    float boardSizeX_ = 400.0f;
    float boardSizeY_ = 400.0f;

    std::optional<Position> CalculateMousePosition();

    sf::RectangleShape makeRectangle(float, float, float, float, sf::Color = sf::Color::White);
    sf::Text makeText(int, std::string, float, float, sf::Color = sf::Color::White);

    bool ShouldReadEvent(sf::Event&, Action, sf::Clock);

    void GraphicCellsUpdate();
    void GraphicCellsMake();

    void SetClockText();
    void UpdateClockText();

    void SetFlagAmountLeftText(std::string);
    void UpdateFlagAmountLeftText(std::string);
};
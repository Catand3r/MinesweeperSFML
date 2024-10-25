#include "SFMLUserInput.h"
#include "SFML/Audio.hpp"
#include "SFML/Graphics.hpp"
#include <iostream>

SFMLUserInput::SFMLUserInput(int x, int y) : window_(sf::VideoMode(40 * x, 40 * y + 40), "Minesweeper")
{
}

void SFMLUserInput::OnResultEmpty(int x, int y, int mineAroundCell)
{
    if (mineAroundCell > 0)
        graphicCells_[x][y].mineAmountText_.setString(std::to_string(mineAroundCell));
}

void SFMLUserInput::OnResultMine(int x, int y)
{
    isGameRunning_ = false;
    graphicCells_[x][y].cellShape_.setFillColor(sf::Color::Red);
}

void SFMLUserInput::OnMarkCell(int x, int y, int flagAmount)
{
    UpdateFlagAmountLeftText(std::to_string(flagAmount));
}

void SFMLUserInput::OnGameWon()
{
    isGameRunning_ = false;
    UpdateWonText();
}

bool SFMLUserInput::Init(const Cells &cells, const MinePositions &, const int &flagAmount)
{
    if (!font_.loadFromFile("../fonts/comic.ttf"))
    {
        return false;
    }

    if (!music_.openFromFile("../soundfx/fortniteballs.wav"))
        return false; // error
    music_.setVolume(100.0f);
    music_.play();

    cells_ = &cells;
    int windowSizeX = static_cast<int>(cells_->size()) * 40;
    int windowSizeY = (static_cast<int>(cells_->at(0).size()) * 40) + 40;
    window_.setSize(sf::Vector2u(windowSizeX, windowSizeY));
    boardSizeX_ = static_cast<float>(window_.getSize().x);
    boardSizeY_ = static_cast<float>(window_.getSize().y - 40);
    cellSize_ = static_cast<float>(boardSizeX_) / static_cast<float>(cells_->size());
    SetWonText();
    SetClockText();
    SetFlagAmountLeftText(std::to_string(flagAmount));
    GraphicCellsMake();
    clock_.restart();

    return true;
}

void SFMLUserInput::Draw()
{
    if (isGameRunning_)
    {
        UpdateClockText();
        GraphicCellsUpdate();
    }

    window_.clear();

    window_.draw(gameWonText_);
    window_.draw(clockText_);
    window_.draw(flagAmountLeftText_);

    for (int i = 0; i < graphicCells_.size(); i++)
    {
        for (int j = 0; j < graphicCells_[i].size(); j++)
        {
            window_.draw(graphicCells_[i][j].cellShape_);
            window_.draw(graphicCells_[i][j].mineAmountText_);
        }
    }
    window_.display();
}

Action SFMLUserInput::PollEvent()
{
    sf::Event event;
    sf::Clock eventTimer;

    Action action = Action::CreateNoneAction();

    while (ShouldReadEvent(event, action, eventTimer))
        ;
    {
        if (event.type == sf::Event::Closed)
        {
            action.actionType_ = ActionType::Close;
            window_.close();
        }
        else if (event.type == sf::Event::MouseButtonPressed)
        {
            if (auto mousePosition = CalculateMousePosition())
            {
                action.playerPos_ = *mousePosition;
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    action.actionType_ = ActionType::CheckCell;
                else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
                    action.actionType_ = ActionType::MarkCell;
            }
        }
        else if (event.type == sf::Event::MouseButtonReleased)
        {
            action.actionType_ = ActionType::None;
        }
    }
    return action;
}

void SFMLUserInput::SetWonText()
{
    gameWonText_ =
        makeText(static_cast<int>(cellSize_ - static_cast<float>((0.15 * cellSize_))), "",
                 static_cast<float>(.5f * boardSizeX_), static_cast<float>(.5f * cellSize_), sf::Color::Green);
}

void SFMLUserInput::UpdateWonText()
{
    gameWonText_.setString("You won");
    auto center = gameWonText_.getGlobalBounds().getSize() / 2.f;
    auto localBounds = center + gameWonText_.getLocalBounds().getPosition();
    auto rounded = sf::Vector2f{std::round(localBounds.x), std::round(localBounds.y)};
    gameWonText_.setOrigin(rounded);
}

void SFMLUserInput::SetClockText()
{
    clockText_ = makeText(static_cast<int>(cellSize_ - static_cast<float>((0.25 * cellSize_))), "skibidi",
                          .125f * boardSizeX_, static_cast<float>(.5f * cellSize_), sf::Color::White);
}

void SFMLUserInput::UpdateClockText()
{
    int currentElapsedTime = static_cast<int>(clock_.getElapsedTime().asSeconds());
    std::string elapsedTimeString = std::to_string(currentElapsedTime);
    clockText_.setString(elapsedTimeString);
}

void SFMLUserInput::SetFlagAmountLeftText(std::string textString)
{
    flagAmountLeftText_ = makeText(static_cast<int>(cellSize_ - static_cast<float>((0.25 * cellSize_))), textString,
                                   .875f * boardSizeX_, static_cast<float>(.5f * cellSize_), sf::Color::White);
}

void SFMLUserInput::UpdateFlagAmountLeftText(std::string textString)
{
    flagAmountLeftText_.setString(textString);
}

void SFMLUserInput::GraphicCellsMake()
{
    graphicCells_.resize(cells_->size());
    for (int i = 0; i < graphicCells_.size(); i++)
    {
        graphicCells_[i].resize(cells_->at(i).size());
        for (int j = 0; j < graphicCells_[i].size(); j++)
        {
            float posX = i * cellSize_;
            float posY = j * cellSize_ + cellSize_;
            graphicCells_[i][j].cellShape_ = makeRectangle(cellSize_, cellSize_, posX, posY);
            graphicCells_[i][j].mineAmountText_ =
                makeText(cellSize_, "", posX + .25f * cellSize_, posY - .125f * cellSize_, sf::Color::Black);
        }
    }
}

void SFMLUserInput::GraphicCellsUpdate()
{
    for (int i = 0; i < graphicCells_.size(); i++)
    {
        for (int j = 0; j < graphicCells_[i].size(); j++)
        {
            if (cells_->at(i).at(j).uncovered && cells_->at(i).at(j).state == CellState::empty)
            {
                graphicCells_[i][j].cellShape_.setFillColor(sf::Color::Green);
            }
            else if (cells_->at(i).at(j).uncovered && cells_->at(i).at(j).state == CellState::mine)
            {
                graphicCells_[i][j].cellShape_.setFillColor(sf::Color::Red);
            }
            else if (cells_->at(i).at(j).marked)
            {
                graphicCells_[i][j].cellShape_.setFillColor(sf::Color::Yellow);
            }
            else if (!cells_->at(i).at(j).uncovered && !cells_->at(i).at(j).marked)
            {
                graphicCells_[i][j].cellShape_.setFillColor(sf::Color::White);
            }
        }
    }
}

std::optional<Position> SFMLUserInput::CalculateMousePosition()
{
    sf::Vector2i mousePosition = sf::Mouse::getPosition(window_);
    for (int i = 0; i < graphicCells_.size(); i++)
    {
        for (int j = 0; j < graphicCells_[i].size(); j++)
        {
            if (graphicCells_[i][j].cellShape_.getGlobalBounds().contains(sf::Vector2f(mousePosition)))
            {
                return Position{i, j};
            }
        }
    }
    return std::nullopt;
}

void SFMLUserInput::Delay(int delayTime) const
{
    sf::sleep(sf::milliseconds(delayTime));
}

bool SFMLUserInput::ShouldReadEvent(sf::Event &event, Action action, sf::Clock eventTimer)
{
    return window_.pollEvent(event) && action.actionType_ == ActionType::None &&
           eventTimer.getElapsedTime() < sf::milliseconds(100);
}

sf::RectangleShape SFMLUserInput::makeRectangle(float sizeX, float sizeY, float posX, float posY, sf::Color color)
{
    sf::RectangleShape rect;
    rect.setSize(sf::Vector2f(sizeX, sizeY));
    rect.setFillColor(color);
    rect.setOutlineThickness(-2.0f);
    rect.setOutlineColor(sf::Color::Black);
    rect.setPosition(sf::Vector2f(posX, posY));
    return rect;
}

sf::Text SFMLUserInput::makeText(int size, std::string textString, float x, float y, sf::Color color)
{
    sf::Text text(textString, font_);
    text.setCharacterSize(size);
    text.setFillColor(color);
    text.setPosition(sf::Vector2f(x, y));
    auto center = text.getGlobalBounds().getSize() / 2.f;
    auto localBounds = center + text.getLocalBounds().getPosition();
    auto rounded = sf::Vector2f{std::round(localBounds.x), std::round(localBounds.y)};
    text.setOrigin(rounded);
    return text;
}

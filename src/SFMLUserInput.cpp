#include <iostream>
#include "SFMLUserInput.h"
#include "SFML/Graphics.hpp"


SFMLUserInput::SFMLUserInput() : window_(sf::VideoMode(400, 320), "Minesweeper")
{
    
}

void SFMLUserInput::OnResultEmpty(int x, int y, int mineAroundCell)
{
    GraphicCellsUpdate();
    graphicCells_[x][y].mineAmountText_.setString(std::to_string(mineAroundCell));
    Draw();
}

void SFMLUserInput::OnResultMine(int x, int y)
{
    graphicCells_[x][y].cellShape_.setFillColor(sf::Color::Red);
    Draw();
}

void SFMLUserInput::OnMarkCell(int x, int y)
{
    GraphicCellsUpdate();
    Draw();
}

bool SFMLUserInput::Init(const Cells& cells, const MinePositions&)
{
    if (!font_.loadFromFile("../fonts/comic.ttf"))
    {
        return false;
    }

    cells_ = &cells;
    int windowSizeX = static_cast<int>(cells_->size()) * 40;
    int windowSizeY = static_cast<int>(cells_->at(0).size())* 40;
    window_.setSize(sf::Vector2u(windowSizeX, windowSizeY));
    cellSize_ = static_cast<float>(window_.getSize().x) / static_cast<float>(cells_->size());
    GraphicCellsMake();
    Draw();
    
    return true;
}

void SFMLUserInput::Draw()
{
        window_.clear();

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

void SFMLUserInput::GraphicCellsMake()
{
    for (int i = 0; i < graphicCells_.size(); i++)
    {
        for (int j = 0; j < graphicCells_[i].size(); j++)
        {
            if (cells_->at(i).at(j) == CellState::uncovered)
            {
                graphicCells_[i][j].mineAmountText_ = makeText(cellSize_ , "", i * cellSize_, j * cellSize_, sf::Color::Black);
                graphicCells_[i][j].cellShape_ = makeRectangle(cellSize_, cellSize_, i * cellSize_, j * cellSize_, sf::Color::Green);
            }
            else if (cells_->at(i).at(j) == CellState::marked)
            {
                graphicCells_[i][j].mineAmountText_ = makeText(cellSize_, "", i * cellSize_, j * cellSize_, sf::Color::Black);
                graphicCells_[i][j].cellShape_ = makeRectangle(cellSize_, cellSize_, i * cellSize_, j * cellSize_, sf::Color::Yellow);
            }
            else
            {
                graphicCells_[i][j].mineAmountText_ = makeText(cellSize_, "", i * cellSize_, j * cellSize_, sf::Color::Black);
                graphicCells_[i][j].cellShape_ = makeRectangle(cellSize_, cellSize_, i * cellSize_, j * cellSize_, sf::Color::White);
            }
        }
    }
}

void SFMLUserInput::GraphicCellsUpdate()
{
    for (int i = 0; i < graphicCells_.size(); i++)
    {
        for (int j = 0; j < graphicCells_[i].size(); j++)
        {
            if (cells_->at(i).at(j) == CellState::uncovered)
            {
                graphicCells_[i][j].cellShape_.setFillColor(sf::Color::Green);
            }
            else if (cells_->at(i).at(j) == CellState::marked)
            {
                graphicCells_[i][j].cellShape_.setFillColor(sf::Color::Yellow);
            }
            else
            {
                graphicCells_[i][j].cellShape_.setFillColor(sf::Color::White);
            }
        }
    }
}


sf::RectangleShape SFMLUserInput::makeRectangle(float sizeX, float sizeY, float posX, float posY, sf::Color color)
{
    sf::RectangleShape rect;
    rect.setSize(sf::Vector2f(sizeX, sizeY));
    rect.setFillColor(color);
    rect.setOutlineThickness(2.0f);
    rect.setOutlineColor(sf::Color::Black);
    rect.setPosition(sf::Vector2f(posX, posY));
    return rect;
}

sf::Text SFMLUserInput::makeText(int size, std::string textString, float x, float y, sf::Color color)
{
    sf::Text text("", font_);
    text.setCharacterSize(size);
    text.setString(textString);
    text.setFillColor(color);
    text.setPosition(sf::Vector2f(x, y));
    return text;
}

Action SFMLUserInput::PollEvent()
{
    sf::Event event;
    sf::Clock eventTimer;

    Action action = Action::CreateNoneAction();

    while (ShouldReadEvent(event, action, eventTimer));
    {
        if (event.type == sf::Event::Closed)
        {
            action.actionType_ = ActionType::Close;
            window_.close();
        }
        else if (event.type == sf::Event::MouseButtonPressed)
        {
            Position mousePosition = CalculateMousePosition();
            action.playerPos_ = mousePosition;
            
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                action.actionType_ = ActionType::CheckCell;
            else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
                action.actionType_ = ActionType::MarkCell;
        }
        else if (event.type == sf::Event::MouseButtonReleased)
        {
            action.actionType_ = ActionType::None;
        }
    }
    return action;
}

Position SFMLUserInput::CalculateMousePosition()
{
    sf::Vector2i mousePosition = sf::Mouse::getPosition(window_);
    int x = static_cast<int>(mousePosition.x) / static_cast<int>(cellSize_);
    int y = static_cast<int>(mousePosition.y) / static_cast<int>(cellSize_);
    return { x, y };
}

void SFMLUserInput::Delay(int delayTime) const
{
    sf::sleep(sf::milliseconds(delayTime));
}

bool SFMLUserInput::ShouldReadEvent(sf::Event& event, Action action, sf::Clock eventTimer)
{
    return window_.pollEvent(event) && action.actionType_ == ActionType::None && eventTimer.getElapsedTime() < sf::milliseconds(100);
}
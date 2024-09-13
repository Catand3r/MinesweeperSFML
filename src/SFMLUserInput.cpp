#include "SFMLUserInput.h"
#include "SFML/Graphics.hpp"

SFMLUserInput::SFMLUserInput() : window_(sf::VideoMode(320, 400), "Minesweeper")
{
    
}

Position SFMLUserInput::GetPos()
{

    return {-1, -1};
}

void SFMLUserInput::OnResultEmpty(int, int, int)
{
    Draw();
}

void SFMLUserInput::OnResultMine(int x, int y)
{
    gcells_[x][y] = makeRectangle(cellSizeX, cellSizeY, cellSizeX * x, cellSizeY * y, sf::Color::Red);
}

bool SFMLUserInput::Init(const Cells& cells, const MinePositions&)
{
    cells_ = &cells;
    cellSizeX = static_cast<float>(window_.getSize().x) / static_cast<float>(cells_->size());
    cellSizeY = static_cast<float>(window_.getSize().y) / static_cast<float>(cells_[0].size());
    for (int i = 0; i < cells_->size(); i++)
    {
        for (int j = 0; j < cells_->at(i).size(); j++)
        {
            gcells_[i][j] = makeRectangle(cellSizeX, cellSizeY, cellSizeX * i, cellSizeY * j);
        }
    }
    return true;
}

void SFMLUserInput::Draw()
{
    GCellsUpdate();
    while (window_.isOpen())
    {
        // Event processing
        

        // Clear the whole window before rendering a new frame
        window_.clear();

        for (int i = 0; i < gcells_.size(); i++)
        {
            for (int j = 0; j < gcells_[i].size(); j++)
            {
                window_.draw(gcells_[i][j]);
            }
        }

        // End the current frame and display its contents on screen
        window_.display();
    }
}

void SFMLUserInput::GCellsUpdate()
{
    for (int i = 0; i < gcells_.size(); i++)
    {
        for (int j = 0; j < gcells_[i].size(); j++)
        {
            if (cells_->at(i).at(j) == CellState::uncovered)
            {
                gcells_[i][j] = makeRectangle(cellSizeX, cellSizeY, i * cellSizeX, j * cellSizeY, sf::Color::Green);
            }
            else
            {
                gcells_[i][j] = makeRectangle(cellSizeX, cellSizeY, i * cellSizeX, j * cellSizeY, sf::Color::White);
            }
        }
    }
}


sf::RectangleShape SFMLUserInput::makeRectangle(float sizeX, float sizeY, float posX, float posY, sf::Color color)
{
    sf::RectangleShape rect;
    rect.setSize(sf::Vector2f(sizeX, sizeY));
    rect.setFillColor(color);
    rect.setPosition(sf::Vector2f(posX, posY));
    return rect;
}

bool SFMLUserInput::PollEvent()
{
    sf::Event event;
    while (window_.pollEvent(event))
    {
        // Request for closing the window
        if (event.type == sf::Event::Closed)
        {
            window_.close();
            return false;
        }
    }
    return true;
}

void SFMLUserInput::Delay() const
{
    sf::sleep(sf::milliseconds(100));
}
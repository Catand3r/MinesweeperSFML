#include <iostream>
#include "SFMLUserInput.h"
#include "SFML/Graphics.hpp"

SFMLUserInput::SFMLUserInput() : window_(sf::VideoMode(320, 400), "Minesweeper")
{
    
}

Position SFMLUserInput::GetPos()
{
    if (isButtonPressed_)
    {
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window_);
        int x = mousePosition.x / cellSizeX_;
        int y = mousePosition.y / cellSizeY_;
        return { x, y };
    }

    return {-1, -1};
}

void SFMLUserInput::OnResultEmpty(int, int, int)
{
    Draw();
}

void SFMLUserInput::OnResultMine(int x, int y)
{
    gcells_[x][y] = makeRectangle(cellSizeX_, cellSizeY_, cellSizeX_ * x, cellSizeY_ * y, sf::Color::Red);
}

bool SFMLUserInput::Init(const Cells& cells, const MinePositions&)
{
    cells_ = &cells;
    int windowSizeX = cells_->size() * 20;
    int windowSizeY = cells_->at(0).size() * 20;
    std::cout << "Okno ma rozmiar: " << windowSizeX << " " << windowSizeY << ".\n";
    window_.setSize(sf::Vector2u(windowSizeX, windowSizeY));
    cellSizeX_ = static_cast<float>(window_.getSize().x) / static_cast<float>(cells_->size());
    cellSizeY_ = static_cast<float>(window_.getSize().y) / static_cast<float>(cells_[0].size());
    GCellsUpdate();
    return true;
}

void SFMLUserInput::Draw()
{
    GCellsUpdate();
        window_.clear();

        for (int i = 0; i < gcells_.size(); i++)
        {
            for (int j = 0; j < gcells_[i].size(); j++)
            {
                window_.draw(gcells_[i][j]);
            }
        }
        window_.display();
}

void SFMLUserInput::GCellsUpdate()
{
    for (int i = 0; i < gcells_.size(); i++)
    {
        for (int j = 0; j < gcells_[i].size(); j++)
        {
            if (cells_->at(i).at(j) == CellState::uncovered)
            {
                gcells_[i][j] = makeRectangle(cellSizeX_, cellSizeY_, i * cellSizeX_, j * cellSizeY_, sf::Color::Green);
                std::cout << "Pozycja cell numer: " << i << " " << j << " wynosi: " << i * cellSizeX_ << " " << j * cellSizeY_ << ".\n";
            }
            else
            {
                gcells_[i][j] = makeRectangle(cellSizeX_, cellSizeY_, i * cellSizeX_, j * cellSizeY_, sf::Color::White);
                std::cout << "Pozycja cell numer: " << i << " " << j << " wynosi: " << i * cellSizeX_ << " " << j * cellSizeY_ << ".\n";
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
        else if (event.type == sf::Event::MouseButtonPressed)
        {
            isButtonPressed_ = true;
        }
        else if (event.type == sf::Event::MouseButtonReleased)
        {
            isButtonPressed_ = false;
        }
    }
    return true;
}

void SFMLUserInput::Delay(int delayTime) const
{
    sf::sleep(sf::milliseconds(delayTime));
}
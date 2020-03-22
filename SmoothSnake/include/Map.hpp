#pragma once

#include "Core/Graphics.hpp"

class Map
{
public:
    Map(sf::Vector2i dimensions, Graphics &gfx);

    void Draw();

private:
    void DrawSnake(std::vector<sf::Vector2i> body, sf::Color color);

private:
    Graphics &m_gfx;

    std::vector<sf::FloatRect> m_grid;

    friend class Snake;
};
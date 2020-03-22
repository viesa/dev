#include "Map.hpp"

Map::Map(sf::Vector2i dimensions, Graphics &gfx)
    : m_gfx(gfx)
{
}

void Map::Draw()
{
    for (auto &rect : m_grid)
        m_gfx.Draw(rect, sf::Color::Transparent, true);
}

void Map::DrawSnake(std::vector<sf::Vector2i> body, sf::Color color)
{
}
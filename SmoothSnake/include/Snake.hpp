#pragma once

#include "Core/Graphics.hpp"
#include "Core/Input.hpp"

class Snake
{
public:
    Snake(sf::Vector2f const &startPos, sf::Color const &color, sf::Time &dt, Input &input, Graphics &gfx);

    void Update();
    void Draw();

private:
    std::vector<sf::FloatRect> m_body;
    sf::Color m_color;
    float m_thickness;

    sf::Vector2i m_direction;
    sf::Vector2i m_lastDirection;
    float m_speed;

    sf::Time &m_dt;
    Input &m_input;
    Graphics &m_gfx;
};
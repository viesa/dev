#include "Snake.hpp"

#include "Core/Library.hpp"
#include "Arth/Random.hpp"

#include <iostream>

Snake::Snake(sf::Vector2f const &startPos, sf::Color const &color, sf::Time &dt, Input &input, Graphics &gfx)
    : m_color(color),
      m_thickness(5.0f),
      m_direction(sf::Vector2i(1, 0)),
      m_lastDirection(m_direction),
      m_speed(40.0f),
      m_dt(dt),
      m_input(input),
      m_gfx(gfx)
{
    m_body.push_back(sf::FloatRect(startPos, sf::Vector2f(m_thickness, m_thickness)));
}

void Snake::Update()
{
    sf::Vector2i newDir(0, 0);
    if (m_input.Get(Code::KeyA) && m_lastDirection.x != 1)
        newDir.x -= 1;
    if (m_input.Get(Code::KeyW) && m_lastDirection.y != 1)
        newDir.y -= 1;
    if (m_input.Get(Code::KeyD) && m_lastDirection.x != -1)
        newDir.x += 1;
    if (m_input.Get(Code::KeyS) && m_lastDirection.y != -1)
        newDir.y += 1;

    if (m_direction.x && m_direction.y)
        m_direction.y = 0;

    if (vf::LengthSq((sf::Vector2f)newDir))
        m_direction = newDir;

    sf::Vector2f add = (sf::Vector2f)m_direction * m_speed * m_dt.asSeconds();

    if (m_direction == m_lastDirection)
    {
        auto &front = m_body.back();
        if (m_direction.x == -1)
        {
            front.left += add.x;
            front.width -= add.x;
        }
        else if (m_direction.y == -1)
        {
            front.top += add.y;
            front.height -= add.y;
        }
        else if (m_direction.x == 1)
        {
            front.width += add.x;
        }
        else if (m_direction.y == 1)
        {
            front.height += add.y;
        }
    }
    else
    {
        m_body.push_back(sf::FloatRect(m_body.back()));
        auto &front = m_body.back();
        if (m_lastDirection.x == -1)
        {
            front.width = m_thickness;
            if (m_direction.y == -1)
                front.height = add.x;
        }
        else if (m_lastDirection.y == -1)
        {
            front.height = m_thickness;
            if (m_direction.x == -1)
                front.width = add.y;
        }
        else if (m_lastDirection.x == 1)
        {
            front.left += (front.width - m_thickness);
            front.width = m_thickness;
            if (m_direction.y == -1)
                front.height = add.x;
        }
        else if (m_lastDirection.y == 1)
        {
            front.top += (front.height - m_thickness);
            front.height = m_thickness;
            if (m_direction.x == -1)
                front.width = add.y;
        }
    }

    m_lastDirection = m_direction;
}

void Snake::Draw()
{
    for (auto &rect : m_body)
    {
        m_gfx.Draw(rect, m_color);
    }
}
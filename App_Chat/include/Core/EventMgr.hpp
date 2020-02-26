#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "Input.hpp"
#include "GUI.hpp"

class EventMgr
{
public:
    EventMgr(sf::RenderWindow &m_renderWindow, Input &input, GUI &gui);

    void PollAll();

private:
    sf::RenderWindow &m_renderWindow;
    sf::Event m_event;
    Input &m_input;
    GUI &m_gui;
};
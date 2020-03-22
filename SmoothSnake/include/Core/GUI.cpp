#include "GUI.hpp"

GUI::GUI(sf::RenderWindow &renderWindow)
    : m_renderWindow(renderWindow),
      m_guiMgr(),
      m_desktop()
{
    m_desktop.Update(0.0f);
}

void GUI::Update(sf::Time dt)
{
    m_desktop.Update(dt.asSeconds());
}

void GUI::Draw()
{
    m_guiMgr.Display(m_renderWindow);
}

void GUI::HandleEvent(const sf::Event &event)
{
    m_desktop.HandleEvent(event);
}

void GUI::AddWindow(sfg::Window::Ptr window)
{
    m_desktop.Add(window);
}

void GUI::RemoveWindow(sfg::Window::Ptr window)
{
    m_desktop.Remove(window);
}
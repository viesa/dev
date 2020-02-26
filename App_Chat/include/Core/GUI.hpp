#pragma once

#include <vector>
#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

class GUI
{
public:
    GUI(sf::RenderWindow &renderWindow);

    void Update(sf::Time dt);
    void Draw();

    void HandleEvent(const sf::Event &event);

    void AddWindow(sfg::Window::Ptr window);
    void RemoveWindow(sfg::Window::Ptr window);

private:
    sf::RenderWindow &m_renderWindow;
    sfg::SFGUI m_guiMgr;
    sfg::Desktop m_desktop;
};
#include "EventMgr.hpp"

EventMgr::EventMgr(sf::RenderWindow &m_renderWindow, Input &input, GUI &gui)
    : m_renderWindow(m_renderWindow),
      m_input(input),
      m_gui(gui)
{
    m_input.Reset();
}

void EventMgr::PollAll()
{
    while (m_renderWindow.pollEvent(m_event))
    {
        m_gui.HandleEvent(m_event);
        switch (m_event.type)
        {
        case sf::Event::Closed:
            m_input.Set(EventQuit, true);
            break;

        case sf::Event::MouseButtonPressed:
        {
            switch (m_event.mouseButton.button)
            {
            case sf::Mouse::Button::Left:
                m_input.Set(Code::MouseL, true);
                break;
            case sf::Mouse::Button::Right:
                m_input.Set(Code::MouseR, true);
                break;
            default:
                break;
            }
            break;
        }
        case sf::Event::KeyPressed:
        {
            switch (m_event.key.code)
            {
            case sf::Keyboard::Key::A:
                m_input.Set(Code::KeyA, true);
                break;
            case sf::Keyboard::Key::B:
                m_input.Set(Code::KeyB, true);
                break;
            case sf::Keyboard::Key::C:
                m_input.Set(Code::KeyC, true);
                break;
            case sf::Keyboard::Key::D:
                m_input.Set(Code::KeyD, true);
                break;
            case sf::Keyboard::Key::E:
                m_input.Set(Code::KeyE, true);
                break;
            case sf::Keyboard::Key::F:
                m_input.Set(Code::KeyF, true);
                break;
            case sf::Keyboard::Key::G:
                m_input.Set(Code::KeyG, true);
                break;
            case sf::Keyboard::Key::H:
                m_input.Set(Code::KeyH, true);
                break;
            case sf::Keyboard::Key::I:
                m_input.Set(Code::KeyI, true);
                break;
            case sf::Keyboard::Key::J:
                m_input.Set(Code::KeyJ, true);
                break;
            case sf::Keyboard::Key::K:
                m_input.Set(Code::KeyK, true);
                break;
            case sf::Keyboard::Key::L:
                m_input.Set(Code::KeyL, true);
                break;
            case sf::Keyboard::Key::M:
                m_input.Set(Code::KeyM, true);
                break;
            case sf::Keyboard::Key::N:
                m_input.Set(Code::KeyN, true);
                break;
            case sf::Keyboard::Key::O:
                m_input.Set(Code::KeyO, true);
                break;
            case sf::Keyboard::Key::P:
                m_input.Set(Code::KeyP, true);
                break;
            case sf::Keyboard::Key::Q:
                m_input.Set(Code::KeyQ, true);
                break;
            case sf::Keyboard::Key::R:
                m_input.Set(Code::KeyR, true);
                break;
            case sf::Keyboard::Key::S:
                m_input.Set(Code::KeyS, true);
                break;
            case sf::Keyboard::Key::T:
                m_input.Set(Code::KeyT, true);
                break;
            case sf::Keyboard::Key::U:
                m_input.Set(Code::KeyU, true);
                break;
            case sf::Keyboard::Key::V:
                m_input.Set(Code::KeyV, true);
                break;
            case sf::Keyboard::Key::W:
                m_input.Set(Code::KeyW, true);
                break;
            case sf::Keyboard::Key::X:
                m_input.Set(Code::KeyX, true);
                break;
            case sf::Keyboard::Key::Y:
                m_input.Set(Code::KeyY, true);
                break;
            case sf::Keyboard::Key::Z:
                m_input.Set(Code::KeyZ, true);
                break;
            case sf::Keyboard::Key::Escape:
                m_input.Set(Code::KeyEsc, true);
                break;
            default:
                break;
            }
            break;
        }
        case sf::Event::MouseButtonReleased:
        {
            switch (m_event.mouseButton.button)
            {
            case sf::Mouse::Button::Left:
                m_input.Set(Code::MouseL, false);
                break;
            case sf::Mouse::Button::Right:
                m_input.Set(Code::MouseR, false);
                break;
            default:
                break;
            }
        }
        case sf::Event::KeyReleased:
        {
            switch (m_event.key.code)
            {
            case sf::Keyboard::Key::A:
                m_input.Set(Code::KeyA, false);
                break;
            case sf::Keyboard::Key::B:
                m_input.Set(Code::KeyB, false);
                break;
            case sf::Keyboard::Key::C:
                m_input.Set(Code::KeyC, false);
                break;
            case sf::Keyboard::Key::D:
                m_input.Set(Code::KeyD, false);
                break;
            case sf::Keyboard::Key::E:
                m_input.Set(Code::KeyE, false);
                break;
            case sf::Keyboard::Key::F:
                m_input.Set(Code::KeyF, false);
                break;
            case sf::Keyboard::Key::G:
                m_input.Set(Code::KeyG, false);
                break;
            case sf::Keyboard::Key::H:
                m_input.Set(Code::KeyH, false);
                break;
            case sf::Keyboard::Key::I:
                m_input.Set(Code::KeyI, false);
                break;
            case sf::Keyboard::Key::J:
                m_input.Set(Code::KeyJ, false);
                break;
            case sf::Keyboard::Key::K:
                m_input.Set(Code::KeyK, false);
                break;
            case sf::Keyboard::Key::L:
                m_input.Set(Code::KeyL, false);
                break;
            case sf::Keyboard::Key::M:
                m_input.Set(Code::KeyM, false);
                break;
            case sf::Keyboard::Key::N:
                m_input.Set(Code::KeyN, false);
                break;
            case sf::Keyboard::Key::O:
                m_input.Set(Code::KeyO, false);
                break;
            case sf::Keyboard::Key::P:
                m_input.Set(Code::KeyP, false);
                break;
            case sf::Keyboard::Key::Q:
                m_input.Set(Code::KeyQ, false);
                break;
            case sf::Keyboard::Key::R:
                m_input.Set(Code::KeyR, false);
                break;
            case sf::Keyboard::Key::S:
                m_input.Set(Code::KeyS, false);
                break;
            case sf::Keyboard::Key::T:
                m_input.Set(Code::KeyT, false);
                break;
            case sf::Keyboard::Key::U:
                m_input.Set(Code::KeyU, false);
                break;
            case sf::Keyboard::Key::V:
                m_input.Set(Code::KeyV, false);
                break;
            case sf::Keyboard::Key::W:
                m_input.Set(Code::KeyW, false);
                break;
            case sf::Keyboard::Key::X:
                m_input.Set(Code::KeyX, false);
                break;
            case sf::Keyboard::Key::Y:
                m_input.Set(Code::KeyY, false);
                break;
            case sf::Keyboard::Key::Z:
                m_input.Set(Code::KeyZ, false);
                break;
            case sf::Keyboard::Key::Escape:
                m_input.Set(Code::KeyEsc, false);
                break;
            default:
                break;
            }
            break;
        }
        default:
            break;
        }
    }
}
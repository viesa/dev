#include "Input.hpp"

Input::Input()
{
    Reset();
}

void Input::Reset()
{
    for (auto &e : m_events)
        e = false;
}

bool Input::Get(const Code key)
{
    return m_events[key];
}

void Input::Set(const Code key, bool newState)
{
    m_events[key] = newState;
}

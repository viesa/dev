#pragma once

#include <array>

enum Code
{
    EventQuit,
    MouseL,
    MouseR,
    KeyA,
    KeyB,
    KeyC,
    KeyD,
    KeyE,
    KeyF,
    KeyG,
    KeyH,
    KeyI,
    KeyJ,
    KeyK,
    KeyL,
    KeyM,
    KeyN,
    KeyO,
    KeyP,
    KeyQ,
    KeyR,
    KeyS,
    KeyT,
    KeyU,
    KeyV,
    KeyW,
    KeyX,
    KeyY,
    KeyZ,
    KeyEsc,
    NCodes
};

class Input
{
public:
    Input();

    void Reset();
    bool Get(const Code key);
    void Set(const Code key, bool newState);

private:
    std::array<bool, Code::NCodes> m_events;
};
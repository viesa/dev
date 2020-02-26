#pragma once

#include "Core/AppIncludes.hpp"

#include "Chat.hpp"

class AppResources
{
public:
    AppResources(class Application &app);
    ~AppResources();
    void Draw();
    void Update();

private:
    class Application &app;

private:
    //Write here
    Chat m_chat;
};
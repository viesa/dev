#pragma once

#include "Core/AppIncludes.hpp"

#include "Snake.hpp"

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
    Snake m_snake;
};
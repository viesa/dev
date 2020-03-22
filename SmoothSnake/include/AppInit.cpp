#include "AppResources.hpp"
#include "Core/Application.hpp"
#include "Core/AppIncludes.hpp"
#include <cstring>

AppResources::AppResources(Application &app)
    : app(app),
      m_snake(sf::Vector2f(100.0f, 100.0f), sf::Color::Red, app.dt, app.input, app.gfx)
{
}
#include "AppResources.hpp"
#include "Core/Application.hpp"
#include "Core/AppIncludes.hpp"

AppResources::AppResources(Application &app)
    : app(app),
      m_chat(app.gui)
{
}
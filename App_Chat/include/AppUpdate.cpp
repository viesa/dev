#include "AppResources.hpp"
#include "Core/Application.hpp"
#include "Core/AppIncludes.hpp"

void AppResources::Update()
{
    m_chat.Update(app.dt);
}
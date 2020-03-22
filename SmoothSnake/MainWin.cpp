#include "include/Core/Application.hpp"
#include "include/Core/EventMgr.hpp"

int main()
{

	Graphics m_gfx;
	Input m_input;
	Camera m_camera(m_gfx, m_input);
	GUI m_gui(m_gfx.GetRenderWindow());
	EventMgr m_eventMgr(m_gfx.GetRenderWindow(), m_input, m_gui);
	AudioMgr m_audioMgr;
	sf::Clock m_application_clock;
	sf::Time m_dt;
	Application m_application(m_gfx, m_camera, m_audioMgr, m_input, m_dt, m_gui);

	m_gfx.SetCamera(&m_camera);

	while (m_gfx.GetRenderWindow().isOpen())
	{
		m_eventMgr.PollAll();
		if (m_input.Get(Code::EventQuit) || m_input.Get(Code::KeyEsc))
			m_gfx.GetRenderWindow().close();
		m_application.Go();
		m_dt = m_application_clock.getElapsedTime();
		m_application_clock.restart();
	}
	return EXIT_SUCCESS;
}
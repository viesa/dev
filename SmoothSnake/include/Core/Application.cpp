#include "Application.hpp"
#include <stdlib.h>
#include <time.h>

Application::Application(Graphics &gfx, Camera &camera, AudioMgr &audioMgr, Input &input, sf::Time &dt, GUI &gui)
	: gfx(gfx),
	  camera(camera),
	  audioMgr(audioMgr),
	  input(input),
	  dt(dt),
	  gui(gui),
	  resources(*this),
	  tofollow(0.0f, 0.0f)
{
	srand(time(NULL));
	camera.SetToFollow(&tofollow);
}

void Application::Go()
{
	gfx.ClearFrame();
	UpdateLogic();
	RenderFrame();
	gfx.EndFrame();
}

void Application::UpdateLogic()
{
	netMgr.PollAll();
	camera.Update(dt);
	gui.Update(dt);

	resources.Update();
}

void Application::RenderFrame()
{
	resources.Draw();
	gui.Draw();
}
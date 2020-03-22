#pragma once
#include "Camera.hpp"
#include "AudioMgr.hpp"
#include "../Net/NetworkMgr.hpp"
#include "Input.hpp"
#include "GUI.hpp"
#include "../Arth/Random.hpp"
#include "../Arth/Math.hpp"
#include "../AppResources.hpp"

class Application
{
public:
	Application(Graphics &gfx, Camera &camera, AudioMgr &audioMgr, Input &input, sf::Time &dt, GUI &gui);
	void Go();

private:
	void UpdateLogic();
	void RenderFrame();

private:
	Graphics &gfx;
	Camera &camera;
	AudioMgr &audioMgr;
	NetworkMgr netMgr;
	Input &input;
	sf::Time &dt;
	GUI &gui;

	AppResources resources;
	sf::Vector2f tofollow;

public:
	friend class AppResources;
};

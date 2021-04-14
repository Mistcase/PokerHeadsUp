#pragma once
#ifndef _APPLICATION_DEFINED_
#define _APPLICATION_DEFINED_

#include "MainMenuState.h"

#include <iostream>
#include <ctime>

class Application
{
public:
	void run();

private:
	void initNetwork();
	void initWindow();
	void initStates();
	void startMainLoop();

private:
	void updateSfmlEvents();

	void updateState();
	void renderState();

private:
	StatesStack states;

	sf::RenderWindow window;
	bool active = true;
};

#endif
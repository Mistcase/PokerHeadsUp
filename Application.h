#pragma once
#ifndef _APPLICATION_DEFINED_
#define _APPLICATION_DEFINED_

#include "Types.h"
#include "MainMenuState.h"

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

	RenderWindow window;
	bool active = true;
};

#endif _APPLICATION_DEFINED_
#include "Application.h"

void Application::run()
{
	srand(static_cast<unsigned int>(time(0)));

	initNetwork();
	initWindow();
	initStates();
	startMainLoop();
}

void Application::initNetwork()
{
	if (!netboost::Initialize())
	{
		//Error
	}
}

void Application::initWindow()
{
	//if Windows
	//::ShowWindow(::GetConsoleWindow(), SW_SHOW);

	window.create(sf::VideoMode((unsigned)APPLICATION_WINDOW_SIZE.x, (unsigned)APPLICATION_WINDOW_SIZE.y), "LAN Poker Heads UP!", sf::Style::Close);
	window.setFramerateLimit(60);
}

void Application::initStates()
{
	states.push(new MainMenuState(&states));
}

void Application::startMainLoop()
{
	while (active)
	{
		updateSfmlEvents();
		updateState();
		renderState();
	}

	window.close();
}

void Application::updateSfmlEvents()
{
	sf::Event ev;
	while (window.pollEvent(ev))
	{
		if (!active)
			return;

		switch (ev.type)
		{
		case sf::Event::Closed:
			active = false;
			break;

		default:
			states.top()->updateSfmlEvent(ev);

			break;
		}
	}
}

void Application::updateState()
{
	if (!active)
		return;

	if (states.top()->requestsExit())
	{
		delete states.top();
		states.pop();
		if (states.empty())
			active = false;
		return;
	}

	static sf::Clock clock;
	int elapsedTime = clock.getElapsedTime().asMilliseconds();
	clock.restart();
	states.top()->update(static_cast<float>(elapsedTime), static_cast<sf::Vector2f>(window.mapPixelToCoords(sf::Mouse::getPosition(window))));
}

void Application::renderState()
{
	if (!active)
		return;

	window.clear(MainMenuState::BACKGROUND_COLOR);
	window.draw(*states.top());
	window.display();
}

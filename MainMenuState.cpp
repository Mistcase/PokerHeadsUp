#include "MainMenuState.h"

MainMenuState::MainMenuState(StatesStack* statesStack)
{
	this->statesStack = statesStack;

	loginBox = TextBox(sf::Vector2f(310, 290), sf::Vector2f(180, 20), sf::Color(50, 50, 50, 200), sf::Color::White, 1.f,
		ApplicationFont::GetDefaultFont());
	loginBox.setStartText("Type your nickname here...", sf::Color(150, 150, 150));

	btnConnect = Button(sf::Vector2f(350, 320), sf::Vector2f(100, 25), L"Connect", sf::Color(50, 50, 50, 200),
		sf::Color(150, 150, 150), sf::Color(20, 20, 20, 200), ApplicationFont::GetDefaultFont(), sf::Color::White, 16);

	btnCreateGame = Button(sf::Vector2f(350, 350), sf::Vector2f(100, 25), L"Start server", sf::Color(50, 50, 50, 200),
		sf::Color(150, 150, 150), sf::Color(20, 20, 20, 200), ApplicationFont::GetDefaultFont(), sf::Color::White, 16);
}

MainMenuState::~MainMenuState()
{
}

void MainMenuState::update(float deltaTime, sf::Vector2f mousePos)
{
	loginBox.update(mousePos, deltaTime);
	btnConnect.update(mousePos);
	btnCreateGame.update(mousePos);

	if (!loginBox.empty())
	{
		if (btnConnect.isReleased())
		{
			btnConnect.reset();
			statesStack->push(new GameState(statesStack, loginBox.getText(), game_state_mode::Value::JUST_CONNECT));
		}
		if (btnCreateGame.isReleased())
		{
			btnCreateGame.reset();
			statesStack->push(new GameState(statesStack, loginBox.getText(), game_state_mode::Value::OPEN_SERVER));
		}
	}
}

void MainMenuState::updateSfmlEvent(sf::Event& ev)
{
	switch (ev.type)
	{
	case sf::Event::TextEntered:
		loginBox.updateEvent(ev);
		break;
	}
}

void MainMenuState::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(loginBox, states);
	target.draw(btnConnect, states);
	target.draw(btnCreateGame, states);
}

sf::Color MainMenuState::BACKGROUND_COLOR = sf::Color(246, 246, 246);
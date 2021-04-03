#include "MainMenuState.h"

MainMenuState::MainMenuState(StatesStack* statesStack)
{
	this->statesStack = statesStack;
	initGui();

	loginBox = TextBox(Vector2f(310, 290), Vector2f(180, 20), Color(50, 50, 50, 200), Color::White, 1.f, "Enter your Nickname...");
}

MainMenuState::~MainMenuState()
{
}

void MainMenuState::update(float deltaTime, sf::Vector2f mousePos)
{
	loginBox.update(mousePos, deltaTime);
	btnConnect.update(mousePos);
	btnCreateGame.update(mousePos);
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

void MainMenuState::handleEvent(const ObsMessage& message)
{
	if (loginBox.getText().isEmpty())
		return;

	if (message.sender == &btnConnect)
	{
		statesStack->push(new GameState(statesStack, loginBox.getText(), network_mode::Value::JUST_CONNECT));
	}
	if (message.sender == &btnCreateGame)
	{
		statesStack->push(new GameState(statesStack, loginBox.getText(), network_mode::Value::OPEN_SERVER));
	}
}

void MainMenuState::initGui()
{
	//Init button prototype
	buttonProrotype.setSize(Vector2f(100, 25));
	buttonProrotype.setButtonColor(Color(50, 50, 50, 200), BTN_IDLE);
	buttonProrotype.setButtonColor(Color(150, 150, 150), BTN_HOVER);
	buttonProrotype.setButtonColor(Color(20, 20, 20, 200), BTN_PRESSED);
	buttonProrotype.setFont(ApplicationFonts::ARIAL);
	buttonProrotype.setTextColor(Color::White);
	buttonProrotype.setCharacterSize(16);

	btnConnect = Button(buttonProrotype);
	btnCreateGame = Button(buttonProrotype);

	btnConnect.setText("Connect");
	btnConnect.setPosition(Vector2f(350, 320));

	btnCreateGame.setText("Start server");
	btnCreateGame.setPosition(Vector2f(350, 350));

	btnConnect.addObserver(this);
	btnCreateGame.addObserver(this);
}

void MainMenuState::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(loginBox, states);
	target.draw(btnConnect, states);
	target.draw(btnCreateGame, states);
}

sf::Color MainMenuState::BACKGROUND_COLOR = sf::Color(246, 246, 246);
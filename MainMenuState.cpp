#include "MainMenuState.h"

MainMenuState::MainMenuState(StatesStack* statesStack)
{
	this->statesStack = statesStack;
	initGui();

	/*loginBox = TextBox(sf::Vector2f(310, 290), sf::Vector2f(180, 20), sf::Color(50, 50, 50, 200), sf::Color::White, 1.f,
		ApplicationFonts::getFont(ApplicationFonts::ARIAL));*/
	//loginBox.setStartText("Type your nickname here...", sf::Color(150, 150, 150));

	/*btnConnect = Button(sf::Vector2f(350, 320), sf::Vector2f(100, 25), L"Connect", sf::Color(50, 50, 50, 200),
		sf::Color(150, 150, 150), sf::Color(20, 20, 20, 200), ApplicationFonts::getFont(ApplicationFonts::ARIAL), sf::Color::White, 16);*/

	/*btnCreateGame = Button(sf::Vector2f(350, 350), sf::Vector2f(100, 25), L"Start server", sf::Color(50, 50, 50, 200),
		sf::Color(150, 150, 150), sf::Color(20, 20, 20, 200), ApplicationFonts::getFont(ApplicationFonts::ARIAL), sf::Color::White, 16);*/
}

MainMenuState::~MainMenuState()
{
}

void MainMenuState::update(float deltaTime, sf::Vector2f mousePos)
{
	loginBox.update(mousePos, deltaTime);
	btnConnect->update(mousePos);
	btnCreateGame.update(mousePos);

	if (!loginBox.empty())
	{
		//if (btnConnect.isReleased())
		//{
		//	//btnConnect.reset();
		//	statesStack->push(new GameState(statesStack, loginBox.getText(), game_state_mode::Value::JUST_CONNECT));
		//}
		//if (btnCreateGame.isReleased())
		//{
		//	//btnCreateGame.reset();
		//	statesStack->push(new GameState(statesStack, loginBox.getText(), game_state_mode::Value::OPEN_SERVER));
		//}
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

	btnConnect = new Button(buttonProrotype);
	btnCreateGame = Button(buttonProrotype);

	btnConnect->setText("Connect");
	btnConnect->setPosition(Vector2f(350, 320));

	btnCreateGame.setText("Start server");
	btnCreateGame.setPosition(Vector2f(350, 350));
}

void MainMenuState::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(loginBox, states);
	target.draw(*btnConnect, states);
	target.draw(btnCreateGame, states);
}

sf::Color MainMenuState::BACKGROUND_COLOR = sf::Color(246, 246, 246);
#include "GameState.h"

GameState::GameState(StatesStack* statesStack, const sf::String& nickname, game_state_mode::Value netMode)
{
	//InitGameRules();

	this->statesStack = statesStack;
	this->netMode = netMode;

	//LocalPlayer
	localPlayer.setNickname(nickname);
	localPlayer.setPlayerSlot(table_slots::BOTTOM);

	opponentPlayer.setNickname("Opponent(Not connected!)");
	opponentPlayer.setPlayerSlot(table_slots::TOP);

	netInit();
	guiInit();
	sfmlGraphicsInit();
}

GameState::~GameState()
{
	if (netMode == game_state_mode::Value::OPEN_SERVER)
	{
		tcpServer.disconnect(connectionDescriptor);
	}
	netboost::ReleaseResources();
}

void GameState::updateSfmlEvent(sf::Event& ev)
{

}

void GameState::netInit()
{
	tcpEntity = (netMode == game_state_mode::Value::OPEN_SERVER) ? reinterpret_cast<TcpEntity*>(&tcpServer) :
		(netMode == game_state_mode::Value::JUST_CONNECT) ? reinterpret_cast<TcpEntity*>(&tcpClient) :
		nullptr;


	using sf::Uint32;
	if (netMode == game_state_mode::Value::JUST_CONNECT)
	{
		//Connect to server
		connectionDescriptor = tcpClient.connect(GameState::serverAddr);
		if (connectionDescriptor == netboost::n_codes::ANY_ERROR)
		{
			//Error
		}

		sendLocalPlayerNameToOpponent();
		receiveOpponentNickname();

	}
	else if (netMode == game_state_mode::Value::OPEN_SERVER)
	{
		//Open server
		tcpServer = TcpServer(GameState::serverAddr);

		//Open connection for opponent
		connectionDescriptor = tcpServer.openConnection();
		if (connectionDescriptor == netboost::n_codes::ANY_ERROR)
		{
			//Error
		}

		std::thread([&]()
		{
				//Waiting for connection...
			if (!tcpServer.accept(connectionDescriptor))
			{
					//Error
			}
			std::cout << "Accept -> thread: " << std::this_thread::get_id() << std::endl;

			receiveOpponentNickname();
			sendLocalPlayerNameToOpponent();

		}).detach();

			//Show message (OK!)
	}
	else
	{
		//Error
	}
}

void GameState::guiInit()
{
	for (auto& btn : buttons)
	{
		btn = Button(sf::Vector2f(), GAMESTATE_BUTTON_SIZE, L"", sf::Color(50, 50, 50, 200),
			sf::Color(150, 150, 150), sf::Color(20, 20, 20, 200), ApplicationFont::GetDefaultFont(), sf::Color::White, 16);

		btn.active = false;
	}

	buttons[BTN_CHECK].setText(L"CHECK");
	buttons[BTN_CALL].setText(L"CALL");
	buttons[BTN_BET].setText(L"BET");
	buttons[BTN_RAISE].setText(L"RAISE");
	buttons[BTN_FOLD].setText(L"FOLD");

	/*buttons[BTN_CHECK].setPosition(findPositionForButton());
	buttons[BTN_CHECK].active = true;

	buttons[BTN_CALL].setPosition(findPositionForButton());
	buttons[BTN_CALL].active = true;

	buttons[BTN_FOLD].setPosition(findPositionForButton());
	buttons[BTN_FOLD].active = true;*/
}

void GameState::sfmlGraphicsInit()
{
	//Graphics
	sf::Image backImg;
	if (!backImg.loadFromFile("back.png"))
	{
		//Handle error
		std::cout << "Error\n";
	}
	backImg.createMaskFromColor(sf::Color::White);
	backgroundTexture.loadFromImage(backImg);
	background.setTexture(&backgroundTexture);
	background.setSize(sf::Vector2f(820, 392));
	background.setPosition(APPLICATION_WINDOW_SIZE.x / 2 - background.getSize().x / 2,
		APPLICATION_WINDOW_SIZE.y / 2 - background.getSize().y / 2);

	pot.setFont(*ApplicationFont::GetDefaultFont());
	pot.setCharacterSize(16);
	pot.setString("Pot: 0");
}

void GameState::receiveOpponentNickname()
{
	Packet packet = tcpEntity->readPacket(connectionDescriptor);
	opponentPlayer.setNickname(static_cast<const char*>(packet.getData()));
}

void GameState::sendLocalPlayerNameToOpponent()
{
	auto nickname = localPlayer.getNickname();
	tcpEntity->write(connectionDescriptor, Packet(string(nickname.toAnsiString() + "\0").c_str(), nickname.getSize() + 1));
}

sf::Vector2f GameState::findPositionForButton()
{
	const sf::Vector2f OFFSET = GAMESTATE_BUTTON_SIZE + sf::Vector2f(5.f, 0);

	size_t activeButtonsCounter = 0;
	for (auto& btn : buttons)
	{
		if (btn.active)
			activeButtonsCounter++;
	}

	sf::Vector2f result = APPLICATION_WINDOW_SIZE - sf::Vector2f((OFFSET.x + 5.f) * (activeButtonsCounter + 1), OFFSET.y + 5.f);

	return result;
}

void GameState::update(float deltaTime, sf::Vector2f mousePos)
{
	auto potParams = pot.getGlobalBounds();
	pot.setPosition(APPLICATION_WINDOW_SIZE.x / 2 - potParams.width / 2, APPLICATION_WINDOW_SIZE.y / 2 - potParams.height / 2);

	//UpdateGui
	for (auto& btn : buttons)
		btn.update(mousePos);
}

void GameState::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(background, states);
	target.draw(pot, states);
	target.draw(localPlayer, states);
	target.draw(opponentPlayer, states);

	for (auto& btn : buttons)
	{
		target.draw(btn, states);
	}
}

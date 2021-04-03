#include "GameState.h"

GameState::GameState(StatesStack* statesStack, const sf::String& nickname, network_mode::Value netMode)
{
	//InitGameRules();

	this->statesStack = statesStack;
	this->netMode = netMode;

	//Players
	localPlayer.setNickname(nickname);
	localPlayer.setPlayerSlot(table_slots::BOTTOM);

	opponentPlayer.setNickname("Opponent(Not connected!)");
	opponentPlayer.setPlayerSlot(table_slots::TOP);

	netInit();
	guiInit();
	sfmlGraphicsInit();

	startGame();
}

GameState::~GameState()
{
	if (netMode == network_mode::Value::OPEN_SERVER)
	{
		tcpServer.disconnect(connectionDescriptor);
	}
}

void GameState::updateSfmlEvent(sf::Event& ev)
{

}

void GameState::handleEvent(const ObsMessage & message)
{
	if (message.sender == &localPlayer)
	{
		std::cout << message.args << std::endl;
	}
	/*if (message.sender == &localPlayer)
	{
		std::cout << "LocalPlayer\n";
	}*/
}

void GameState::notifyObservers(const ObsMessageString & messageString)
{
	ObsMessage message(this, messageString);
	for (auto& obs : observers)
		obs->handleEvent(message);
}

void GameState::netInit()
{
	//Determine network role
	tcpEntity = (netMode == network_mode::Value::OPEN_SERVER) ? reinterpret_cast<TcpEntity*>(&tcpServer) :
		(netMode == network_mode::Value::JUST_CONNECT) ? reinterpret_cast<TcpEntity*>(&tcpClient) :
		nullptr;


	using sf::Uint32;
	if (netMode == network_mode::Value::JUST_CONNECT)
	{
		//Connect to server
		connectionDescriptor = tcpClient.connect(GameState::serverAddr);
		if (connectionDescriptor == netboost::n_codes::ANY_ERROR)
		{
			//Error
		}

		sendLocalPlayerNameToOpponent();
		receiveOpponentNickname();
		gameIsActive = true;
	}
	else if (netMode == network_mode::Value::OPEN_SERVER)
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
				gameIsActive = true;
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
	//Init button prototype
	buttonPrototype.setSize(GAMESTATE_BUTTON_SIZE);
	buttonPrototype.setButtonColor(Color(50, 50, 50, 200), BTN_IDLE);
	buttonPrototype.setButtonColor(Color(150, 150, 150), BTN_HOVER);
	buttonPrototype.setButtonColor(Color(20, 20, 20, 200), BTN_PRESSED);
	buttonPrototype.setFont(ApplicationFonts::ARIAL);
	buttonPrototype.setTextColor(Color::White);
	buttonPrototype.setCharacterSize(16);

	//Create buttons
	for (auto& btn : buttons)
	{
		btn = Button(buttonPrototype);
		btn.active = false;
	}

	buttons[BTN_CHECK].setText(L"CHECK");
	buttons[BTN_CALL].setText(L"CALL");
	buttons[BTN_BET].setText(L"BET");
	buttons[BTN_RAISE].setText(L"RAISE");
	buttons[BTN_FOLD].setText(L"FOLD");

	buttons[BTN_CHECK].setPosition(findPositionForButton());
	buttons[BTN_CHECK].active = true;

	buttons[BTN_CALL].setPosition(findPositionForButton());
	buttons[BTN_CALL].active = true;

	buttons[BTN_FOLD].setPosition(findPositionForButton());
	buttons[BTN_FOLD].active = true;
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

	pot.setFont(ApplicationFonts::getFont(ApplicationFonts::ARIAL));
	pot.setCharacterSize(16);
	pot.setString("Pot: 0");
}

void GameState::startGame()
{
	std::thread([&]() {
		//Wait for opponent
		while (!gameIsActive)
			std::this_thread::sleep_for(std::chrono::milliseconds(200));

		RingPlayersQueue playersBlindQueue;
		playersBlindQueue.push(&localPlayer);
		playersBlindQueue.push(&opponentPlayer);

		while (gameIsActive)
		{
			pokerHand = PokerHand(this, playersBlindQueue);
			pokerHand.addObserver(this);
			pokerHand.play();

			playersBlindQueue.next();
		}

	}).detach();
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

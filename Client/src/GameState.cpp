#include "GameState.h"

GameState::GameState(StatesStack *statesStack, const sf::String &localPlayerName, network_mode::Value netMode)
{
	this->statesStack = statesStack;

	if (!playersInit(localPlayerName) || !netInit(netMode) || !guiInit() || !sfmlGraphicsInit())
		exitState();
}

GameState::~GameState()
{
	netClient.disconnect();
}

void GameState::updateSfmlEvent(sf::Event &ev)
{
}

void GameState::handleEvent(const EventMessage &message)
{
	// if (message.sender == &pokerGameServer)
	// {
	// 	//Handle Game Server Event
	// 	if (Notifications::GetNotificationAction(message.params) == "MakeDescision")
	// 	{
	// 		auto currentPlayer = pokerGameServer.getCurrentPlayer();
	// 		if (currentPlayer == &localPlayer)
	// 		{
	// 			showButtons(Notifications::GetNotificationArgs(message.params));
	// 		}
	// 		else if (currentPlayer == &opponentPlayer)
	// 		{
	// 			tcpClient->write(connectionDescriptor, Packet(message.params.c_str(), message.params.size()));
	// 		}
	// 		else
	// 		{
	// 			//Error
	// 		}
	// 	}
	// }
	if (message.sender == &netClient)
	{
		handleNetworkEvent(message.params);
	}
	else
	{
		handleGuiEvent(message);
	}
}

void GameState::notifyObservers(const EventMessageString &messageString)
{
	EventMessage message(this, messageString);
	for (auto &obs : observers)
		obs->handleEvent(message);
}

bool GameState::playersInit(const String &localPlayerName)
{
	localPlayer.setNickname(localPlayerName);
	localPlayer.setPlayerSlot(table_slots::BOTTOM);
	opponentPlayer.setNickname("Opponent(Not connected!)");
	opponentPlayer.setPlayerSlot(table_slots::TOP);
	return true;
}

bool GameState::netInit(network_mode::Value netMode)
{
	if (!netClient.connect(SERVER_ADDRESS))
	{
		SfmlMessageBox("Connection to server failed!", "Connection error").show();
		return false;
	}

	netClient.sendMessage(Notifications::CreateNofiticationMessage("ConnectedPlayerName", localPlayer.getNickname()));
	return true;
}

bool GameState::guiInit()
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
	for (auto &btn : buttons)
	{
		btn = Button(buttonPrototype);
		btn.addObserver(this);
		btn.active = false;
	}

	buttons[BTN_CHECK].setText(L"CHECK");
	buttons[BTN_CALL].setText(L"CALL");
	buttons[BTN_BET].setText(L"BET");
	buttons[BTN_RAISE].setText(L"RAISE");
	buttons[BTN_FOLD].setText(L"FOLD");

	return true;
}

bool GameState::sfmlGraphicsInit()
{
	//Graphics
	sf::Image backImg;
	if (!backImg.loadFromFile(Resources::path("back.png")))
	{
		SfmlMessageBox("Cannot load background!", "Error").show();
		return false;
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

	tableButton.setTableSlot(table_slots::BOTTOM);

	return true;
}

// void GameState::startServer()
// {
// 	std::thread([&]() {
// 		RingPlayersQueue playersBlindQueue;
// 		playersBlindQueue.push(&localPlayer);
// 		playersBlindQueue.push(&opponentPlayer);

// 		bool gameIsActive = true;
// 		while (gameIsActive)
// 		{
// 			pokerGameServer = PokerServer(playersBlindQueue);
// 			this->addObserver(&pokerGameServer);
// 			pokerGameServer.addObserver(this);
// 			pokerGameServer.start();

// 			playersBlindQueue.next();
// 		}
// 	}).detach();
// }

void GameState::updateNetwork()
{
	if (!netClient.hasMessage())
		return;
	
	AnsiString message = netClient.receiveMessage();
	handleEvent(EventMessage(&netClient, message));
}

void GameState::updateGui(const Vector2f &mousePos)
{
	//UpdateGui
	for (auto &btn : buttons)
		btn.update(mousePos);
}

void GameState::updateGraphicsEntities()
{
	auto potParams = pot.getGlobalBounds();
	pot.setPosition(APPLICATION_WINDOW_SIZE.x / 2 - potParams.width / 2, APPLICATION_WINDOW_SIZE.y / 2 - potParams.height / 2);
}

void GameState::handleNetworkEvent(const EventMessageString &message)
{
	auto showButtons = [&](const AnsiString &buttonsNotation) {
		auto findPositionForButton = [&]() {
			const sf::Vector2f OFFSET = GAMESTATE_BUTTON_SIZE + sf::Vector2f(5.f, 0);

			size_t activeButtonsCounter = 0;
			for (auto &btn : buttons)
			{
				if (btn.active)
					activeButtonsCounter++;
			}

			sf::Vector2f result = APPLICATION_WINDOW_SIZE - sf::Vector2f((OFFSET.x + 5.f) * (activeButtonsCounter + 1), OFFSET.y + 5.f);

			return result;
		};

		for (auto &btn : buttons)
			btn.active = false;

		if (buttonsNotation == "None")
			return;

		if (buttonsNotation == "CHECK_BET")
		{
			buttons[BTN_CHECK].setPosition(findPositionForButton());
			buttons[BTN_CHECK].active = true;

			buttons[BTN_BET].setPosition(findPositionForButton());
			buttons[BTN_BET].active = true;
		}
		else if (buttonsNotation == "CALL_RAISE_FOLD")
		{
			buttons[BTN_CALL].setPosition(findPositionForButton());
			buttons[BTN_CALL].active = true;

			buttons[BTN_RAISE].setPosition(findPositionForButton());
			buttons[BTN_RAISE].active = true;

			buttons[BTN_FOLD].setPosition(findPositionForButton());
			buttons[BTN_FOLD].active = true;
		}
		else
		{
			//Error
		}
	};
	auto toInt = [](const AnsiString& str){return atoi(str.c_str());};

	if (Notifications::GetNotificationAction(message) == "PlayersInfo")
	{
		//Occurs when a player connects. Players information is received by each client
		int playersCount = toInt(Notifications::GetNotificationNamedArg(message, "count"));

		//This is heads up version
		if (playersCount != 2) //Error
			SfmlMessageBox("Game version: HeadUp! Player count = " + std::to_string(playersCount), "Network event handling error");

		vector<AnsiString> players;
		for (int i = 0; i < playersCount; i++)
		{
			players[i] = Notifications::GetNotificationNamedArg(message, "player" + std::to_string(i));
			if (players[i] != localPlayer.getNickname())
				opponentPlayer.setNickname(players[i]);
		}

	}
	else if (Notifications::GetNotificationAction(message) == "MakeDescision")
	{
		showButtons(Notifications::GetNotificationArgs(message));
	}
	else if (Notifications::GetNotificationAction(message) == "ClientAnswer")
	{
		notifyObservers(message);
	}
}

void GameState::handleGuiEvent(const EventMessage &message)
{
	//Handle buttonEvent
	AnsiString buttonText = static_cast<Button*>(message.sender)->getText().toAnsiString();
	AnsiString answer = Notifications::CreateNofiticationMessage("ClientAnswer", buttonText);

	//tcpClient->write(connectionDescriptor, Packet(answer.c_str(), answer.size()));
}

void GameState::update(float deltaTime, const Vector2f &mousePos)
{
	updateGraphicsEntities();
	updateGui(mousePos);
	updateNetwork();

	//std::cout << pokerGameServer.getPlayersCount() << std::endl;
}

void GameState::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(background, states);
	target.draw(pot, states);
	target.draw(localPlayer, states);
	target.draw(opponentPlayer, states);

	target.draw(tableButton, states);

	for (auto &btn : buttons)
	{
		target.draw(btn, states);
	}
}

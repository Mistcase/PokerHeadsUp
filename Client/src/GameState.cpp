#include "GameState.h"

GameState::GameState(StatesStack *statesStack, const sf::String &localPlayerName)
{
	this->statesStack = statesStack;

	if (!playersInit(localPlayerName) || !netInit() || !guiInit() || !sfmlGraphicsInit())
		exitState();
}

GameState::~GameState()
{
	netClient.disconnect();
}

void GameState::updateSfmlEvent(sf::Event &ev)
{
	raiseTextBox.updateEvent(ev);
}

void GameState::handleEvent(const EventMessage &message)
{
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

bool GameState::netInit()
{
	if (!netClient.connect(SERVER_ADDRESS))
	{
		SfmlMessageBox("Connection to server failed!", "Connection error").show();
		return false;
	}

	netClient.sendMessage(Notifications::CreateNofiticationMessage("NewConnection", 
	{
		"Name:" + localPlayer.getNickname(),
		"",
	}));
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

	raiseTextBox = TextBox(Vector2f(680, 520), Vector2f(100, 20), Color(128, 128, 128), Color::Black, 1.f, "Bet/Raise value",
	ApplicationFonts::ARIAL, Color::Black);

	return true;
}

bool GameState::sfmlGraphicsInit()
{
	//Graphics
	sf::Image backImg;
	if (!backImg.loadFromFile(Resources::path("back.png")) || !tableButton.loadFromFile(Resources::path("dealer_button.png")))
	{
		SfmlMessageBox("Cannot load resource!", "Error").show();
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

void GameState::updateNetwork()
{
	if (!netClient.active())
	{
		std::cout << "Server is not available. Exit the program!\n";
		netClient.disconnect();
		exit(0);
	}

	if (!netClient.hasMessage())
		return;

	AnsiString message = netClient.receiveMessage() + "\0";
	handleEvent(EventMessage(&netClient, message));
}

void GameState::updateGui(float deltaTime, const Vector2f &mousePos)
{
	//UpdateGui
	for (auto &btn : buttons)
		btn.update(mousePos);
	raiseTextBox.update(mousePos, deltaTime);
}

void GameState::updateGraphicsEntities()
{
	auto potParams = pot.getGlobalBounds();
	pot.setPosition(APPLICATION_WINDOW_SIZE.x / 2 - potParams.width / 2, APPLICATION_WINDOW_SIZE.y / 2 - potParams.height / 2);
}

void GameState::handleNetworkEvent(const EventMessageString &message)
{
	std::cout << "Update message -> " << message << std::endl;

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
		else if (buttonsNotation == "CHECK_RAISE")
		{
			buttons[BTN_CHECK].setPosition(findPositionForButton());
			buttons[BTN_CHECK].active = true;

			buttons[BTN_RAISE].setPosition(findPositionForButton());
			buttons[BTN_RAISE].active = true;
		}
		else
		{
			//Error
		}
	};
	auto toInt = [](const AnsiString &str) { return atoi(str.c_str()); };

	AnsiString action = Notifications::GetNotificationAction(message);
	if (action == "PlayersInfo")
	{
		//Occurs when a player connects. Players information is received by each client
		int playersCount = toInt(Notifications::GetNotificationNamedArg(message, "PlayersCount"));

		vector<AnsiString> players(playersCount);
		for (int i = 0; i < playersCount; i++)
		{
			players[i] = Notifications::GetNotificationNamedArg(message, "player" + std::to_string(i));
			if (players[i] != localPlayer.getNickname())
			{
				opponentPlayer.setNickname(players[i]);
				std::cout << "Opponent nickname: " << players[i] << std::endl;
			}
		}
	}
	else if (action == "StartGame")
	{
		std::cout << "Game Started!\n";
		//This is heads up version
		// int playersCount = toInt(Notifications::GetNotificationNamedArg(message, "PlayersCount"));
		// if (playersCount != 2) //Error
		// 	SfmlMessageBox("Game version: HeadUp! Player count = " + std::to_string(playersCount), "Network event handling error").show();
	}
	else if (action == "StartHand")
	{
		AnsiString player = Notifications::GetNotificationNamedArg(message, "button");
		if (player == localPlayer.getNickname())
		{
			tableButton.setTableSlot(table_slots::BOTTOM);
		}
		else if (player == opponentPlayer.getNickname())
		{
			tableButton.setTableSlot(table_slots::TOP);
		}
		else
		{
			//Error
			SfmlMessageBox("Button player is unknown: " + player, "Exception!").show();
		}

		AnsiString sbp = Notifications::GetNotificationNamedArg(message, "sbp"),
				   bbp = Notifications::GetNotificationNamedArg(message, "bbp"),
				   sbv = Notifications::GetNotificationNamedArg(message, "sbv");

		int smallBlind = atoi(sbv.c_str());
		
		Player* sbPlayer = getPlayer(sbp);
		Player* bbPlayer = getPlayer(bbp);

		sbPlayer->makeBet(smallBlind);
		bbPlayer->makeBet(2 * smallBlind);
	}
	else if (action == "MakeDescision")
	{
		if (Notifications::GetNotificationNamedArg(message, "Player") == localPlayer.getNickname())
			showButtons(Notifications::GetNotificationNamedArg(message, "ButtonMode"));
	}
	else if (action == "ClientAnswer")
	{
		notifyObservers(message);
	}
	else if (action == "HideGui")
	{
		showButtons("None");
	}
	else if (action == "TableInfo")
	{
		Balance playerBalance = atoi(Notifications::GetNotificationNamedArg(message, "Balance").c_str());
		Balance betValue = atoi(Notifications::GetNotificationNamedArg(message, "Bet").c_str());

		Player* player = getPlayer(Notifications::GetNotificationNamedArg(message, "Player"));
		player->setBalance(playerBalance);
		player->setBet(betValue);
	}
	else if (action == "Cards")
	{
		Deck::CardsValuesGraphicsId val[2];
		Deck::CardsSuitsGraphicsId suit[2];
		for (int i = 0; i < 2; i++)
		{
			val[i] = static_cast<Deck::CardsValuesGraphicsId>(atoi(Notifications::GetNotificationNamedArg(message, "CardValue" + to_string(i)).c_str()) - 1);
			suit[i] = static_cast<Deck::CardsSuitsGraphicsId>(atoi(Notifications::GetNotificationNamedArg(message, "CardSuit" + to_string(i)).c_str()) - 1);
		}
		localPlayer.setCards(Card(val[0], suit[0]), Card(val[1], suit[1]));
	}
	else
	{
		std::cout << "Unknown message: " << message << std::endl;
	}
}

Player *GameState::getPlayer(const AnsiString &nickname) const
{
	if (localPlayer.getNickname() == nickname)
		return (Player *)(&localPlayer);
	else if (opponentPlayer.getNickname() == nickname)
		return (Player *)(&opponentPlayer);
	else
		return nullptr;
}

void GameState::handleGuiEvent(const EventMessage &message)
{
	//Handle buttonEvent
	AnsiString buttonText = static_cast<Button *>(message.sender)->getText().toAnsiString();

	AnsiString args = "Action:" + buttonText + "|";

	if (buttonText == "RAISE")
		args += "Value:" + raiseTextBox.getText() + "|";
	
	netClient.sendMessage(Notifications::CreateNofiticationMessage("ClientDescision", args));
}

void GameState::update(float deltaTime, const Vector2f &mousePos)
{
	updateGraphicsEntities();
	updateGui(deltaTime, mousePos);
	updateNetwork();

	localPlayer.update();
	opponentPlayer.update();

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
	target.draw(raiseTextBox);
}

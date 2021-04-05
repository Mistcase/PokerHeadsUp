#include "GameState.h"

GameState::GameState(StatesStack* statesStack, const sf::String& localPlayerName, network_mode::Value netMode)
{
	this->statesStack = statesStack;

	playersInit(localPlayerName);
	netInit(netMode);
	guiInit();
	sfmlGraphicsInit();
}

GameState::~GameState()
{
	delete tcpEntity;
}

void GameState::updateSfmlEvent(sf::Event& ev)
{

}

void GameState::handleEvent(const EventMessage& message)
{
	auto showButtons = [&](const string& buttonsNotation)
	{
		auto findPositionForButton = [&]()
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
		};

		for (auto& btn : buttons)
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

	if (message.sender == &pokerGameServer)
	{
		//Handle Game Server Event
		if (Notifications::GetNotificationAction(message.args) == "MakeDescision")
		{
			auto currentPlayer = pokerGameServer.getCurrentPlayer();
			if (currentPlayer == &localPlayer)
			{
				showButtons(Notifications::GetNotificationArgs(message.args));
			}
			else if (currentPlayer == &opponentPlayer)
			{
				tcpEntity->write(connectionDescriptor, Packet(message.args.c_str(), message.args.size()));
			}
			else
			{
				//Error
			}
		}
		else if (Notifications::GetNotificationAction(message.args) == "ReplyingToClientAction")
		{
			auto answer = Notifications::GetNotificationArgs(message.args);
			if (answer == "Ok")
			{
				showButtons("None");
				tcpEntity->write(connectionDescriptor, Packet(message.args.c_str(), message.args.size()));
			}
			else if (answer == "Fail")
			{
				//std::cout << "FailMessage!\n";
			}
			else
			{
				//Error
			}
		}
	}
	else if (message.sender == tcpEntity)
	{
		auto action = Notifications::GetNotificationAction(message.args);

		//Handle Network Message
		if (action == "MakeDescision")
		{
			showButtons(Notifications::GetNotificationArgs(message.args));
		}
		else if (action == "ClientAnswer")
		{
			notifyObservers(message.args);
		}
		else if (action == "ReplyingToClientAction")
		{
			//Server reacted to localClient action
			if (Notifications::GetNotificationArgs(message.args) == "Ok")
				showButtons("None");
		}
		
	}
	else
	{
		//Handle buttonEvent
		string buttonText = static_cast<Button*>(message.sender)->getText().toAnsiString();
		string answer = Notifications::CreateNofiticationMessage("ClientAnswer", buttonText);

		tcpEntity->write(connectionDescriptor, Packet(answer.c_str(), answer.size()));
	}
}

void GameState::notifyObservers(const EventMessageString& messageString)
{
	EventMessage message(this, messageString);
	for (auto& obs : observers)
		obs->handleEvent(message);
}

void GameState::playersInit(const String& localPlayerName)
{
	localPlayer.setNickname(localPlayerName);
	localPlayer.setPlayerSlot(table_slots::BOTTOM);
	opponentPlayer.setNickname("Opponent(Not connected!)");
	opponentPlayer.setPlayerSlot(table_slots::TOP);
}

void GameState::netInit(network_mode::Value netMode)
{
	auto receiveOpponentNickname = [&]()
	{
		this->opponentPlayer.setNickname(static_cast<const char*>(tcpEntity->readPacket(connectionDescriptor).getData()));
	};
	auto sendLocalPlayerName = [&]()
	{
		auto nickname = localPlayer.getNickname();
		tcpEntity->write(connectionDescriptor, Packet(string(nickname.toAnsiString() + "\0").c_str(), nickname.getSize() + 1));
	};
	auto enableNetworkUpdating = [&]()
	{
		updateNetwork = [&]()
		{
			if (tcpEntity->readable(connectionDescriptor))
			{
				//std::cout << "PokerGameServer: " << &pokerGameServer << std::endl;
				handleEvent(EventMessage(tcpEntity, EventMessageString(static_cast<const char*>(tcpEntity->readPacket(connectionDescriptor).getData()))));
			}
		};
	};
	auto openTcpServerAsync = [&]()
	{
		connectionDescriptor = static_cast<TcpServer*>(tcpEntity)->openConnection();
		std::thread([this, receiveOpponentNickname, sendLocalPlayerName, enableNetworkUpdating]()
			{
				//Waiting for connection...
				if (!static_cast<TcpServer*>(tcpEntity)->accept(connectionDescriptor))
				{
					//Error
				}
				std::cout << "Accept -> thread: " << std::this_thread::get_id() << std::endl;

				receiveOpponentNickname();
				sendLocalPlayerName();
				startServer();
				enableNetworkUpdating();

			}).detach();
	};
	auto connectToTcpServer = [&]()
	{
		connectionDescriptor = static_cast<TcpClient*>(tcpEntity)->connect(GameState::serverAddr);

		sendLocalPlayerName();
		receiveOpponentNickname();
		enableNetworkUpdating();
	};

	switch (netMode)
	{
	case network_mode::Value::OPEN_SERVER:
		tcpEntity = new TcpServer(serverAddr);
		openTcpServerAsync();
		break;

	case network_mode::Value::JUST_CONNECT:
		tcpEntity = new TcpClient();
		connectToTcpServer();
		break;

	default:
		//Error
		break;
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
		btn.addObserver(this);
		btn.active = false;
	}

	buttons[BTN_CHECK].setText(L"CHECK");
	buttons[BTN_CALL].setText(L"CALL");
	buttons[BTN_BET].setText(L"BET");
	buttons[BTN_RAISE].setText(L"RAISE");
	buttons[BTN_FOLD].setText(L"FOLD");
}

void GameState::sfmlGraphicsInit()
{
	//Graphics
	sf::Image backImg;
	if (!backImg.loadFromFile("back.png"))
	{
		//Handle error
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
}

void GameState::startServer()
{
	std::thread([&]()
		{
			RingPlayersQueue playersBlindQueue;
			playersBlindQueue.push(&localPlayer);
			playersBlindQueue.push(&opponentPlayer);

			bool gameIsActive = true;
			while (gameIsActive)
			{
				pokerGameServer = PokerServer(playersBlindQueue);
				this->addObserver(&pokerGameServer);
				pokerGameServer.addObserver(this);
				pokerGameServer.start();

				playersBlindQueue.next();
			}

		}).detach();
}

void GameState::updateGui(const Vector2f& mousePos)
{
	//UpdateGui
	for (auto& btn : buttons)
		btn.update(mousePos);
}

void GameState::updateGraphicsEntities()
{
	auto potParams = pot.getGlobalBounds();
	pot.setPosition(APPLICATION_WINDOW_SIZE.x / 2 - potParams.width / 2, APPLICATION_WINDOW_SIZE.y / 2 - potParams.height / 2);
}


void GameState::update(float deltaTime, const Vector2f& mousePos)
{
	updateGraphicsEntities();
	updateGui(mousePos);
	updateNetwork();

	//std::cout << pokerGameServer.getPlayersCount() << std::endl;
}

void GameState::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(background, states);
	target.draw(pot, states);
	target.draw(localPlayer, states);
	target.draw(opponentPlayer, states);

	target.draw(tableButton, states);

	for (auto& btn : buttons)
	{
		target.draw(btn, states);
	}
}


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
		try
		{
			Command::Create(message.params, this)->execute();
		}
		catch (std::exception& ex)
		{
			std::cout << ex.what() << std::endl;
		}
	}
	else
	{
		handleGuiEvent(message);
	}
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

	netClient.sendMessage(CmdNewConnection(this).str());
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

	//Board Cards
	constexpr float CARDS_DISTANCE = 5.f;
	Vector2f cardsShape = Vector2f(5 * GAMESTATE_CARD_SIZE.x + 4 * CARDS_DISTANCE, GAMESTATE_CARD_SIZE.y);
	Vector2f currentCardPoint = Vector2f(APPLICATION_WINDOW_SIZE.x / 2 - cardsShape.x / 2, APPLICATION_WINDOW_SIZE.y / 2 - cardsShape.y / 2);
	for (int i = 0; i < BOARD_CARDS_COUNT; i++)
	{
		boardCards[i].setFillColor(Color::Transparent);
		//boardCards[i].setOutlineColor(Color::White);
		//boardCards[i].setOutlineThickness(1.f);
		boardCards[i].setSize(GAMESTATE_CARD_SIZE);
		boardCards[i].setPosition(currentCardPoint);
		currentCardPoint.x += GAMESTATE_CARD_SIZE.x + CARDS_DISTANCE;
	}

	potPosition = MultiplyVector(APPLICATION_WINDOW_SIZE, 0.5f);
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
	pot.setPosition(potPosition.x - potParams.width / 2, potPosition.y - potParams.height / 2);
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

	if (buttonText == "BET" || buttonText == "RAISE")
		args += "Value:" + raiseTextBox.getText() + "|";
	

	netClient.sendMessage(CmdPlayerDecisionRequest(args).str());
}

void GameState::showButtons(const AnsiString & buttonsNotation)
{
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
		SfmlMessageBox("Unknown buttons notation: " + buttonsNotation, "Error").show();
	}
};

void GameState::update(float deltaTime, const Vector2f &mousePos)
{
	updateGraphicsEntities();
	updateGui(deltaTime, mousePos);
	updateNetwork();

	localPlayer.update();
	opponentPlayer.update();
}

void GameState::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(background, states);

	for (auto& card : boardCards)
		target.draw(card, states);

	target.draw(pot, states);
	target.draw(tableButton, states);
	target.draw(localPlayer, states);
	target.draw(opponentPlayer, states);

	for (auto &btn : buttons)
		target.draw(btn, states);

	target.draw(raiseTextBox);
}




//-------------------------------Commands-------------------------------
const AnsiString & GameState::Command::str() const
{
	return cmd;
}

GameState::ptr_command GameState::Command::Create(const AnsiString & cmd, GameState* gs)
{
	AnsiString action = Notifications::GetNotificationAction(cmd);

	if (action == "PlayersInfo")
		return ptr_command(new CmdSetPlayers(cmd, gs));
	if (action == "StartGame")
		return ptr_command(new CmdStartGame(cmd, gs));
	if (action == "StartHand")
		return ptr_command(new CmdStartHand(cmd, gs));
	if (action == "Cards")
		return ptr_command(new CmdSetCards(cmd, gs));
	if (action == "TableInfo")
		return ptr_command(new CmdSetTable(cmd, gs));
	if (action == "MakeDecision")
		return ptr_command(new CmdPlayerDecisionRequest(cmd, gs));
	if (action == "HideGui")
		return ptr_command(new CmdHideGui(gs));
	if (action == "OpenBoardCards")
		return ptr_command(new CmdOpenBoardCards(cmd, gs));
	if (action == "SetWinners")
		return ptr_command(new CmdSetWinners(cmd, gs));

	throw std::exception(AnsiString("Unknown command was received from server: " + action).c_str());
}

void GameState::CmdNewConnection::execute()
{
	throw std::exception("Cannot execute command -> CmdNewConnection");
}

void GameState::CmdSetPlayers::execute()
{
	//Occurs when a player connects. Players information is received by each client
	int playersCount = ToInt(Notifications::GetNotificationNamedArg(cmd, "PlayersCount"));

	vector<AnsiString> players(playersCount);
	for (int i = 0; i < playersCount; i++)
	{
		players[i] = Notifications::GetNotificationNamedArg(cmd, "player" + std::to_string(i));
		if (players[i] != gameState->localPlayer.getNickname())
			gameState->opponentPlayer.setNickname(players[i]);
	}
}

void GameState::CmdStartGame::execute()
{

}

void GameState::CmdStartHand::execute()
{
	gameState->potPosition = MultiplyVector(APPLICATION_WINDOW_SIZE, 0.5f);

	for (auto& boardCard : gameState->boardCards)
		boardCard.setFillColor(Color::Transparent);

	AnsiString player = Notifications::GetNotificationNamedArg(cmd, "button");
	if (player == gameState->localPlayer.getNickname())
		gameState->tableButton.setTableSlot(table_slots::BOTTOM);
	else if (player == gameState->opponentPlayer.getNickname())
		gameState->tableButton.setTableSlot(table_slots::TOP);
	else
		SfmlMessageBox("Button player is unknown: " + player, "Exception!").show();

	AnsiString sbp = Notifications::GetNotificationNamedArg(cmd, "sbp"),
		bbp = Notifications::GetNotificationNamedArg(cmd, "bbp"),
		sbv = Notifications::GetNotificationNamedArg(cmd, "sbv");

	int smallBlind = ToInt(sbv);

	Player* sbPlayer = gameState->getPlayer(sbp);
	Player* bbPlayer = gameState->getPlayer(bbp);

	sbPlayer->setBet(smallBlind);
	bbPlayer->setBet(2 * smallBlind);
}

void GameState::CmdPlayerDecisionRequest::execute()
{
	if (Notifications::GetNotificationNamedArg(cmd, "Player") == gameState->localPlayer.getNickname())
		gameState->showButtons(Notifications::GetNotificationNamedArg(cmd, "ButtonMode"));
}

void GameState::CmdHideGui::execute()
{
	gameState->showButtons("None");
}

void GameState::CmdSetTable::execute()
{
	Balance currentMaxBet = ToInt(Notifications::GetNotificationNamedArg(cmd, "CurrentMaxBet"));
	Balance pot = ToInt(Notifications::GetNotificationNamedArg(cmd, "Pot"));
	Balance playersCount = ToInt(Notifications::GetNotificationNamedArg(cmd, "PlayersCount"));

	gameState->pot.setString("Pot: " + to_string(pot));

	for (int i = 0; i < playersCount; i++)
	{
		AnsiString strIndex = to_string(i);
		Player* player = gameState->getPlayer(Notifications::GetNotificationNamedArg(cmd, "Player" + strIndex));
		player->setBalance(ToInt(Notifications::GetNotificationNamedArg(cmd, "Balance" + strIndex)));
		player->setBet(ToInt(Notifications::GetNotificationNamedArg(cmd, "Bet" + strIndex)));
	}
}

void GameState::CmdSetCards::execute()
{
	Deck::CardsValuesGraphicsId val[2];
	Deck::CardsSuitsGraphicsId suit[2];
	for (int i = 0; i < 2; i++)
	{
		val[i] = static_cast<Deck::CardsValuesGraphicsId>(ToInt(Notifications::GetNotificationNamedArg(cmd, "CardValue" + to_string(i))) - 1);
		suit[i] = static_cast<Deck::CardsSuitsGraphicsId>(ToInt(Notifications::GetNotificationNamedArg(cmd, "CardSuit" + to_string(i))) - 1);

		if (val[i] == Deck::CardsValuesGraphicsId::GI_ACE_TOP)
			val[i] = Deck::CardsValuesGraphicsId::GI_ACE;
	}
	gameState->localPlayer.setCards(Card(val[0], suit[0]), Card(val[1], suit[1]));
}

void GameState::CmdOpenBoardCards::execute()
{
	AnsiString stage = Notifications::GetNotificationNamedArg(cmd, "Stage");

	vector<size_t> cardsIndexes;
	if (stage == "Flop")
		cardsIndexes = { 0, 1, 2 };
	else if (stage == "Turn")
		cardsIndexes = { 3 };
	else if (stage == "River")
		cardsIndexes = { 4 };
	else
		SfmlMessageBox("GameState::CmdOpenBoardCards -> Unknown stage -> " + stage, "Error").show();

	for (size_t index : cardsIndexes)
	{
		auto& card = gameState->boardCards[index];
		auto cardValue = static_cast<Deck::CardsValuesGraphicsId>(ToInt(Notifications::GetNotificationNamedArg(cmd, "CardValue" + to_string(index))) - 1);

		Deck::setCard(&card,
			cardValue == Deck::CardsValuesGraphicsId::GI_ACE_TOP ? Deck::CardsValuesGraphicsId::GI_ACE : cardValue,
			static_cast<Deck::CardsSuitsGraphicsId>(ToInt(Notifications::GetNotificationNamedArg(cmd, "CardSuit" + to_string(index))) - 1));
		card.setFillColor(Color::White);
	}

	gameState->potPosition = MultiplyVector(APPLICATION_WINDOW_SIZE, 0.5f) + Vector2f(0, GAMESTATE_CARD_SIZE.y / 2 + 20.f);
}

void GameState::CmdSetWinners::execute()
{
	size_t winnersCount = ToInt(Notifications::GetNotificationNamedArg(cmd, "Count"));
	Balance gain = ToInt(Notifications::GetNotificationNamedArg(cmd, "Gain"));

	for (int i = 0; i < winnersCount; i++)
	{
		AnsiString winnerName = Notifications::GetNotificationNamedArg(cmd, "Player" + to_string(i));

		Player* winner = gameState->getPlayer(winnerName);

		if (winner == nullptr)
			SfmlMessageBox("Unknown winner name: " + winnerName, "Error").show();
	}
}

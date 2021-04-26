#pragma once
#ifndef _GAME_STATE_DEFINED_
#define _GAME_STATE_DEFINED_

#include "Types.h"
#include "Settings.h"
#include "State.h"
#include "Player.h"
#include "PokerButton.h"
#include "MessageBox.h"
#include "Resources.h"
#include "NetClient.h"
#include "Notifications.h"
#include "Deck.h"

#include <Button.h>
#include <TextBox.h>

#include <iostream>
#include <thread>
#include <functional>

class GameState : public State, public Observer
{
private:
	//Commands
	class Command;
	typedef shared_ptr<Command> ptr_command;
	class Command
	{
	public:
		Command(const AnsiString& cmd, GameState* gs) : gameState(gs), cmd(cmd) {}
		const AnsiString& str() const;

		static ptr_command Create(const AnsiString& cmd, GameState* gs);

		virtual void execute() = 0;

	protected:
		GameState* gameState;
		AnsiString cmd;
		
		friend class NetClient;
	};
	class CmdNewConnection : public Command
	{
	public:
		CmdNewConnection(GameState* gs) : Command(Notifications::CreateNofiticationMessage("NewConnection", "Name:" + gs->localPlayer.getNickname() + "|"), gs) {}
		void execute() override final;
	};
	class CmdSetPlayers : public Command
	{
	public:
		CmdSetPlayers(const AnsiString& cmd, GameState* gs) : Command(cmd, gs) {}
		void execute() override final;
	};
	class CmdStartGame : public Command
	{
	public:
		CmdStartGame(const AnsiString& cmd, GameState* gs) : Command(cmd, gs) {}
		void execute() override final;
	};
	class CmdStartHand : public Command
	{
	public:
		CmdStartHand(const AnsiString& cmd, GameState* gs) : Command(cmd, gs) {}
		void execute() override final;
	};
	class CmdPlayerDecisionRequest : public Command
	{
	public:
		CmdPlayerDecisionRequest(const AnsiString& cmd, GameState* gs) : Command(cmd, gs) {}
		CmdPlayerDecisionRequest(const AnsiString& answer) : Command(Notifications::CreateNofiticationMessage("ClientDecision", answer), nullptr) {}
		void execute() override final;
	};
	class CmdHideGui : public Command
	{
	public:
		CmdHideGui(GameState* gs) : Command("HideGui", gs) {}
		void execute() override final;
	};
	class CmdSetTable : public Command
	{
	public:
		CmdSetTable(const AnsiString& cmd, GameState* gs) : Command(cmd, gs) {}
		void execute() override final;
	};
	class CmdSetCards : public Command
	{
	public:
		CmdSetCards(const AnsiString& cmd, GameState* gs) : Command(cmd, gs) {}
		void execute() override final;
	};
	class CmdOpenBoardCards : public Command
	{
	public:
		CmdOpenBoardCards(const AnsiString& cmd, GameState* gs) : Command(cmd, gs) {}
		void execute() override final;
	};
	class CmdSetWinners : public Command
	{
	public:
		CmdSetWinners(const AnsiString& cmd, GameState* gs) : Command(cmd, gs) {}
		void execute() override final;
	};

public:
	const size_t BOARD_CARDS_COUNT = 5;
	enum ButtonId
	{
		BTN_CHECK,
		BTN_BET,
		BTN_CALL,
		BTN_RAISE,
		BTN_FOLD,
		BTN_COUNT
	};

public:
	GameState(StatesStack* statesStack, const String& nickName);
	~GameState() override;

	void update(float deltaTime, const Vector2f& mousePos) override;
	void updateSfmlEvent(sf::Event& ev) override;

	//Observer
	void handleEvent(const EventMessage& message) override;

protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	//Functional
	Player* getPlayer(const AnsiString& nickname) const;

private:
	//Updating
	void updateNetwork();
	void updateGui(float deltaTime, const Vector2f& mousePos);
	void updateGraphicsEntities();

	//void handleNetworkEvent(const EventMessageString& message);
	void handleGuiEvent(const EventMessage& message);
	void showControls(const AnsiString &buttonsNotation);

private:
	//Initialization
	bool playersInit(const String& localPlayerName);
	bool netInit();
	bool guiInit();
	bool sfmlGraphicsInit();

private:
	//Graphics
	Texture backgroundTexture, cardShirtTexture;
	RectangleShape background, boardCards[5];

	Text pot;
	Vector2f potPosition;

	Button buttonPrototype, buttons[BTN_COUNT];
	TextBox raiseTextBox;

	PokerButton tableButton;

private:
	//Networking
	NetClient netClient;

private:
	//Data
	Player localPlayer, opponentPlayer;
	Deck deck;
};

#endif
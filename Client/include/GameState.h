#pragma once
#ifndef _GAME_STATE_DEFINED_
#define _GAME_STATE_DEFINED_

#include <iostream>
#include <thread>
#include <functional>

#include "Types.h"
#include "Settings.h"
#include "State.h"
#include "Player.h"
#include "PokerButton.h"
#include "Button.h"
#include "MessageBox.h"
#include "Resources.h"
#include "NetClient.h"
#include "Notifications.h"
#include "Deck.h"

namespace network_mode
{
	enum class Value
	{
		UNKNOWN,
		OPEN_SERVER,
		JUST_CONNECT
	};
}

class GameState : public State, public Observerable, public Observer
{
public:
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
	GameState(StatesStack* statesStack, const String& nickName, network_mode::Value mode);
	~GameState() override;

	void update(float deltaTime, const Vector2f& mousePos) override;
	void updateSfmlEvent(sf::Event& ev) override;

	//Observer
	void handleEvent(const EventMessage& message) override;

	//Observerable
	void notifyObservers(const EventMessageString& message = "") override;

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

	void handleNetworkEvent(const EventMessageString& message);
	void handleGuiEvent(const EventMessage& message);

private:
	//Initialization
	bool playersInit(const String& localPlayerName);
	bool netInit(network_mode::Value networkMode);
	bool guiInit();
	bool sfmlGraphicsInit();

private:
	//Graphics
	sf::Texture backgroundTexture, cardShirtTexture;
	sf::RectangleShape background, myCards[2], oppCards[2];
	sf::Text pot;
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
#pragma once
#ifndef _GAME_STATE_DEFINED_
#define _GAME_STATE_DEFINED_

#include <iostream>
#include <thread>
#include <functional>

#include "CombinationIdentifier.h"

#include "Settings.h"
#include "State.h"
#include "Player.h"
#include "PokerButton.h"
#include "PokerServer.h"

#include "Button.h"

#include "Address.hpp"
#include "TcpClient.hpp"
#include "TcpServer.hpp"

#include "Resources.h"

using netboost::TcpEntity;
using netboost::TcpClient;
using netboost::TcpServer;
using netboost::ConnectionDescriptor;
using netboost::Packet;

using std::string;
using std::function;

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
	//Updating
	function<void()> updateNetwork = []() {};
	void updateGui(const Vector2f& mousePos);
	void updateGraphicsEntities();

private:
	//Initialization
	void playersInit(const String& localPlayerName);
	void netInit(network_mode::Value networkMode);
	void guiInit();
	void sfmlGraphicsInit();
	void startServer();

private:
	//Graphics
	sf::Texture backgroundTexture, cardShirtTexture;
	sf::RectangleShape background, myCards[2], oppCards[2];
	sf::Text pot;
	Button buttonPrototype, buttons[BTN_COUNT];

	PokerButton tableButton;

private:
	//Networking
	const Address serverAddr = Address(SERVER_ADDRESS);

	TcpEntity* tcpEntity = nullptr;
	ConnectionDescriptor connectionDescriptor = 0;

private:
	//Data
	Player localPlayer, opponentPlayer;
	PokerServer pokerGameServer;
};

#endif
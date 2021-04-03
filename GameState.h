#pragma once
#ifndef _GAME_STATE_DEFINED_
#define _GAME_STATE_DEFINED_

#include <iostream>
#include <thread>
#include <memory>

#include "CombinationIdentifier.h"

#include "Settings.h"
#include "State.h"
#include "Player.h"
#include "PokerHand.h"

#include "Button.h"

#include "Address.hpp"
#include "TcpClient.hpp"
#include "TcpServer.hpp"

using netboost::TcpEntity;
using netboost::TcpClient;
using netboost::TcpServer;
using netboost::ConnectionDescriptor;
using netboost::Packet;

using std::string;

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
	GameState(StatesStack* statesStack, const sf::String& nickName, network_mode::Value mode);
	~GameState();

	void update(float deltaTime, sf::Vector2f mousePos) override;
	void updateSfmlEvent(sf::Event& ev) override;

	//Observer
	void handleEvent(const ObsMessage& message) override;

	//Observerable
	void notifyObservers(const ObsMessageString& message = "") override;

protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	void netInit();
	void guiInit();
	void sfmlGraphicsInit();
	void startGame();

	void receiveOpponentNickname();
	void sendLocalPlayerNameToOpponent();

	sf::Vector2f findPositionForButton();

private:
	//Graphics
	sf::Texture backgroundTexture, cardShirtTexture;
	sf::RectangleShape background, myCards[2], oppCards[2];
	sf::Text pot;
	Button buttonPrototype, buttons[BTN_COUNT];

private:
	//Networking
	const Address serverAddr = Address(SERVER_ADDRESS);

	TcpEntity* tcpEntity = nullptr;
	TcpClient tcpClient;
	TcpServer tcpServer;
	ConnectionDescriptor connectionDescriptor = 0;
	network_mode::Value netMode;

private:
	//Data
	Player localPlayer, opponentPlayer;
	PokerHand pokerHand;
	bool gameIsActive = false;
};

#endif
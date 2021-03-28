#pragma once
#ifndef _GAME_STATE_DEFINED_
#define _GAME_STATE_DEFINED_

#include <iostream>
#include <queue>
#include <thread>
#include <memory>

#include "CombinationIdentifier.h"

#include "Settings.h"
#include "State.h"
#include "Player.h"

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

const std::string SERVER_ADDRESS = "127.0.0.1:12345";

namespace game_state_mode
{
	enum class Value
	{
		UNKNOWN,
		OPEN_SERVER,
		JUST_CONNECT
	};
}

class GameState : public State
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
	GameState(StatesStack* statesStack, const sf::String& nickName, game_state_mode::Value mode);
	~GameState();

	void update(float deltaTime, sf::Vector2f mousePos) override;
	void updateSfmlEvent(sf::Event& ev) override;

protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	void netInit();
	void guiInit();
	void sfmlGraphicsInit();

	void receiveOpponentNickname();
	void sendLocalPlayerNameToOpponent();

	sf::Vector2f findPositionForButton();

private:
	//Graphics
	sf::Texture backgroundTexture, cardShirtTexture;
	sf::RectangleShape background, myCards[2], oppCards[2];
	sf::Text pot;
	Button buttons[BTN_COUNT];

	//Networking
	const Address serverAddr = Address(SERVER_ADDRESS);

	TcpEntity* tcpEntity = nullptr;
	TcpClient tcpClient;
	TcpServer tcpServer;
	ConnectionDescriptor connectionDescriptor = 0;
	game_state_mode::Value netMode;

	//LocalPlayer
	Player localPlayer, opponentPlayer;
};

#endif
#include "Player.h"

Player::Player()
{
	g_playerString.setFont(ApplicationFonts::getFont(ApplicationFonts::ARIAL));
	g_playerString.setFillColor(sf::Color::Black);
	g_playerString.setCharacterSize(16);

	g_currentBet.setFont(ApplicationFonts::getFont(ApplicationFonts::ARIAL));
	g_currentBet.setCharacterSize(16);

	//Set all text positions
	//
	//
	//
	//Set all text positions

	sf::Image chipsImage;
	if (!chipsImage.loadFromFile("chips.png"))
	{
		//Handle error
	}
	chipsImage.createMaskFromColor(sf::Color::Black);
	tChips.loadFromImage(chipsImage);
	tChips.generateMipmap();
	chipsSprite.setTexture(&tChips);
	chipsSprite.setSize(sf::Vector2f(20, 20));
}

void Player::setNickname(const sf::String& name)
{
	this->name = name;
	g_playerString.setString(name + ": " + std::to_string(balance));

	setPlayerSlot(playerSlot);
}

void Player::setPlayerSlot(table_slots::Value slot)
{
	playerSlot = slot;

	switch (slot)
	{
	case table_slots::Value::TOP:
		g_playerString.setPosition(APPLICATION_WINDOW_SIZE.x / 2 - g_playerString.getGlobalBounds().width / 2, 135);
		chipsSprite.setPosition(g_playerString.getGlobalBounds().left + g_playerString.getGlobalBounds().width + 5, 135);
		g_currentBet.setPosition(APPLICATION_WINDOW_SIZE.x / 2 - g_currentBet.getGlobalBounds().width / 2, 155);
		break;

	case table_slots::Value::BOTTOM:
		g_playerString.setPosition(APPLICATION_WINDOW_SIZE.x / 2 - g_playerString.getGlobalBounds().width / 2, 450);
		chipsSprite.setPosition(g_playerString.getGlobalBounds().left + g_playerString.getGlobalBounds().width + 5, 450);
		g_currentBet.setPosition(APPLICATION_WINDOW_SIZE.x / 2 - g_currentBet.getGlobalBounds().width / 2, 425);
		break;

	default:
		//Error
		break;
	}
}

void Player::setPossibleActions(const string& actions)
{
	possibleActions = actions;
}

void Player::setBalance(Balance balance)
{
	this->balance = balance;
}

Balance Player::makeBet(Balance betValue)
{
	if (balance >= betValue)
	{
		balance -= betValue;
		currentBet += betValue;
		g_currentBet.setString(std::to_string(currentBet));
		return betValue;
	}
	else
	{
		unsigned oldBalance = balance;
		balance = 0;
		currentBet += oldBalance;
		g_currentBet.setString(std::to_string(currentBet));
		return oldBalance;
	}
}

void Player::zeroCurrentBet()
{
	currentBet = 0;
}

const sf::String& Player::getNickname() const
{
	return name;
}

unsigned int Player::getBalance() const
{
	return balance;
}

Balance Player::getCurrentBet() const
{
	return currentBet;
}

bool Player::hasAction(const string& action) const
{
	return possibleActions.find(action) != string::npos;
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(g_playerString);
	target.draw(chipsSprite);
	target.draw(g_currentBet);
}

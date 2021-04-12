#include "Player.h"

Player::Player()
{
	playerString.setFont(ApplicationFonts::getFont(ApplicationFonts::ARIAL));
	playerString.setFillColor(sf::Color::Black);
	playerString.setCharacterSize(16);

	//Set all text positions
	//
	//
	//
	//Set all text positions

	sf::Image chipsImage;
	if (!chipsImage.loadFromFile(Resources::path("chips.png")))
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
	playerString.setString(name + ": " + std::to_string(balance));

	setPlayerSlot(playerSlot);
}

void Player::setPlayerSlot(table_slots::Value slot)
{
	playerSlot = slot;

	switch (slot)
	{
	case table_slots::Value::TOP:
		playerString.setPosition(APPLICATION_WINDOW_SIZE.x / 2 - playerString.getGlobalBounds().width / 2, 135);
		chipsSprite.setPosition(playerString.getGlobalBounds().left + playerString.getGlobalBounds().width + 5, 135);
		break;

	case table_slots::Value::BOTTOM:
		playerString.setPosition(APPLICATION_WINDOW_SIZE.x / 2 - playerString.getGlobalBounds().width / 2, 450);
		chipsSprite.setPosition(playerString.getGlobalBounds().left + playerString.getGlobalBounds().width + 5, 450);
		break;

	default:
		//Error
		break;
	}
}

void Player::setPlayerDescision(PlayerDescision desc)
{
	action = desc;
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
		return betValue;
	}
	else
	{
		unsigned oldBalance = balance;
		balance = 0;
		currentBet += oldBalance;
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

Player::PlayerDescision Player::getAction() const
{
	return action;
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(playerString);
	target.draw(chipsSprite);
}

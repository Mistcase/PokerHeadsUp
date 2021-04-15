#include "Player.h"

Player::Player()
{
	playerString.setFont(ApplicationFonts::getFont(ApplicationFonts::ARIAL));
	playerString.setFillColor(sf::Color::Black);
	playerString.setCharacterSize(16);

	g_betValue.setFont(ApplicationFonts::getFont(ApplicationFonts::ARIAL));
	g_betValue.setFillColor(Color::White);
	g_betValue.setCharacterSize(14);

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
	chipsSprite.setSize(Vector2f(20, 20));

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
		g_betValue.setPosition(APPLICATION_WINDOW_SIZE.x / 2 - g_betValue.getGlobalBounds().width / 2, 158);
		break;

	case table_slots::Value::BOTTOM:
		playerString.setPosition(APPLICATION_WINDOW_SIZE.x / 2 - playerString.getGlobalBounds().width / 2, 450);
		chipsSprite.setPosition(playerString.getGlobalBounds().left + playerString.getGlobalBounds().width + 5, 450);
		g_betValue.setPosition(APPLICATION_WINDOW_SIZE.x / 2 - g_betValue.getGlobalBounds().width / 2, 428);
		break;

	default:
		//Error
		break;
	}
}

void Player::setBalance(Balance balance)
{
	this->balance = balance;
}

void Player::setBet(Bet betValue)
{
	currentBet = betValue;
}

Bet Player::makeBet(Bet betValue)
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

const String& Player::getNickname() const
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

void Player::update()
{
	playerString.setString(name + ": " + std::to_string(balance));
	g_betValue.setString(std::to_string(currentBet));
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(playerString);
	target.draw(chipsSprite);

	if (currentBet != 0)
		target.draw(g_betValue);
}

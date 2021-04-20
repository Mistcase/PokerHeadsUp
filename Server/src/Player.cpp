#include "Player.h"

Player::Player(const AnsiString &nickname, Balance startBalance, int id) : name(nickname), balance(startBalance), id(id)
{
}

void Player::setCards(const Card & c1, const Card & c2)
{
	cards[0] = c1;
	cards[1] = c2;
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

void Player::setActive(bool val)
{
    active = val;
}

void Player::addAction(const AnsiString& action)
{
    actionsMask |= PLAYERS_DESC_MAP.at(action);
}

void Player::removeAction(const AnsiString& action)
{
    actionsMask &= ~PLAYERS_DESC_MAP.at(action);
}

void Player::removeAllActions()
{
    actionsMask = 0;
}

const AnsiString& Player::getNickname() const
{
    return name;
}

Balance Player::getBalance() const
{
    return balance;
}

Balance Player::getCurrentBet() const
{
    return currentBet;
}

bool Player::isActive() const
{
    return active;
}

bool Player::hasAction(const AnsiString& action) const
{
    return actionsMask & PLAYERS_DESC_MAP.at(action);
}

pair<Card, Card> Player::getCards() const
{
	return pair<Card, Card>(cards[0], cards[1]);
}

int Player::getId() const
{
	return id;
}

const map<AnsiString, Player::PlayerDescisionId> Player::PLAYERS_DESC_MAP =
{
	{"CHECK", DESC_CHECK},
	{"CALL", DESC_CALL},
    {"BET", DESC_BET},
	{"RAISE", DESC_RAISE},
	{"FOLD", DESC_FOLD},
};
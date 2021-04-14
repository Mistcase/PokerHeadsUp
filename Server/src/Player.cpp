#include "Player.h"

Player::Player(const string &nickname, Balance startBalance) : name(nickname), balance(startBalance)
{
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

const string& Player::getNickname() const
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
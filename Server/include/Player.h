#ifndef _PLAYER_INCLUDED_
#define _PLAYER_INCLUDED_

#include <string>

using std::string;

typedef int Balance;

class Player
{
public:
    Player(const string& nickname, Balance startBalance);

	void setBalance(Balance balance);
	Balance makeBet(Balance betValue);
    void zeroCurrentBet();

	const string& getNickname() const;
	Balance getBalance() const;
	Balance getCurrentBet() const;
	//PlayerDescision getAction() const;

private:
	//Player data
	string name;
	Balance balance, currentBet = 0;
};

#endif
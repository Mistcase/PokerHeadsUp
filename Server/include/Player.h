#ifndef _PLAYER_INCLUDED_
#define _PLAYER_INCLUDED_

#include "Types.h"

typedef int Balance;

class Player
{
public:

	enum PlayerDescisionId : uint8_t
	{
		DESC_NONE,
		DESC_CHECK = 1,
		DESC_CALL = 1 << 2,
		DESC_BET = 1 << 3,
		DESC_RAISE = 1 << 4,
		DESC_FOLD = 1 << 5,
	};
	typedef uint8_t PlayerDescisionMask;

	static const map<AnsiString, PlayerDescisionId> PLAYERS_DESC_MAP;

public:
    Player(const AnsiString& nickname, Balance startBalance, int id);

	void setBalance(Balance balance);
	Balance makeBet(Balance betValue);
    void zeroCurrentBet();
	void setActive(bool val);
	void addAction(const AnsiString& action);
	void removeAction(const AnsiString& action);
	void removeAllActions();

	const AnsiString& getNickname() const;
	Balance getBalance() const;
	Balance getCurrentBet() const;
	bool isActive() const;
	bool hasAction(const AnsiString& action) const;

private:
	//Player data
	AnsiString name;
	Balance balance, currentBet = 0;
	PlayerDescisionMask actionsMask = PlayerDescisionId::DESC_NONE;
	int id;
	bool active = true;
};

#endif
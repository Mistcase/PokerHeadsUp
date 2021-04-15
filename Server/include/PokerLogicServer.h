#ifndef _POKER_LOGIC_SERVER_INCLUDED_
#define _POKER_LOGIC_SERVER_INCLUDED_

#include "Types.h"
#include "Notifications.h"
#include "RingQueue.h"
#include "Observer.h"
#include "Player.h"

#include <vector>
#include <thread>
#include <memory>

using std::thread;
using std::vector;

enum LOGIC_SERVER_INFO_TYPE
{
	INFO_BUTTON_PLAYER_NICKNAME,
	INFO_ALL_PLAYERS,
	INFO_CURRENT_PLAYER_NICKNAME,
	INFO_NEXT_PLAYER_NICKNAME,
	INFO_SMALL_BLIND_VALUE,
	INFO_BB_PLAYER_NICKNAME,
	INFO_SB_PLAYER_NICKNAME,
	INFO_TYPES_COUNT
};

class PokerLogicServer : public Observerable
{
private:
	enum StageId
	{
		STAGE_PREFLOP,
		STAGE_FLOP,
		STAGE_TURN,
		STAGE_RIVER
	};

	struct TableInfo
	{
		PlayersRingQueue players;
		Player *button = nullptr;
		Player *bb = nullptr;
		Player *sb = nullptr;
		Player *loopStartPlayer = nullptr;

		Balance smallBlind = 10, currentMaxBet = 0, pot = 0;

		bool handActive = false;
		bool onceComplited = false;
	};

	class Stage
	{
	public:
		virtual void makeLoopAction(TableInfo& table, StageContext& stageContext) = 0;
	};
	class PreflopStage : public Stage
	{
		PreflopStage();
		void makeLoopAction(TableInfo& table, StageContext& stageContext) override;
	};
	class FlopStage : public Stage
	{
		FlopStage();
		void makeLoopAction(TableInfo& table, StageContext& stageContext) override;
	};
	class TurnStage : public Stage
	{
		TurnStage();
		void makeLoopAction(TableInfo& table, StageContext& stageContext) override;
	};
	class RiverStage : public Stage
	{
		RiverStage();
		void makeLoopAction(TableInfo& table, StageContext& stageContext) override;
	};
	typedef std::shared_ptr<Stage> StageContext;

public:
	void setPlayersCount(size_t count);
	void handleMessage(const AnsiString &message);

private:
	void startNewHand();
	bool identifyNextHandAction();

private:
	void handleNewConnectedPlayer(const AnsiString &message);
	void handlePlayerDescision(const AnsiString &message);

private:
	//Observerable
	void notifyObservers(const EventMessageString &message = "") override;

private:
	StageContext handStage;
	TableInfo table;

	size_t playersCount = 0;
	bool gameStarted = false;
};

#endif
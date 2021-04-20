#ifndef _POKER_LOGIC_SERVER_INCLUDED_
#define _POKER_LOGIC_SERVER_INCLUDED_

#include "Types.h"
#include "Notifications.h"
#include "RingQueue.h"
#include "Observer.h"
#include "Player.h"
#include "Deck.h"

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

		Balance smallBlind = 10, currentMaxBet = 0, pot = 0;
		size_t interviewedPlayers = 0;
		
		Deck deck;
	};

	class Stage;
	typedef std::shared_ptr<Stage> stage_ptr;

	class StageContext
	{
	public:
		AnsiString update(TableInfo& table);
		void setStage(Stage* newStage);
	private:
		stage_ptr stage;
	};

	class Stage
	{
	public:
		Stage(TableInfo& table, StageContext& stageContext) : table(&table), stageContext(&stageContext) { table.interviewedPlayers = 0;}

		virtual AnsiString identifyPlayerAction(const AnsiString& params = "") = 0;

	protected:
		TableInfo* table;
		StageContext* stageContext;
	};
	class PreflopStage : public Stage
	{
	public:
		PreflopStage(TableInfo& table, StageContext& stageContext);
		AnsiString identifyPlayerAction(const AnsiString& params = "") override;
	};
	class NonPreflopStage : public Stage
	{
	public:
		NonPreflopStage(TableInfo& table, StageContext& stageContext);
		AnsiString identifyPlayerAction(const AnsiString& params = "") override;
	};

public:
	void setPlayersCount(size_t count);
	void handleMessage(const AnsiString &message);

private:
	void handleNewConnectedPlayer(const AnsiString &message);
	void handlePlayerDescision(const AnsiString &message);

private:
	//Observerable
	void notifyObservers(const EventMessageString &message = "") override;

private:
	StageContext stageContext;
	TableInfo table;

	size_t playersCount = 0;
	bool handActive = false;
	bool gameStarted = false;
};

#endif
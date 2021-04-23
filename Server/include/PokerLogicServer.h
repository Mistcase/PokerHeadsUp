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

using std::thread;
using std::vector;

class PokerLogicServer : public Observerable
{
private:
	struct TableInfo
	{
		size_t expectedPlayersCount;

		PlayersRingQueue players;
		Player *button = nullptr;
		Player *bb = nullptr;
		Player *sb = nullptr;

		Balance smallBlind = 10, currentMaxBet = 0, pot = 0;
		size_t interviewedPlayers = 0;
		
		vector<Card> boardCards;
		Deck deck;
	};

	class Stage;
	typedef shared_ptr<Stage> stage_ptr;
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
		virtual ~Stage();

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
	enum StageId : int32_t
	{
		STAGE_PREFLOP,
		STAGE_FLOP,
		STAGE_TURN,
		STAGE_RIVER,
	};

	class Command;
	class ExecutableCommand;
	typedef shared_ptr<Command> ptr_command;
	typedef shared_ptr<ExecutableCommand> ptr_executable_command;
	class Command
	{
	public:
		Command(PokerLogicServer* logicServer = nullptr, const AnsiString& strCmd = "") : cmd(strCmd) {}
		virtual ~Command() = default;

		AnsiString str();

	protected:
		AnsiString cmd;
	};
	class ExecutableCommand : public Command
	{
	public:
		static ptr_executable_command Create(const AnsiString& cmd, PokerLogicServer* logicServer = nullptr);

		ExecutableCommand(PokerLogicServer* logicServer, const AnsiString& strCmd) : logicServer(logicServer), Command(nullptr, strCmd) {}
		virtual void execute() = 0;

	protected:
		PokerLogicServer* logicServer;
	};
	class CmdNewConnection : public ExecutableCommand
	{
	public:
		CmdNewConnection(PokerLogicServer* logicServer, const AnsiString& strCmd = "") : ExecutableCommand(logicServer, strCmd) {}
		void execute() override final;
	};
	class CmdPlayersInfo : public Command
	{
	public:
		CmdPlayersInfo(PokerLogicServer* logicServer, const AnsiString& strCmd = "");
	};
	class CmdStartGame : public Command
	{
	public:
		CmdStartGame() : Command(nullptr, Notifications::CreateNofiticationMessage("StartGame")) {}
	};
	class CmdStartHand : public Command
	{
	public:
		CmdStartHand(PokerLogicServer* logicServer);
	};
	class CmdClientDecisionRequest : public ExecutableCommand
	{
	public:
		CmdClientDecisionRequest(PokerLogicServer* logicServer, const AnsiString& strCmd = "") : ExecutableCommand(logicServer, strCmd) {}
		void execute() override final;
	};
	class CmdHideGui : public Command
	{
	public:
		CmdHideGui() : Command(nullptr, Notifications::CreateNofiticationMessage("HideGui")) {}
	};
	class CmdTableInfo : public Command
	{
	public:
		CmdTableInfo(PokerLogicServer* logicServer);
	};
	class CmdSetCards : public Command
	{
	public:
		CmdSetCards(const Player* player);
	};
	class CmdSetBoardCards : public Command
	{
		CmdSetBoardCards(const vector<Card>& cards);
	};

public:
	void setPlayersCount(size_t count);
	void handleMessage(const AnsiString &message);

private:
	void sendCommand(ptr_command command);
	void notifyObservers(const EventMessageString &message = "") override;

private:
	StageContext stageContext;
	TableInfo table;

	bool gameStarted = false;
};

#endif
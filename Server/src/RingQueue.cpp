#include "RingQueue.h"

void PlayersRingQueue::push(Player *newPlayer)
{
    players.emplace_back(newPlayer);
}

Player *PlayersRingQueue::front() const
{
    return players.front();
}

Player *PlayersRingQueue::next() const
{
    if (!players.size())
        return nullptr;
    
    return (players.size() > 1) ? players[1] : players.front();
}   

void PlayersRingQueue::pop()
{
    players.emplace_back(players.front());
    players.erase(players.begin());
}

void PlayersRingQueue::deleteFront()
{
    players.erase(players.begin());
}

void PlayersRingQueue::zeroAllBets()
{
    for (auto& player : players)
        player->zeroCurrentBet();
}

bool PlayersRingQueue::playerExists(const AnsiString& name) const
{
    return std::find_if(players.cbegin(), players.cend(), [&name](const Player* player){ return player->getNickname() == name; }) != players.cend();
}

 bool PlayersRingQueue::allBetsAreEaqual() const
 {
     Balance bet = players.front()->getCurrentBet();
     for (const auto& player : players)
     {
         if (player->getCurrentBet() != bet)
            return false;
     }
     return true;
 }

size_t PlayersRingQueue::activePlayersCount() const
{
    size_t result = 0;
    for (const auto& player : players)
    {
        if (player->isActive())
            result++;
    }
    return result;
}

size_t PlayersRingQueue::nonZeroStackPlayersCount() const
{
	size_t result = 0;
	for (const auto& player : players)
	{
		if (player->getBalance() > 0)
			result++;
	}
	return result;
}

Balance PlayersRingQueue::getAllBets() const
{
	Balance result = 0;
	for (const auto& player : players)
		result += player->getCurrentBet();
	return result;
}

size_t PlayersRingQueue::size() const
{
    return players.size();
}

const vector<Player*>& PlayersRingQueue::getPlayersData() const
{
    return players;
}
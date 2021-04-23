#include "Server.h"

int main()
{
	netboost::Initialize();
    Server("127.0.0.1:12345", 2).start();

	/*PlayerHand hand1 = { Card(ACE, HEARTS), Card(KING, SPADES) };
	PlayerHand hand2 = { Card(TEN, HEARTS), Card(TWO, SPADES) };

	auto comb1 = CombinationIdentifier(hand1, { Card(ACE, DIAMONDS), Card(ACE, HEARTS), Card(ACE, CLUBS), Card(JACK, DIAMONDS), Card(EIGHT, DIAMONDS)}).identify();
	auto comb2 = CombinationIdentifier(hand2, { Card(ACE, DIAMONDS), Card(ACE, HEARTS), Card(ACE, CLUBS), Card(JACK, DIAMONDS), Card(EIGHT, DIAMONDS)}).identify();

	std::cout << (comb1 < comb2) << std::endl;*/

    return 0;
}
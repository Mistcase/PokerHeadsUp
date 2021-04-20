#include "Application.h"

#include "Notifications.h"

using namespace std;

int main()
{
	netboost::Initialize();

	Application app;
	app.run();

	/*Deck deck;
	Deck::Card& card = const_cast<Deck::Card&>(deck.getCard(Deck::CardsValuesGraphicsId::GI_TEN, Deck::CardsSuitsGraphicsId::GI_DIAMONDS));
	card.setPosition(100, 100);

	RenderWindow window(VideoMode(800, 600), "Window");
	while (window.isOpen())
	{
		Event ev;
		while (window.pollEvent(ev))
		{
			if (ev.type == Event::Closed)
				window.close();
		}

		window.clear();
		window.draw(card);
		window.display();
	}*/

	//cout << Resources::path("Arial.ttf") << endl;

	//InitGameRules();

	/*CombinationIdentifier id(std::make_pair(Card(NINE, DIAMONDS), Card(KING, SPADES)),
		{ Card(TEN, CLUBS) , Card(TWO, HEARTS) , Card(THREE, DIAMONDS), Card(EIGHT, DIAMONDS), Card(SEVEN, DIAMONDS) });
	auto comb = id.identify();


	using namespace std;
	using namespace poker_combs;
	switch (comb.combValue)
	{
	case ROYALE_FLUSH:
		cout << "ROYALE_FLUSH!\n";
		break;

	case STRAIGHT_FLUSH:
		cout << "STRAIGHT_FLUSH!\n";
		break;

	case FOUR_OF_KIND:
		cout << "FOUR_OF_KIND!\n";
		break;

	case FULL_HOUSE:
		cout << "FULL_HOUSE!\n";
		break;

	case FLUSH:
		cout << "FLUSH!\n";
		break;

	case STRAIGHT:
		cout << "STRAIGHT!\n";
		break;

	case THREE_OF_KIND:
		cout << "THREE_OF_KIND!\n";
		break;

	case TWO_PAIRS:
		cout << "TWO_PAIRS!\n";
		break;

	case PAIR:
		cout << "PAIR!\n";
		break;

	case HIGH_CARD:
		cout << "HIGH_CARD\n";
		break;
	}*/

	return 0;
}
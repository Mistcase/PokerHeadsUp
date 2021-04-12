#include "Application.h"

#include "Resources.h"

using namespace std;

int main()
{
	//cout << Resources::path("Arial.ttf") << endl;

	Application app;
	app.run();

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

	//netboost::Initialize();

	//std::string mode;
	//std::cin >> mode;

	//if (mode == "server")
	//{
	//	TcpServer server;
	//	server = TcpServer(Address("127.0.0.1:12345"));
	//	auto conn = server.openConnection();

	//	server.accept(conn);

	//	while (true)
	//	{
	//		std::cout << "Message: ";
	//		std::string str;
	//		cin >> str;

	//		server.write(conn, Packet(str.c_str(), str.size()));

	//		/*uint32_t size = str.size();
	//		server.write(conn, size, sizeof(uint32_t));
	//		server.write(conn, *str.c_str(), str.size());*/
	//	}

	//}

	//else if (mode == "client")
	//{
	//	TcpClient client;
	//	auto conn = client.connect(Address("127.0.0.1:12345"));
	//	while (true)
	//	{
	//		char buffer[256];
	//		ZeroMemory(buffer, 256);

	//		uint32_t size;

	//		Packet packet = client.readPacket(conn);
	//		/*client.read(conn, &size, sizeof(uint32_t));
	//		client.read(conn, buffer, size);*/

	//		for (int i = 0; i < packet.getSize(); i++)
	//			std::cout << (*((char*)packet.getData() + i));
	//		std::cout << std::endl;
	//	}
	//}

	return 0;
}
#include "Server.h"

#include <string>

#include <TcpClient.hpp>

int main()
{
    char mode = ' ';
    while (mode != 's' && mode != 'c')
    {
        std::cout << "Enter mode: ";
        std::cin >> mode;
    }

    if (mode == 's')
    {
        std::cout << "Starting server!\n";
        TcpServer server(Address("127.0.0.1:12345"));
        auto connection = server.openConnection();
        server.accept(connection);

        while (true)
        {
            static int counter = 0;
            if (server.readable(connection))
            {
                counter++;
                std::cout << "Message: " << server.readPacket(connection).getSize() << std::endl;
            }

            if (counter == 10)
                break;
        }

        std::cout << "Active: " << server.active(connection) << std::endl;
    }

    if (mode == 'c')
    {
        std::cout << "Starting client!\n";
        using netboost::TcpClient;
        TcpClient client;
        auto connection = client.connect(Address("127.0.0.1:12345"));
        std::cout << "Connected!\n";

        std::cout << "Enter any key...\n";
        getchar();
        getchar();
        std::cout << "Disconnect!\n";
        client.disconnect(connection);
    }

    //Server("127.0.0.1:12345", 2).start();

    return 0;
}
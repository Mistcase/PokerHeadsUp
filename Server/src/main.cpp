#include "Server.h"

#include <map>

int main()
{
    Server("127.0.0.1:12345", 2).start();

    return 0;
}
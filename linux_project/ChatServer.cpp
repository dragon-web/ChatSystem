#include "ChatServer.hpp"

int main()
{
    ChatServer* cs = new ChatServer();
    if(!cs)
    {
        std::cout << "Init ChatServer Fail" << std::endl;
        return -1;
    }
    cs->InitSvr();
    cs->Start();
    return 0;
}

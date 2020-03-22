#pragma once

#include <SFML/Network.hpp>
#include <deque>
#include <vector>
#include <map>
#include <thread>
#include <iomanip>
#include <iostream>

#include "PacketMgr.hpp"

#include "../Core/Flags.hpp"
#include "../Core/Library.hpp"

//#define STANDALONE

#ifdef SFML_SYSTEM_WINDOWS
#include <winsock2.h>
#else
#include <sys/socket.h>
#endif

class ReuseableListener : public sf::TcpListener
{
public:
    void reuse()
    {
        char reuse = 1;
        setsockopt(getHandle(), SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    }
};

class Server : public PacketMgr
{
public:
    Server();
    Server(int const &port);
    ~Server();

    int AcceptConnections();
    void RejectConnections();

    template <typename T>
    void Broadcast(Query query, T *data, size_t size)
    {
#ifdef DEBUG
        std::cout << "Info: Broadcasting packet..." << std::endl;
#endif
        for (auto &client : m_clients)
        {
            Send<T>(query, data, size, client);
        }
#ifdef DEBUG
        std::cout << "Info: Broadcast finished..." << std::endl;
#endif
    }

private:
    void Mgr();
    void TryAcceptNewClient();
    std::string GenerateToken(size_t length);

    std::string i_am() { return "Server"; }

private:
    int m_port;

    sf::SocketSelector m_socketSelector;
    ReuseableListener m_listener;
    std::vector<Connection> m_clients;

    bool m_active;
    bool m_accepting;
    bool m_receiving;
    bool m_sending;

    std::thread m_worker;
};

#ifdef STANDALONE

int main()
{
    Server server;
    server.AcceptConnections();

    while (true)
    {
    }
}
#endif
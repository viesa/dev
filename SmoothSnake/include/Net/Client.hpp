#pragma once

#include <deque>
#include <thread>

#include "PacketMgr.hpp"

#include "../Core/Flags.hpp"

class Client : public PacketMgr
{
public:
    Client();
    Client(sf::IpAddress const &hostIp, int const &port);
    ~Client();

    int Connect();
    void Disconnect();

    template <typename T>
    void Send(Query query, T *data, size_t size)
    {
        PacketMgr::Send<T>(query, data, size, m_server);
    }

    void SetToken(std::string const &token) { m_server.token = token; }

private:
    void Mgr();

    std::string i_am() { return "Client"; }

private:
    sf::IpAddress m_ip;
    int m_port;

    sf::SocketSelector m_socketSelector;
    Connection m_server;

    bool m_active;
    bool m_receiving;
    bool m_sending;
    bool m_connected;

    std::thread m_worker;

    unsigned int counter;
};